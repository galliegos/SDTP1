#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;


// Variáveis globais

int N;                  // tamanho buffer
int M = 100000;         // total de itens a consumir

vector<int> buffer;
int in = 0, out = 0;

sem_t sem_empty;
sem_t sem_full;
sem_t sem_mutex;

int consumidos = 0;
bool rodando = true;

// histórico ocupação
vector<int> historicoOcupacao;
pthread_mutex_t mutex_historico = PTHREAD_MUTEX_INITIALIZER;


// Primo

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

// Ocupação buffer

void registrarOcupacao() {
    pthread_mutex_lock(&mutex_historico);

    int val_full;
    sem_getvalue(&sem_full, &val_full);
    historicoOcupacao.push_back(val_full);

    pthread_mutex_unlock(&mutex_historico);
}


// Produtor

void* produtor(void* arg) {
    while (true) {

        if (!rodando) break;

        int num = rand() % 10000000 + 1;

        sem_wait(&sem_empty);
        sem_wait(&sem_mutex);

        // checa novamente após acordar
        if (!rodando) {
            sem_post(&sem_mutex);
            sem_post(&sem_empty);
            break;
        }

        buffer[in] = num;
        in = (in + 1) % N;

        registrarOcupacao();

        sem_post(&sem_mutex);
        sem_post(&sem_full);
    }

    return NULL;
}


// Consumidor

void* consumidor(void* arg) {
    while (true) {

        sem_wait(&sem_full);
        sem_wait(&sem_mutex);

        int num = buffer[out];
        out = (out + 1) % N;

        // sentinela
        if (num == -1) {
            sem_post(&sem_mutex);
            sem_post(&sem_empty);
            break;
        }

        consumidos++;

        if (consumidos >= M && rodando) {
            rodando = false;
        }

        registrarOcupacao();

        sem_post(&sem_mutex);
        sem_post(&sem_empty);

        isPrime(num);
    }

    return NULL;
}


// MAIN

int main(int argc, char* argv[]) {

    if (argc != 4) {
        cout << "Uso: " << argv[0] << " <N> <Np> <Nc>" << endl;
        return 1;
    }

    N = atoi(argv[1]);
    int Np = atoi(argv[2]);
    int Nc = atoi(argv[3]);

    buffer.resize(N);
    srand(time(NULL));

    sem_init(&sem_empty, 0, N);
    sem_init(&sem_full, 0, 0);
    sem_init(&sem_mutex, 0, 1);

    vector<pthread_t> produtores(Np);
    vector<pthread_t> consumidores(Nc);

    auto start = chrono::high_resolution_clock::now();

    // cria threads
    for (int i = 0; i < Np; i++)
        pthread_create(&produtores[i], NULL, produtor, NULL);

    for (int i = 0; i < Nc; i++)
        pthread_create(&consumidores[i], NULL, consumidor, NULL);

    // espera atingir M consumidos
    while (true) {
        sem_wait(&sem_mutex);

        if (consumidos >= M) {
            rodando = false;
            sem_post(&sem_mutex);
            break;
        }

        sem_post(&sem_mutex);
    }

    // libera produtores bloqueados
    for (int i = 0; i < Np; i++)
        sem_post(&sem_empty);

    // espera produtores terminarem
    for (int i = 0; i < Np; i++)
        pthread_join(produtores[i], NULL);

    // envia sentinelas aos consumidores
    for (int i = 0; i < Nc; i++) {
        sem_wait(&sem_empty);
        sem_wait(&sem_mutex);

        buffer[in] = -1;
        in = (in + 1) % N;

        sem_post(&sem_mutex);
        sem_post(&sem_full);
    }

    // espera consumidores
    for (int i = 0; i < Nc; i++)
        pthread_join(consumidores[i], NULL);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;

    cout << "Tempo: " << diff.count()
         << "s | Consumidos: "
         << consumidos << endl;

    ofstream file("ocupacao.txt");
    for (int v : historicoOcupacao)
        file << v << "\n";
    file.close();

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_mutex);

    return 0;
}

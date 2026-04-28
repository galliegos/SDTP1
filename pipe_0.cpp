#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define MSG_SIZE 20

using namespace std;

// Verifica se número é primo
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Função que encapsula a lógica do PRODUTOR
void executarProdutor(int fd_escrita, int quantidade) {
    srand(time(NULL));

    int n = 1;
    char buffer[MSG_SIZE];

    for (int i = 0; i < quantidade; i++) {
        int delta = rand() % 100 + 1;
        n += delta;

        snprintf(buffer, MSG_SIZE, "%d", n);
        write(fd_escrita, buffer, MSG_SIZE);
    }

    // Envia 0 para encerrar o consumidor
    snprintf(buffer, MSG_SIZE, "%d", 0);
    write(fd_escrita, buffer, MSG_SIZE);

    close(fd_escrita);
}

// Função que encapsula a lógica do CONSUMIDOR
void executarConsumidor(int fd_leitura) {
    char buffer[MSG_SIZE];

    while (true) {
        int bytes = read(fd_leitura, buffer, MSG_SIZE);
        if (bytes <= 0) break;

        int num = atoi(buffer);

        if (num == 0) break;

        if (isPrime(num))
            cout << "Consumidor: " << num << " é primo" << endl;
        else
            cout << "Consumidor: " << num << " NÃO é primo" << endl;
    }

    close(fd_leitura);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <quantidade>" << endl;
        return 1;
    }

    int quantidade = atoi(argv[1]);
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Processo FILHO (Consumidor)
        close(pipefd[1]); // fecha ponta de escrita
        executarConsumidor(pipefd[0]);
    } 
    else {
        // Processo PAI (Produtor)
        close(pipefd[0]); // fecha ponta de leitura
        executarProdutor(pipefd[1], quantidade);
        
        wait(NULL); // espera o filho terminar
    }

    return 0;
}

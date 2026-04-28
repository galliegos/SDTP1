import matplotlib.pyplot as plt
import os

# =========================
# GRÁFICO 1 - TEMPO
# =========================

data = [
    (1,1,1,5.974414),(1,1,2,6.073620),(1,1,4,6.673910),(1,1,8,6.829598),
    (1,2,1,6.625897),(1,4,1,6.752126),(1,8,1,6.876060),

    (10,1,1,0.430690),(10,1,2,0.525735),(10,1,4,1.891304),(10,1,8,2.073435),
    (10,2,1,0.525705),(10,4,1,1.849223),(10,8,1,2.089448),

    (100,1,1,0.112527),(100,1,2,0.289596),(100,1,4,2.006310),(100,1,8,1.748695),
    (100,2,1,0.280536),(100,4,1,1.855554),(100,8,1,1.743886),

    (1000,1,1,0.079778),(1000,1,2,0.278124),(1000,1,4,1.665327),(1000,1,8,1.801117),
    (1000,2,1,0.323012),(1000,4,1,1.753939),(1000,8,1,1.993460),
]

configs = [(1,1),(1,2),(1,4),(1,8),(2,1),(4,1),(8,1)]
labels = [f"Np={np},Nc={nc}" for np,nc in configs]

results = {}
for N,Np,Nc,t in data:
    results.setdefault(N,{})[(Np,Nc)] = t

plt.figure()

for N in sorted(results):
    y = [results[N][cfg] for cfg in configs]
    plt.plot(labels, y, marker='o', label=f"N={N}")

plt.xlabel("Configuração de Threads")
plt.ylabel("Tempo (s)")
plt.title("Gráfico 1 - Tempo vs Threads")
plt.legend()
plt.grid()

plt.savefig("grafico_tempo.png")
plt.show()

# =========================
# GRÁFICO 2 - OCUPAÇÃO
# =========================

def ler_arquivo(path):
    with open(path) as f:
        return [int(x.strip()) for x in f.readlines()]

# CASOS EXTREMOS
arquivo_cheio = "ocupacao_N100_NP8_NC1.txt"
arquivo_vazio = "ocupacao_N100_NP1_NC8.txt"

ocup_cheio = ler_arquivo(arquivo_cheio)
ocup_vazio = ler_arquivo(arquivo_vazio)

# eixo tempo
t1 = list(range(len(ocup_cheio)))
t2 = list(range(len(ocup_vazio)))

plt.figure()

plt.plot(t1, ocup_cheio, label="Buffer cheio (Np=8, Nc=1)")
plt.plot(t2, ocup_vazio, label="Buffer vazio (Np=1, Nc=8)")

plt.xlabel("Tempo (amostras)")
plt.ylabel("Ocupação do buffer")
plt.title("Gráfico 2 - Ocupação do Buffer")

plt.legend()
plt.grid()

plt.savefig("grafico_ocupacao.png")
plt.show()
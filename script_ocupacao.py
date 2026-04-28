import matplotlib.pyplot as plt
import os

# =========================
# FUNÇÃO PARA LER ARQUIVO
# =========================
def ler_arquivo(path):
    with open(path) as f:
        return [int(x.strip()) for x in f.readlines()]

# =========================
# CONFIGURAÇÕES
# =========================
Ns = [1, 10, 100, 1000]
configs = [
    (1,1),(1,2),(1,4),(1,8),
    (2,1),(4,1),(8,1)
]

# =========================
# CRIAR SUBPLOTS
# =========================
fig, axes = plt.subplots(2, 2, figsize=(12, 8))
axes = axes.flatten()

for i, N in enumerate(Ns):
    ax = axes[i]

    for Np, Nc in configs:
        nome = f"ocupacao_N{N}_NP{Np}_NC{Nc}.txt"

        if not os.path.exists(nome):
            continue

        dados = ler_arquivo(nome)
        tempo = list(range(len(dados)))

        ax.plot(tempo, dados, label=f"Np={Np},Nc={Nc}")

    ax.set_title(f"N = {N}")
    ax.set_xlabel("Tempo")
    ax.set_ylabel("Ocupação")
    ax.grid()
    ax.legend(fontsize=7)

plt.suptitle("Gráfico 2 - Ocupação do Buffer (todos os casos)")

plt.tight_layout()
plt.savefig("grafico_ocupacao_completo.png")
plt.show()
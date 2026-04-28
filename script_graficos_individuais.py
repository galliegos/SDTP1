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
Ns = [100, 1000]

configs = [
    (1,1),(1,2),(1,4),(1,8),
    (2,1),(4,1),(8,1)
]

# pasta de saída
os.makedirs("graficos_individuais", exist_ok=True)

# =========================
# GERAR GRÁFICOS
# =========================
for N in Ns:
    for Np, Nc in configs:

        nome_arquivo = f"ocupacao_N{N}_NP{Np}_NC{Nc}.txt"

        if not os.path.exists(nome_arquivo):
            continue

        dados = ler_arquivo(nome_arquivo)
        tempo = list(range(len(dados)))

        plt.figure()

        plt.plot(tempo, dados)

        plt.title(f"Ocupação do Buffer\nN={N}, Np={Np}, Nc={Nc}")
        plt.xlabel("Tempo (amostras)")
        plt.ylabel("Ocupação")
        plt.grid()

        # salva
        nome_saida = f"graficos_individuais/N{N}_Np{Np}_Nc{Nc}.png"
        plt.savefig(nome_saida)

        plt.close()

print("Todos os gráficos individuais foram gerados!")

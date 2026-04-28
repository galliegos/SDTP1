#!/bin/bash

# Nome do executável (ajuste se necessário)
EXE="./prod_cons"

# Arquivo de saída para a tabela
OUTPUT_FILE="resultados_estudo_caso.csv"

# Cabeçalho da tabela
echo "N;Np;Nc;Tempo_Medio(s)" > $OUTPUT_FILE

# Definição dos parâmetros conforme o roteiro
BUFFERS=(1 10 100 1000)
THREADS=("1 1" "1 2" "1 4" "1 8" "2 1" "4 1" "8 1")

echo "Iniciando bateria de testes..."
echo "--------------------------------------"

for N in "${BUFFERS[@]}"; do
    for T in "${THREADS[@]}"; do
        # Divide a string de threads em Np e Nc
        set -- $T
        NP=$1
        NC=$2
        
        echo "Testando: N=$N, Np=$NP, Nc=$NC..."
        
        total_time=0
        
        # Executa 10 vezes para calcular a média
        for i in {1..10}; do
            # Roda o programa e captura o tempo impresso (ajustado para o padrão do seu código)
            # O grep busca o valor numérico após "Tempo: "
            saida=$($EXE $N $NP $NC)
            tempo=$(echo "$saida" | grep -oP 'Tempo: \K[0-9.]+')
            
            # Acumula o tempo (usa 'bc' para cálculos de ponto flutuante)
            total_time=$(echo "$total_time + $tempo" | bc -l)
        done
        
        # Calcula a média das 10 execuções
        avg_time=$(echo "scale=6; $total_time / 10" | bc -l)
        
        # Salva na tabela CSV
        echo "$N;$NP;$NC;$avg_time" >> $OUTPUT_FILE
        
        # Opcional: Salva o arquivo de ocupação da última execução com um nome específico
        # para você não perdê-lo
        mv "ocupacao.txt" "ocupacao_N${N}_NP${NP}_NC${NC}.txt"
        
        echo "  Média: $avg_time s"
    done
done

echo "--------------------------------------"
echo "Testes concluídos! Resultados salvos em: $OUTPUT_FILE"

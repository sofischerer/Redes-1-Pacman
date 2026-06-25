#!/bin/bash

./teste 1 &
PID_INSTANCIA_1=$! # Guarda o ID do processo para podermos fechar depois

# Aguarda 1 segundo para garantir que a primeira instância inicializou o socket
sleep 1

# 2. Roda a segunda instância (ex: em outra interface ou a mesma)
./teste 0 &
PID_INSTANCIA_2=$!

echo "Ambas as instâncias estão rodando..."
echo "Pressione [CTRL+C] para encerrar os testes."

# Função que limpa os processos quando você der CTRL+C no terminal
trap "echo 'Encerrando instâncias...'; sudo kill $PID_INSTANCIA_1 $PID_INSTANCIA_2; exit" INT

# Mantém o script principal rodando para você ver os printf() de ambas no mesmo terminal
wait
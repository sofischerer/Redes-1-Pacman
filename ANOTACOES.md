## ANOTAÇÕES
- inclui um makefile, o "make" sozinho só imprime as opções de make
    - "./pacman -new" pra resetar o jogo (atualmente só funciona no single)
    - ./pacman pra carregar

#### MOVIMENTO
- pacman anda com ASDW
- fantasmas viram apenas quando batem na parede
    - vermelho: sentido horário
    - azul: sentido anti-horário
    - verde: alterna entre horário e anti-horário
    - amarelo: vira para direção aleatória (pode ser que dê meia-volta)
- quando o fantasma colide com outro fantasma, o outro é tratado como parede.

#### TABULEIRO
- tabuleiro.csv nem é tocado
- ao dar um ./pacman -new , o jogo cria um jogo.csv
    - esse jogo.csv é sobrescrito em cada rodada
- VIEW: uma matriz 40x40 com só o raio do pacman visível
    - o jogo faz um .csv e manda pro cliente
        - o cliente imprime

# IMPORTANTE

- o game.c é a main do servidor
- o main.c é a main do cliente

- alterar funçoes:
    - tesoura.c , tesoura.h : colocar parâmetros do socket
    - game.c , main.c : dar CTRL + F e colocar ^
        - envia_instrucao
        - recebe_instrucao
        - envia_arquivo
        - recebe_arquivo
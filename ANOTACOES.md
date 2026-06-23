## ANOTAÇÕES
- inclui um makefile, dá um "make single" pra rodar
    - "./pacman -new" pra resetar o jogo
    - ./pacman pra carregar

#### MOVIMENTO
- pacman anda com ASDW
    - a ideia é enviar um char (minúsculo) pro servidor
        - o servidor recebe o char e calcula posicoes etc.
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
    - a ideia é enviar só a view pro cliente
        - o cliente imprime

#### OUTROS
subtituir funçoes no modelo final:
- mygetch (lê input sem precisar de enter)
- todas as funcoes de print_ 
- dá um CTRL + F em ENVIAR
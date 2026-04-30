#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

typedef struct posicao{
    int x;
    int y;
}posicao;

typedef struct type_board{
    posicao P;  //pacman
    posicao R;  //fantasma vermelho
    posicao B;  //fantasma azul
    posicao G;  //fantasma verde
    posicao Y;  //fantasma amarelo
    posicao itens[6]; //itens
    char** tabuleiro;
}type_board;

void criar_jogo(FILE* entrada, type_board* jogo){
    char linha[100];
    for (int i=0; i<40; i++){
        fgets(linha, 100, entrada);
        for (int j=0; j<40; j++){
            char c = linha[2*j];
            jogo->tabuleiro[i][j] = c;
            switch (c) {
                case 'P':
                    jogo->P.x = j;
                    jogo->P.y = i;
                    break;

                case 'R':
                    jogo->R.x = j;
                    jogo->R.y = i;
                    break;

                case 'B':
                    jogo->B.x = j;
                    jogo->B.y = i;
                    break;

                case 'G':
                    jogo->G.x = j;
                    jogo->G.y = i;
                    break;

                case 'Y':
                    jogo->Y.x = j;
                    jogo->Y.y = i;
                    break;

                case '1':
                    jogo->itens[0].x = j;
                    jogo->itens[0].y = i;
                    break;

                case '2':
                    jogo->itens[1].x = j;
                    jogo->itens[1].y = i;
                    break;

                case '3':
                    jogo->itens[2].x = j;
                    jogo->itens[2].y = i;
                    break;

                case '4':
                    jogo->itens[3].x = j;
                    jogo->itens[3].y = i;
                    break;

                case '5':
                    jogo->itens[4].x = j;
                    jogo->itens[4].y = i;
                    break;

                case '6':
                    jogo->itens[5].x = j;
                    jogo->itens[5].y = i;
                    break;
            }
        }
    }   
}

void print_jogo(type_board* jogo){
    for (int i=0; i<40; i++){
        for (int j=0; j<40; j++){
            printf("%c ", jogo->tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("P:%d,%d R:%d,%d B:%d,%d G:%d,%d Y:%d,%d ", jogo->P.x, jogo->P.y, jogo->R.x, jogo->R.y, jogo->B.x, jogo->B.y, jogo->G.x, jogo->G.y, jogo->Y.x, jogo->Y.y);
    printf("Items: [%d,%d;%d,%d;%d,%d;%d,%d;%d,%d;%d,%d]\n", jogo->itens[0].x, jogo->itens[0].y, jogo->itens[1].x, jogo->itens[1].y, jogo->itens[2].x, jogo->itens[2].y, jogo->itens[3].x, jogo->itens[3].y, jogo->itens[4].x, jogo->itens[4].y, jogo->itens[5].x, jogo->itens[5].y);
}

/*
    Primeiro argumento = mapa de entrada em csv
*/
int main(int argc, char *argv[]){

    //Parametros
    char* arquivo = "tabuleiro.csv";
    char* rede = "lo";
    int debug = 0;

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-debug") == 0) {
            debug = 1;
        }
        else if (strcmp(argv[i], "-rede") == 0 && i + 1 < argc) {
            rede = argv[++i];
        }
        else if (strcmp(argv[i], "-mapa") == 0 && i + 1 < argc) {
            arquivo = argv[++i];
        }
    }

    //Declarações
    type_board jogo;
    char** tabuleiro = calloc(40, sizeof(char*));
    for (int i=0; i<40; i++){
        tabuleiro[i] = calloc(40, sizeof(char));
    }
    jogo.tabuleiro = tabuleiro;

    FILE* entrada = fopen(arquivo, "r");

    //Criar tabuleiro a partir do csv    
    criar_jogo(entrada, &jogo);
    // print_jogo(&jogo);

    //Inicia conexão
    int socket = cria_raw_socket(rede);
    
    
    
    //Começar jogo
    //  while(1){

    //}

}
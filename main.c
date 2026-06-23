#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "network.h"
#include "files.h"
#include "client.h"
/* LADO DO CLIENTE */
    
int main(int argc, char *argv[]){
    srand(0);
    int ch;
    int playing = GAME_RUNNING;
    int andou = 0;
    int turn = 0;
    int dist = 1;
    char carregar = '0';
    char** view;
    //Parametros
    char* arquivo = "jogo.csv";
/*
    char* rede = "lo";
    int debug = 0;
*/

    for (int i = 1; i < argc; i++) {
/*
        if (strcmp(argv[i], "-debug") == 0) {
            debug = 1;
        }
        else if (strcmp(argv[i], "-rede") == 0 && i + 1 < argc) {
            rede = argv[++i];
        }
        else if (strcmp(argv[i], "-mapa") == 0 && i + 1 < argc) {
            *arquivo = argv[++i];
        }
        */
    }
    /* checa se ha algum arquivo que nao conseguiu ser enviado */
    carregar = load_save_c();
    dist = load_save_n();
    if( carregar != '0'){
        if( carregar > 'a')
            playing = GAME_OVER;
    }

    //Declarações
    type_board* jogo;
    FILE* entrada = fopen(arquivo, "r");
    if (entrada == NULL){
        perror("ERRO AO ABRIR TABULEIRO");
        return 1;
    }
    jogo = malloc( sizeof( type_board)) ;

}
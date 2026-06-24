#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "network.h"
#include "files.h"
#include "client.h"
#include "tesoura.h"
/* LADO DO CLIENTE */
    
int main(int argc, char *argv[]){
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

    char ch;
    char** view = calloc(40, sizeof(char*));
    for (int i=0; i<40; i++)
        view[i] = calloc(40, sizeof(char));

    while(1){
        /* coleta input ASDW */
        ch = mygetch();
        while( (ch != KEY_W) && (ch != KEY_D) && (ch != KEY_S) && (ch != KEY_A)){
            ch = mygetch();
        }

        /* manda mensagem pro servidor*/
        switch( ch){
            case KEY_W: /* CIMA */

            break;
            case KEY_D: /* DIR */
            
            break;
            case KEY_S:/* BAIXO */

            break;
            case KEY_A: /* ESQ */

            break;
        }

        /* recebe dados e monta matriz view */
        /* OU */
        /* recebe arquivo e abre */
        /* caso timeout, break */
        /*
        .
        .
        .
        .
        .
        .
        */

    }

    /* frees */
    for (int i=0; i<40; i++)
        free(view[i]);
    free(view);
}
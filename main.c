#include <stdio.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include "network.h"
#include "files.h"
#include "client.h"
#include "tesoura.h"

#define TIMEOUTCMDSCLIENT for (int i=0; i<40; i++) free(view[i]); free(view); return 0;

/* LADO DO CLIENTE */
    
int main(int argc, char *argv[]){
    char* trash = {"trash"};
    char* rede = "lo";
/*
    int debug = 0;
*/
    for (int i = 1; i < argc; i++) {
/*
        if (strcmp(argv[i], "-debug") == 0) {
            debug = 1;
        }
        else */if (strcmp(argv[i], "-rede") == 0 && i + 1 < argc) {
            rede = argv[++i];
        }/*
        else if (strcmp(argv[i], "-mapa") == 0 && i + 1 < argc) {
            *arquivo = argv[++i];
        }
        */
    }

    char ch;
    char** view = calloc(40, sizeof(char*));
    for (int i=0; i<40; i++)
        view[i] = calloc(40, sizeof(char));
    system( " touch view.csv");

    int instr, codarquivo;
    int sock = cria_raw_socket(rede);

    //Resquicios de uma tentativa de deixar o código mais versátil
    uint8_t bcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    uint8_t meu_mac[6];
    struct ifreq ifr;
    strncpy(ifr.ifr_name, rede, IFNAMSIZ - 1);
    memcpy(meu_mac, ifr.ifr_hwaddr.sa_data, 6);


    int rodando = 1;
    while(rodando == 1){
        /* recebe instrucao se prox envio eh arquivo ou view */
        instr = recebe_instrucao(NULL, sock, rede, bcast, meu_mac);
        if( instr == COD_TIMEOUT){
            TIMEOUTCMDSCLIENT
        } else if( instr == ARQ){
            /* recebe o nome do arquivo e abre */

            codarquivo = recebe_instrucao(NULL, sock, rede, bcast, meu_mac);
            switch(codarquivo){
                case COD_ARQ1:
                    if(recebe_arquivo(FILE_1, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_1);
                    break;
                case COD_ARQ2:
                    if( recebe_arquivo( FILE_2, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_2);
                    break;
                case COD_ARQ3:
                    if( recebe_arquivo( FILE_3, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_3);
                    break;
                case COD_ARQ4:
                    if( recebe_arquivo( FILE_4, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_4);
                    break;
                case COD_ARQ5:
                    if( recebe_arquivo( FILE_5, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_5);
                    break;
                case COD_ARQ6:
                    if( recebe_arquivo( FILE_6, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_6);
                    break;
                case COD_ARQ7:
                    if( recebe_arquivo( FILE_R, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_R);
                    break;
                case COD_ARQ8:
                    if( recebe_arquivo( FILE_B, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_B);
                    break;
                case COD_ARQ9:
                    if( recebe_arquivo( FILE_G, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_G);
                    break;
                case COD_ARQ10:
                    if( recebe_arquivo( FILE_Y, sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_Y);
                    break;
                default:
                    printf("ERRO: CODIGO DO ARQUIVO N EXISTE\n");
                    rodando = 0;
                    break;
            }
        } else if( instr == VISUALIZACAO){
            /* recebe o .csv e imprime */
            
            if( recebe_arquivo( "view.csv", sock, rede, bcast, meu_mac) < 1){ TIMEOUTCMDSCLIENT }
            read_board( "view.csv", view);
            print_view( view);

        } else if( instr == MOVE){
            /* coleta input ASDW */

            ch = mygetch();
            while( (ch != KEY_W) && (ch != KEY_D) && (ch != KEY_S) && (ch != KEY_A) && (ch != KEY_Q))
                ch = mygetch();

            /* manda mensagem pro servidor*/
            switch( ch){
                case KEY_W: /* CIMA */
                    if( envia_instrucao(CIMA, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_D: /* DIR */
                    if( envia_instrucao(DIREITA, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_S:/* BAIXO */
                    if( envia_instrucao(BAIXO, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_A: /* ESQ */
                    if( envia_instrucao(ESQUERDA, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_Q: /* QUIT */
                    if( envia_instrucao(END, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
            }
        } else if( instr == END ){
            rodando = 0;
        } else{
            printf("ERRO = ?\n");
            rodando = 0;
        }
    }

    /* frees */
    for (int i=0; i<40; i++)
        free(view[i]);
    free(view);

    /* free das conexoes aq */

    return 0;
}
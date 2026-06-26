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

    uint8_t instr, codarquivo;

    //Resquicios de uma tentativa de deixar o código mais versátil
    uint8_t bcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    uint8_t meu_mac[6];
    struct ifreq ifr;
    strncpy(ifr.ifr_name, rede, IFNAMSIZ - 1);
    memcpy(meu_mac, ifr.ifr_hwaddr.sa_data, 6);

    int rodando = 1;
    while(rodando == 1){
        /* recebe instrucao se prox envio eh arquivo ou view */
        instr = recebe_instrucao( );
        if( instr == INSTR_TIMEOUT){
            TIMEOUTCMDSCLIENT
        } else if( instr == INSTR_MANDA_ARQUIVO){
            /* recebe o nome do arquivo e abre */

            codarquivo = recebe_instrucao( );
            switch( codarquivo){
                case INSTR_ARQ_PAS_1:
                    if( recebe_arquivo( trash, FILE_1) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_1);
                    break;
                case INSTR_ARQ_PAS_2:
                    if( recebe_arquivo( trash, FILE_2) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_2);
                    break;
                case INSTR_ARQ_PAS_3:
                    if( recebe_arquivo( trash, FILE_3) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_3);
                    break;
                case INSTR_ARQ_PAS_4:
                    if( recebe_arquivo( trash, FILE_4) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_4);
                    break;
                case INSTR_ARQ_PAS_5:
                    if( recebe_arquivo( trash, FILE_5) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_5);
                    break;
                case INSTR_ARQ_PAS_6:
                    if( recebe_arquivo( trash, FILE_6) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_6);
                    break;
                case INSTR_ARQ_FAN_R:
                    if( recebe_arquivo( trash, FILE_R) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_R);
                    break;
                case INSTR_ARQ_FAN_B:
                    if( recebe_arquivo( trash, FILE_B) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_B);
                    break;
                case INSTR_ARQ_FAN_G:
                    if( recebe_arquivo( trash, FILE_G) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_G);
                    break;
                case INSTR_ARQ_FAN_Y:
                    if( recebe_arquivo( trash, FILE_Y) < 1){ TIMEOUTCMDSCLIENT }
                    system( OPEN_FILE_Y);
                    break;
                default:
                    printf("ERRO: CODIGO DO ARQUIVO N EXISTE\n");
                    rodando = 0;
                    break;
            }
        } else if( instr == INSTR_MANDA_BOARD){
            /* recebe o .csv e imprime */
            
            if( recebe_arquivo( trash, "view.csv") < 1){ TIMEOUTCMDSCLIENT }
            read_board( "view.csv", view);
            print_view( view);

        } else if( instr == INSTR_MANDA_ASDW){
            /* coleta input ASDW */

            ch = mygetch();
            while( (ch != KEY_W) && (ch != KEY_D) && (ch != KEY_S) && (ch != KEY_A) && (ch != KEY_Q))
                ch = mygetch();

            /* manda mensagem pro servidor*/
            switch( ch){
                case KEY_W: /* CIMA */
                    if( envia_instrucao( INSTR_MOVE_UP) == INSTR_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_D: /* DIR */
                    if( envia_instrucao( INSTR_MOVE_RIGHT) == INSTR_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_S:/* BAIXO */
                    if( envia_instrucao( INSTR_MOVE_DOWN) == INSTR_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_A: /* ESQ */
                    if( envia_instrucao( INSTR_MOVE_LEFT) == INSTR_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
                case KEY_Q: /* QUIT */
                    if( envia_instrucao( INSTR_MOVE_QUIT) == INSTR_TIMEOUT){ TIMEOUTCMDSCLIENT }
                    break;
            }
        } else if( instr == INSTR_END ){
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
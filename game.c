#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "network.h"
#include "files.h"
#include "server.h"
#include "tesoura.h"



#define TIMEOUTCMDS destruir_jogo(jogo); destruir_view(view); return 0;

/* LADO DO SERVIDOR */

int main(int argc, char *argv[]){
    srand(0);
    int dir;
    int playing = GAME_RUNNING;
    int andou = 0;
    int turn = 0;
    int dist = 1;
    char carregar = '0';
    char** view;
    //Parametros
    char* arquivo = "jogo.csv";
    char* rede = "lo";

    //Resquicios de uma tentativa de deixar o código mais versátil
    uint8_t bcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    uint8_t meu_mac[6];
    struct ifreq ifr;
    strncpy(ifr.ifr_name, rede, IFNAMSIZ - 1);
    memcpy(meu_mac, ifr.ifr_hwaddr.sa_data, 6);
/*
    int debug = 0;
*/

    for (int i = 1; i < argc; i++) {
/*
        if (strcmp(argv[i], "-debug") == 0) {
            debug = 1;
        }
        else*/ if (strcmp(argv[i], "-rede") == 0 && i + 1 < argc) {
            rede = argv[++i];
        }/*
        else if (strcmp(argv[i], "-mapa") == 0 && i + 1 < argc) {
            *arquivo = argv[++i];
        }
        else */ if (strcmp(argv[i], "-new") == 0) {
            system("cp tabuleiro.csv jogo.csv");
            system("touch save.txt");
            write_save('0', 1);
        }
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

    //Criar tabuleiro a partir do csv    
    criar_jogo(entrada, jogo);
    fclose(entrada);
    view = criar_view();
    update_view(view, jogo, dist);
    write_board(view, "view.csv");

    //Cria socket
    int sock = cria_raw_socket(rede);

    //Inicializa
    if(envia_instrucao(INICIALIZACAO, view, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDS }
    
    
    //Começar jogo
    while(playing == GAME_RUNNING){
        /* carrega itens */
        if( carregar > '0'){
            write_save( carregar, (char)dist);
            if (envia_instrucao(ARQ, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT) { TIMEOUTCMDS };
            switch( carregar){
            case '1':
                if( envia_instrucao(TXT, FILE_1, sock, rede, bcast, meu_mac, 1) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case '2':
                if( envia_instrucao(TXT, FILE_2, sock, rede, bcast, meu_mac, 2) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case '3':
                if( envia_instrucao(JPG, FILE_3, sock, rede, bcast, meu_mac, 3) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case '4':
                if( envia_instrucao(JPG, FILE_4, sock, rede, bcast, meu_mac, 4) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case '5':
                if( envia_instrucao(MP4, FILE_5, sock, rede, bcast, meu_mac,5) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case '6':
                if( envia_instrucao(MP4, FILE_6, sock, rede, bcast, meu_mac,6) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            }
            write_save('0', (char)dist);
            if( pegou_tudo(jogo)){
                destruir_jogo(jogo);
                destruir_view(view);
                return 0;
            }
        }

        /* input de tecla ASDW */
        if( envia_instrucao(MOVE, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDS }

        dir = recebe_instrucao(NULL, sock, rede, bcast, meu_mac);
        if( dir == COD_TIMEOUT){ TIMEOUTCMDS }
        switch( dir){
            case CIMA:
                jogo->P.dir = UP;
                break;
            case ESQUERDA:
                jogo->P.dir = LEFT;
                break;
            case DIREITA:
                jogo->P.dir = RIGHT;
                break;
            case BAIXO:
                jogo->P.dir = DOWN;
                break;
        }

        /* move os personagens */
        if( playing == GAME_RUNNING){ /* 0 andou; 1-6 pastilha; 8-11 fantasma */
            andou = move_pac(jogo, dir);
            carregar = '0';
            if( andou > 0){
                switch( andou){
                    case HIT_R:
                        carregar = 'r';
                        break;
                    case HIT_B:
                        carregar = 'b';
                        break;
                    case HIT_G:
                        carregar = 'g';
                        break;
                    case HIT_Y:
                        carregar = 'y';
                        break;
                    default:
                        carregar = '0' + andou;
                        break;
                }
            }
            if( andou >= 8)
                playing = GAME_OVER;
        }
        if( playing == GAME_RUNNING){
            playing = GAME_OVER;
            if( move_fan(jogo, &(jogo->R), 'R') == 1) /* 1 = hit */
                carregar = 'r';
            else if( move_fan(jogo, &(jogo->B), 'B') == 1)
                carregar = 'b';
            else if( move_fan(jogo, &(jogo->G), 'G') == 1)
                carregar = 'g';
            else if( move_fan(jogo, &(jogo->Y), 'Y') == 1)
                carregar = 'y';
            else playing = GAME_RUNNING;
        }
        if( playing == GAME_RUNNING){
            /* se algum fantasma passou por cima de alguma pastilha, checa se eh para desenhar de volta */
            for( int i = 0; i < 6; i++)
                if( jogo->itens[i].status > 0){
                    if( jogo->tabuleiro[jogo->itens[i].pos.y][jogo->itens[i].pos.x] == '0'){
                        jogo->tabuleiro[jogo->itens[i].pos.y][jogo->itens[i].pos.x] = i + '1';
                        jogo->itens[i].status = 0;
                    }
                }
            turn++;
            if( (turn % 5) == 0)
                if( dist < 127){
                    dist++;
                    write_save('0', (char)dist);
                }
            update_view(view, jogo, dist);
            write_board(view, "view.csv");
            if( envia_instrucao(VISUALIZACAO, view, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDS }
            write_board(jogo->tabuleiro, "jogo.csv");
        }
    }

    if( playing == GAME_OVER){
        write_save(carregar, (char)dist);
        if (envia_instrucao(ARQ, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT) { TIMEOUTCMDS }
        switch( carregar){
            case 'r':
                if( envia_instrucao(JPG, FILE_R, sock, rede, bcast, meu_mac, 7) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case 'b':
                if( envia_instrucao(JPG, FILE_B, sock, rede, bcast, meu_mac, 8) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case 'g':
                if( envia_instrucao(JPG, FILE_G, sock, rede, bcast, meu_mac, 9) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
            case 'y':
                if( envia_instrucao(JPG, FILE_Y, sock, rede, bcast, meu_mac, 10) == COD_TIMEOUT){ TIMEOUTCMDS }
                break;
        }
        write_save('0', (char)dist);
        if( envia_instrucao(END, NULL, sock, rede, bcast, meu_mac, 0) == COD_TIMEOUT){ TIMEOUTCMDS }
    }

    destruir_jogo(jogo);
    destruir_view(view);
    return 0;
}
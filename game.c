#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "files.h"
#include "server.h"


/* LADO DO SERVIDOR */

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
        else */ if (strcmp(argv[i], "-new") == 0) {
            system("cp tabuleiro.csv jogo.csv");
            system("touch save.txt");
            write_save('0', 1);
        }
    }
    
    /* checa se ha algum arquivo que nao conseguiu ser enviado */
    carregar = load_save_c();           /*.*/
    dist = load_save_n();               /*.*/
    if( carregar != '0'){               /* NAO MEXER */
        if( carregar > 'a')             /*.*/
            playing = GAME_OVER;        /*.*/
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


    /* ---------- */
    /* envia a matriz */
    print_view(view);
    /* ----------*/

    //Inicia conexão
    //int socket = cria_raw_socket(rede);
    
    
    
    //Começar jogo
    while(( playing == GAME_RUNNING) || (playing == GAME_PAUSED)){
        /* carrega itens */
        if( carregar > '0'){
            write_save( carregar, (char)dist);

            /* ----------*/
            print_load( carregar); /* mensagem de que esta enviando/recebendo */
            /* ----------*/

            /* ---------- */
            /* ESSE TRECHO */
            /* o case eh o nome do arquivo que tem que enviar */
            /* mais pra baixo tem outro switch pros fantasmas */
            switch( carregar){
                case '1':
                    system( OPEN_FILE_1);
                    break;
                case '2':
                    system( OPEN_FILE_2);
                    break;
                case '3':
                    system( OPEN_FILE_3);
                    break;
                case '4':
                    system( OPEN_FILE_4);
                    break;
                case '5':
                    system( OPEN_FILE_5);
                    break;
                case '6':
                    system( OPEN_FILE_6);
                    break;
            }
            /* ---------- */

            write_save('0', (char)dist); /* NAO MEXER */
            if( pegou_tudo(jogo)){

                /* ---------- */
                /* jogador ganhou, envia alguma coisa? */
                print_win();
                /* ---------- */

                destruir_jogo(jogo);
                destruir_view(view);
                return 0;
            }
        }

        /* input de tecla ASDW */
        ch = 0;
        while( (ch != KEY_W) && (ch != KEY_A) && (ch != KEY_D) && (ch != KEY_S)){

            /* ---------- */
            /* aqui ele recebe a msg do cliente */
            ch = mygetch();
            /* ---------- */

        }
        switch( ch){
            case KEY_W:
                jogo->P.dir = UP;
                break;
            case KEY_D:
                jogo->P.dir = RIGHT;
                break;
            case KEY_S:
                jogo->P.dir = DOWN;
                break;
            case KEY_A:
                jogo->P.dir = LEFT;
                break;
        }

        /* move os personagens */
        if( playing == GAME_RUNNING){ /* 0 andou; 1-6 pastilha; 8-11 fantasma */
            andou = move_pac(jogo, ch);
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

             /* ---------- */
            /* envia a matriz */
            print_view(view);
            /* ----------*/

            write_board(jogo);
        }
    }

    if( playing == GAME_OVER){
        write_save(carregar, (char)dist);

        /* ----------*/
            print_load( carregar); /* mensagem de que esta enviando/recebendo */
        /* ----------*/

        /* ----------*/
        /* TRECHO - mandar arquivo */
        switch( carregar){
            case 'r':
                system( OPEN_FILE_R);
                break;
            case 'b':
                system( OPEN_FILE_B);
                break;
            case 'g':
                system( OPEN_FILE_G);
                break;
            case 'y':
                system( OPEN_FILE_Y);
                break;
        /* ----------*/


        }
        write_save('0', (char)dist); /* NAO MEXER */

        /* ---------- */
        /* jogador perdeu, envia alguma coisa? */
        print_gameover();
        /* ---------- */

    }

    destruir_jogo(jogo);
    destruir_view(view);
    return 0;
}
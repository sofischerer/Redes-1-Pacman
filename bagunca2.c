#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
//#include "network.h"
#include "files.h"
#include "server.h"
#include "client.h"
#include "debug.h"

/*----------------*/

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
    print_view(view);
    //Inicia conexão
    //int socket = cria_raw_socket(rede);
    
    
    
    //Começar jogo
    while(( playing == GAME_RUNNING) || (playing == GAME_PAUSED)){
        /* carrega itens */
        if( carregar > '0'){
            write_save( carregar, (char)dist);
            print_load( carregar);
            switch( carregar){
                case '1':
                    /* ENVIA */
                    system( OPEN_FILE_1);
                    break;
                case '2':
                    /* ENVIA */
                    system( OPEN_FILE_2);
                    break;
                case '3':
                    /* ENVIA */
                    system( OPEN_FILE_3);
                    break;
                case '4':
                    /* ENVIA */
                    system( OPEN_FILE_4);
                    break;
                case '5':
                    /* ENVIA */
                    system( OPEN_FILE_5);
                    break;
                case '6':
                    /* ENVIA */
                    system( OPEN_FILE_6);
                    break;
            }
            write_save('0', (char)dist);
            if( pegou_tudo(jogo)){
                print_win();
                destruir_jogo(jogo);
                destruir_view(view);
                return 0;
            }
            playing = GAME_PAUSED;
            print_pause();
        }

        /* input de tecla ASDW */
        ch = 0;
        if( playing == GAME_PAUSED){
            ch = mygetch();
            while( ch != KEY_P)
                ch = mygetch();
            playing = GAME_RUNNING;
            ch = 0;
            update_view(view, jogo, dist);
            print_view(view);
        }
        while( (ch != KEY_W) && (ch != KEY_A) && (ch != KEY_D) && (ch != KEY_S) && (ch != KEY_Q) && (ch != KEY_P)){
            ch = mygetch();
        }
        switch( ch){
            case KEY_W:
                /* UP */
                jogo->P.dir = UP;
                break;
            case KEY_A:
                /* LEFT */
                jogo->P.dir = LEFT;
                break;
            case KEY_D:
                /* RIGHT */
                jogo->P.dir = RIGHT;
                break;
            case KEY_S:
                /* DOWN */
                jogo->P.dir = DOWN;
                break;
            case KEY_Q:
                /* Q = QUIT */
                printf("\nQUIT\n");
                playing = GAME_QUIT;
                destruir_jogo(jogo);
                destruir_view(view);
                return 0;
            case KEY_P:
                /* P = PAUSE/UNPAUSE */
                playing = GAME_PAUSED;
                print_pause();
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
            print_view(view);
            write_board(jogo);
        }
    }

    if( playing == GAME_OVER){
        write_save(carregar, (char)dist);
        print_load( carregar);
        switch( carregar){
            case 'r':
                /* ENVIA */
                system( OPEN_FILE_R);
                break;
            case 'b':
                /* ENVIA */
                system( OPEN_FILE_B);
                break;
            case 'g':
                /* ENVIA */
                system( OPEN_FILE_G);
                break;
            case 'y':
                /* ENVIA */
                system( OPEN_FILE_Y);
                break;
        }
        write_save('0', (char)dist);
        print_gameover();
    }

    destruir_jogo(jogo);
    destruir_view(view);
    return 0;
}
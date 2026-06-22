#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
//#include "network.h"
#include "server.h"

/* isso vai no client.c */
#ifndef CLIENT_H
#define CLIENT_H

# ifndef KEYS
# define KEYS
# define KEY_W 119
# define KEY_A 97
# define KEY_D 100
# define KEY_S 115
# define KEY_Q 113
# define KEY_P 112
# define GAME_QUIT 0
# define GAME_RUNNING 1
# define GAME_PAUSED 2
# define GAME_OVER 3
# define GAME_WIN 4
# endif

// int mygetch ( void );

#endif

int mygetch ( void ) 
{
  int ch;
  struct termios oldt, newt;
  
  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
  
  return ch;
}
/*----------------*/

int main(int argc, char *argv[]){
    srand(0);
    int ch;
    int playing = GAME_RUNNING;
    int pastilha = 0;
    //Parametros
    char* arquivo = "tabuleiro.csv";
/*
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
*/
    //Declarações
    type_board* jogo;
    FILE* entrada = fopen(arquivo, "r");
    if (entrada == NULL){
        perror("ERRO AO ABRIR TABULEIRO");
        fclose(entrada);
        return 1;
    }
    jogo = malloc( sizeof( type_board));

    //Criar tabuleiro a partir do csv    
    criar_jogo(entrada, jogo);
    fclose(entrada);
    print_jogo(jogo);

    //Inicia conexão
    //int socket = cria_raw_socket(rede);
    
    
    
    //Começar jogo
    //  while(1){

    //}
    while(( playing == GAME_RUNNING) || (playing == GAME_PAUSED)){
        /* input de tecla ASDW */
        ch = 0;
        if( playing == GAME_PAUSED){
            ch = mygetch();
            while( ch != KEY_P)
                ch = mygetch();
            playing = GAME_RUNNING;
            ch = 0;
            print_jogo(jogo);
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
                return 0;
            case KEY_P:
                /* P = PAUSE/UNPAUSE */
                playing = GAME_PAUSED;
                for( int i = 0; i < 20; i++)
                    printf("\n");
                printf("                            JOGO PAUSADO (pressione P)\n");
                for( int i = 0; i < 21; i++)
                    printf("\n");
                break;
        }

        /* move os personagens */
        if( playing == GAME_RUNNING)
            switch( move_pac(jogo, ch)){
                case HIT_WALL:
                    break;
                case HIT_GHOST:
                    playing = GAME_OVER;
                    break;
                case COLLECTED_ALL:
                    playing = GAME_WIN;
                    break;
            }
        if(( playing == GAME_RUNNING) || (pastilha = 1)){
            if( move_fan(jogo, &(jogo->R), 'R') == HIT_GHOST){
                playing = GAME_OVER;
            }
            /*
        if( playing == GAME_RUNNING)
            if( move_fan(jogo, &(jogo->B), 'B') == HIT_GHOST){
                playing = GAME_OVER;
            }
        if( playing == GAME_RUNNING){
            if( move_fan(jogo, &(jogo->G), 'G') == HIT_GHOST){
                playing = GAME_OVER;
            }
        }
        if( playing == GAME_RUNNING)
            if( move_fan(jogo, &(jogo->Y), 'Y') == HIT_GHOST){
                playing = GAME_OVER;
            }
            */
        }
        if(( playing == GAME_RUNNING) || (pastilha = 1)){
            /* se algum fantasma passou por cima de alguma pastilha, checa se eh para desenhar de volta */
            for( int i = 0; i < 6; i++)
                if( jogo->itens[i].status > 0){
                    if( jogo->tabuleiro[jogo->itens[i].pos.y][jogo->itens[i].pos.x] == '0'){
                        jogo->tabuleiro[jogo->itens[i].pos.y][jogo->itens[i].pos.x] = i + '1';
                        jogo->itens[i].status = 0;
                    }
                }
            print_jogo(jogo);
        }
        pastilha = 0;
    }

    switch( playing){
        case GAME_WIN:
            for( int i = 0; i < 20; i++)
                printf("\n");
            printf("                            PARABÉNS! VOCÊ GANHOU\n");
            for( int i = 0; i < 21; i++)
                printf("\n");
            break;
        case GAME_OVER:
            for( int i = 0; i < 20; i++)
                printf("\n");
            printf("                                    GAME OVER\n");
            for( int i = 0; i < 21; i++)
                printf("\n");
            break;
    }

    /* TRASH */
    destruir_jogo(jogo);
}
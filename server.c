#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "network.h"
#include "server.h"
#include "color.h"

void criar_jogo(FILE* entrada, type_board* jogo){
    jogo->tabuleiro = calloc(40, sizeof(char*));
    for (int i=0; i<40; i++){
        jogo->tabuleiro[i] = calloc(40, sizeof(char));
    }
    for( int i = 0; i < 6; i++){
        jogo->itens[i].pos.x = -1;
        jogo->itens[i].pos.y = -1;
    }
    char linha[100];
    for (int i=0; i<40; i++){
        fgets(linha, 100, entrada);
        for (int j=0; j<40; j++){
            char c = linha[2*j];
            jogo->tabuleiro[i][j] = c;
            switch (c) {
                case 'P':
                    jogo->P.pos.x = j;
                    jogo->P.pos.y = i;
                    break;

                case 'R':
                    jogo->R.pos.x = j;
                    jogo->R.pos.y = i;
                    jogo->R.dir = RANDOM(4);
                    break;

                case 'B':
                    jogo->B.pos.x = j;
                    jogo->B.pos.y = i;
                    jogo->B.dir = RANDOM(4);
                    break;

                case 'G':
                    jogo->G.pos.x = j;
                    jogo->G.pos.y = i;
                    jogo->G.dir = RANDOM(4);
                    break;

                case 'Y':
                    jogo->Y.pos.x = j;
                    jogo->Y.pos.y = i;
                    jogo->Y.dir = RANDOM(4);
                    break;

                case '1':
                    jogo->itens[0].pos.x = j;
                    jogo->itens[0].pos.y = i;
                    break;

                case '2':
                    jogo->itens[1].pos.x = j;
                    jogo->itens[1].pos.y = i;
                    break;

                case '3':
                    jogo->itens[2].pos.x = j;
                    jogo->itens[2].pos.y = i;
                    break;

                case '4':
                    jogo->itens[3].pos.x = j;
                    jogo->itens[3].pos.y = i;
                    break;

                case '5':
                    jogo->itens[4].pos.x = j;
                    jogo->itens[4].pos.y = i;
                    break;

                case '6':
                    jogo->itens[5].pos.x = j;
                    jogo->itens[5].pos.y = i;
                    break;
            }
        }
    }   
}
void destruir_jogo(type_board* jogo){
    for (int i=0; i<40; i++){
        free(jogo->tabuleiro[i]);
    }
    free(jogo->tabuleiro);
    free(jogo);
}

void anda_interno(type_board* jogo, personagem* chara, char c){
    jogo->tabuleiro[chara->pos.y][chara->pos.x] = '0';
    switch( (chara->dir) % 4){
        case UP:
            chara->pos.y--;
            break;
        case LEFT:
            chara->pos.x--;
            break;
        case RIGHT:
            chara->pos.x++;
            break;
        case DOWN:
            chara->pos.y++;
            break;
    }
    jogo->tabuleiro[chara->pos.y][chara->pos.x] = c;
}


void vira_fan(personagem* chara, char c){
    switch( c){
        case 'R':
            chara->dir = HORARIO(chara->dir);
            break;
        case 'B':
            chara->dir = ANTIHORARIO(chara->dir);
            break;
        case 'G':
            if( chara->dir < 4)
                chara->dir = HORARIO(chara->dir);
            else
                chara->dir = ANTIHORARIO(chara->dir);
            break;
        case 'Y':
            chara->dir = RANDOM(4);
            break;
    }
}

int move_pac(type_board* jogo, int key){
    int px = jogo->P.pos.x;
    int py = jogo->P.pos.y;
    /* armazena temporariamente a nova posicao */
    switch( key){
        case KEY_W:
            py--;
            jogo->P.dir = UP;
            break;
        case KEY_A:
            px--;
            jogo->P.dir = LEFT;
            break;
        case KEY_D:
            px++;
            jogo->P.dir = RIGHT;
            break;
        case KEY_S:
            py++;
            jogo->P.dir = DOWN;
            break;
    }

    /* checa colisoes */
    switch( jogo->tabuleiro[py][px]){
        case 'X':
            /* parede - para*/
            break;
        case '0':
            /* vazio - move */
            anda_interno(jogo, &(jogo->P), 'P');
            break;
        case '1' ... '6':
            /* FILE - retorna */
            jogo->itens[(jogo->tabuleiro[py][px] - '1')].pos.x = -1;
            jogo->itens[(jogo->tabuleiro[py][px] - '1')].pos.y = -1;
            jogo->itens[(jogo->tabuleiro[py][px] - '1')].status = -1;
            int n = (jogo->tabuleiro[py][px] - '0');
            anda_interno(jogo, &(jogo->P), 'P');
            return (n); /* retorna 1-6 */
        /* fantasmas - retorna 8-11*/
        case 'R':
            return HIT_R;
        case 'B':
            return HIT_B;
        case 'G':
            return HIT_G;
        case 'Y':
            return HIT_Y;
    }
    return 0;
}

/* retorna 1 se atinge pacman */
int move_fan(type_board* jogo, personagem* chara, char c){
    int andou = 0;
    int virou = 0;
    int px;
    int py;
    
    while( andou == 0){
        px = chara->pos.x;
        py = chara->pos.y;
        /* armazena temporariamente a posicao */
        switch( chara->dir % 4){
            case UP:
                py--;
                break;
            case LEFT:
                px--;
                break;
            case RIGHT:
                px++;
                break;
            case DOWN:
                py++;
                break;
        }
        
        /* checa colisoes */
        switch( jogo->tabuleiro[py][px]){
            case '0':
                /* vazio - move */
                anda_interno(jogo, chara, c);
                andou = 1;
                break;
            case 'P':
                return 1;
            case '1' ... '6':
                /* pastilha - passa por cima  */
                jogo->itens[(jogo->tabuleiro[py][px] - '1')].status = 1;
                anda_interno(jogo, chara, c);
                andou = 1;
                break;
            default:
                /* parede ou outros fantasmas */
                vira_fan( chara, c);
                virou = 1;
                break;
        }
    }
    if( (c == 'G') && (virou == 1))
        chara->dir = (chara->dir + 4) % 8;
    return 0;
}

int pegou_tudo(type_board* jogo){
    int collall = 1;
    for( int i = 0; i < 6; i++)
        if( jogo->itens[i].status >= 0){
            collall = 0;
        }
    if( collall == 1)
        return 1;
    return 0;
}

void print_jogo(type_board* jogo){
    char c;
    for (int i=0; i<40; i++){
        for (int j=0; j<40; j++){
            c = jogo->tabuleiro[i][j];
            switch (c) {
                case 'P':
                    printf("%s %c%s", BOLD, c, RESET);
                    break;
                case 'R':
                    printf("%s %c%s", RED, c, RESET);
                    break;
                case 'B':
                    printf("%s %c%s", BLUE, c, RESET);
                    break;
                case 'G':
                    printf("%s %c%s", GREEN, c, RESET);
                    break;
                case 'Y':
                    printf("%s %c%s", YELLOW, c, RESET);
                    break;
                case '1' ... '6':
                    printf("%s %c%s", GOLD, c, RESET);
                    break;
                case 'X':
                    printf("%s %c%s", WALL, c, RESET);
                    break;
                default:
                    printf("%s %c%s", BLANK, c, RESET);
                    break;
            }
        }
        printf("\n");
    }
    printf("%sP:%2d,%2d     R:%2d,%2d     B:%2d,%2d     G:%2d,%2d     Y:%2d,%2d\n", BLANK, jogo->P.pos.x, jogo->P.pos.y, jogo->R.pos.x, jogo->R.pos.y, jogo->B.pos.x, jogo->B.pos.y, jogo->G.pos.x, jogo->G.pos.y, jogo->Y.pos.x, jogo->Y.pos.y);
    printf("Items: [%2d,%2d;  %2d,%2d;  %2d,%2d;  %2d,%2d;  %2d,%2d;  %2d,%2d]\n", jogo->itens[0].pos.x, jogo->itens[0].pos.y, jogo->itens[1].pos.x, jogo->itens[1].pos.y, jogo->itens[2].pos.x, jogo->itens[2].pos.y, jogo->itens[3].pos.x, jogo->itens[3].pos.y, jogo->itens[4].pos.x, jogo->itens[4].pos.y, jogo->itens[5].pos.x, jogo->itens[5].pos.y);
}
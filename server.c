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

int horario( int dir){
    return( (dir + 1) % 4);
}

int antihorario( int dir){
    return( ((dir + 3) % 4) + 4);
}

void vira_fan(personagem* chara, char c){
    switch( c){
        case 'R':
            chara->dir = horario( chara->dir);
            break;
        case 'B':
            chara->dir = antihorario( chara->dir);
            break;
        case 'G':
            if( chara->dir < 4)
                chara->dir = horario( chara->dir);
            else
                chara->dir = antihorario( chara->dir);
            break;
        case 'Y':
            chara->dir = RANDOM(4);
            break;
    }
}

int move_pac(type_board* jogo, int key){
    int px = jogo->P.pos.x;
    int py = jogo->P.pos.y;
    int collall = 0;
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
            return HIT_WALL;
        case '0':
            /* vazio - move */
            anda_interno(jogo, &(jogo->P), 'P');
            break;
        case '1' ... '6':
            /* FILE - faz alguma coisa */



            /* */
            jogo->itens[(jogo->tabuleiro[py][px] - '1')].pos.x = -1;
            jogo->itens[(jogo->tabuleiro[py][px] - '1')].pos.y = -1;
            jogo->itens[(jogo->tabuleiro[py][px] - '1')].status = -1;
            anda_interno(jogo, &(jogo->P), 'P');

            /* checa se todas as pastilhas foram coletadas */
            collall = 1;
            for( int i = 0; i < 6; i++)
                if( jogo->itens[i].status >= 0){
                    collall = 0;
                }
            if( collall == 1)
                return COLLECTED_ALL;
            break;
        default:
            /* fantasma */
            return HIT_GHOST;
    }
    return 0;
}

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
                return HIT_GHOST;
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

/*
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
*/
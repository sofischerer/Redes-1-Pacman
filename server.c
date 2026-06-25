#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "color.h"
#include "tesoura.h"
#include "server.h"

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

int move_pac(type_board* jogo, uint8_t key){
    int px = jogo->P.pos.x;
    int py = jogo->P.pos.y;
    /* armazena temporariamente a nova posicao */
    switch( key){
        case INSTR_MOVE_UP:
            py--;
            jogo->P.dir = UP;
            break;
        case INSTR_MOVE_LEFT:
            px--;
            jogo->P.dir = LEFT;
            break;
        case INSTR_MOVE_RIGHT:
            px++;
            jogo->P.dir = RIGHT;
            break;
        case INSTR_MOVE_DOWN:
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
        
        /* checa se esta tentando movimentar para fora do board */
        if( (px < 0) || (py < 0) || (px >= 40) || (py >= 40)){
            /* trata como parade e vira */
            vira_fan( chara, c);
            virou = 1;
            break;
        } else
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
char** criar_view(){
    char** view = calloc(40, sizeof(char*));
    for (int i=0; i<40; i++)
        view[i] = calloc(40, sizeof(char));
    return view;
}
void destruir_view(char** view){
    for (int i=0; i<40; i++)
        free(view[i]);
    free(view);
}
void update_view(char** view, type_board* jogo, int dist){
    for (int i=0; i<40; i++)
        for (int j=0; j<40; j++)
            view[i][j] = '-';
    
    int vx = jogo->P.pos.x - dist;
    int vy = jogo->P.pos.y - dist;
    int maxx = vx + (dist * 2) + 1;
    int maxy = vy + (dist * 2) + 1;
    maxx = (maxx < 40 ? maxx : 40);
    maxy = (maxy < 40 ? maxy : 40);
    
    for (int i=(vy >= 0 ? vy : 0); i<maxy; i++)
        for (int j=(vx >= 0 ? vx : 0); j<maxx; j++)
            view[i][j] = jogo->tabuleiro[i][j];
}

int write_board(char** a, char* saida){
    char c;
    FILE *fp = fopen(saida, "w");
    if (fp == NULL){
        perror("ERRO AO SOBRESCREVER TABULEIRO");
        return 1;
    }
    for (int i=0; i<40; i++){
        for (int j=0; j<40; j++){
            c = a[i][j];
            fputc(c, fp);
            fputc(';', fp);
        }
        fputs("\n", fp);
    }
    fclose(fp);
    return 0;
}
int write_save(char c, char n){
    FILE *fp = fopen("save.txt", "w");
    if (fp == NULL){
        perror("ERRO AO SOBRESCREVER SAVE");
        return 1;
    }
    fputc(c, fp);
    fputc(n, fp);
    fputc('\0', fp);
    fclose(fp);
    return 0;
}
char load_save_c(){
    FILE *fp = fopen("save.txt", "r");
    if (fp == NULL){
        perror("ERRO AO ABRIR SAVE");
        return '-';
    }
    char c = fgetc(fp);
    fclose(fp);
    return c;
}
int load_save_n(){
    FILE *fp = fopen("save.txt", "r");
    if (fp == NULL){
        perror("ERRO AO ABRIR SAVE");
        return '-';
    }
    fgetc(fp);
    char c = fgetc(fp);
    fclose(fp);
    return (int)c;
}

int eh_jogavel( type_board* jogo){
    int total = 0;
    for( int i = 0; i < 40; i++)
        for( int j = 0; j < 40; j++)
            /* para todas as casas, 1. checa se tabuleiro eh jogavel */
            if( jogo->tabuleiro[i][j] != 'X')
                total++;
    if( total <= 11)
        return 0; /* nao eh jogavel */
    return 1;
}

/* OBS. garantir que tabuleiro eh jogavel antes de chamar */
void randomiza_board( type_board* jogo){
    int numper = 11; /* 6 pastilhas + 4 fantasmas + pacman */
    int x, y;

    while( numper > 0){
        x = RANDOM(40);
        y = RANDOM(40);

        if( jogo->tabuleiro[x][y] == '0'){
            switch( numper){
                case 11: jogo->tabuleiro[x][y] = 'P'; break;
                case 10: jogo->tabuleiro[x][y] = 'Y'; break;
                case 9: jogo->tabuleiro[x][y] = 'G'; break;
                case 8: jogo->tabuleiro[x][y] = 'B'; break;
                case 7: jogo->tabuleiro[x][y] = 'R'; break;
                case 6: jogo->tabuleiro[x][y] = '6'; break;
                case 5: jogo->tabuleiro[x][y] = '5'; break;
                case 4: jogo->tabuleiro[x][y] = '4'; break;
                case 3: jogo->tabuleiro[x][y] = '3'; break;
                case 2: jogo->tabuleiro[x][y] = '2'; break;
                case 1: jogo->tabuleiro[x][y] = '1'; break;
            }
            numper--;
        }
    }
}
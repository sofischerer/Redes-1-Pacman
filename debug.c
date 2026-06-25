#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "server.h"
#include "client.h"

void print_jogo(type_board* jogo){
    print_view(jogo->tabuleiro);
    printf("%sP:%2d,%2d     R:%2d,%2d     B:%2d,%2d     G:%2d,%2d     Y:%2d,%2d\n", BLANK, jogo->P.pos.x, jogo->P.pos.y, jogo->R.pos.x, jogo->R.pos.y, jogo->B.pos.x, jogo->B.pos.y, jogo->G.pos.x, jogo->G.pos.y, jogo->Y.pos.x, jogo->Y.pos.y);
    printf("Items: [%2d,%2d;  %2d,%2d;  %2d,%2d;  %2d,%2d;  %2d,%2d;  %2d,%2d]\n", jogo->itens[0].pos.x, jogo->itens[0].pos.y, jogo->itens[1].pos.x, jogo->itens[1].pos.y, jogo->itens[2].pos.x, jogo->itens[2].pos.y, jogo->itens[3].pos.x, jogo->itens[3].pos.y, jogo->itens[4].pos.x, jogo->itens[4].pos.y, jogo->itens[5].pos.x, jogo->itens[5].pos.y);
}
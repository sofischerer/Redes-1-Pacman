#ifndef SERVER_H
#define SERVER_H

#define RANDOM(n) (rand() % n)
#define HORARIO(dir) ((dir + 1) % 4)
#define ANTIHORARIO(dir) (((dir + 3) % 4) + 4)
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define HIT_WALL 2
#define HIT_R 8
#define HIT_B 9
#define HIT_G 10
#define HIT_Y 11
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

typedef struct posicao{
    int x;
    int y;
}posicao;

typedef struct personagem{
    posicao pos;
    int dir;
}personagem;

typedef struct pastilha{
    posicao pos;
    int status;
}pastilha;

typedef struct type_board{
    personagem P;  //pacman
    personagem R;  //fantasma vermelho
    personagem B;  //fantasma azul
    personagem G;  //fantasma verde
    personagem Y;  //fantasma amarelo
    pastilha itens[6]; //itens
    char** tabuleiro;
}type_board;

void criar_jogo(FILE* entrada, type_board* jogo);
void destruir_jogo(type_board* jogo);
void anda_interno(type_board* jogo, personagem* chara, char c);
void vira_fan(personagem* chara, char c);
int move_pac(type_board* jogo, int key);
int move_fan(type_board* jogo, personagem* chara, char c);
int pegou_tudo(type_board* jogo);
void print_jogo(type_board* jogo);
int write_board(type_board* jogo);

#endif

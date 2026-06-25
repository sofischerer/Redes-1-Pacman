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

int mygetch ( void );
void print_pause();
void print_win();
void print_gameover();
void print_load(char c);
void print_view(char** view);
void read_board(char* nomearquivo, char** saida);

#endif

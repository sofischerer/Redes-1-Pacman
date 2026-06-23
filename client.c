#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "files.h"
#include "color.h"
#include "client.h"

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
void print_pause(){
    for( int i = 0; i < 20; i++)
        printf("\n");
    printf("                            JOGO PAUSADO (pressione P)\n");
    for( int i = 0; i < 21; i++)
        printf("\n");
}
void print_win(){
    for( int i = 0; i < 20; i++)
        printf("\n");
    printf("                            PARABÉNS! VOCÊ GANHOU\n");
    for( int i = 0; i < 21; i++)
        printf("\n");
}
void print_gameover(){
    for( int i = 0; i < 20; i++)
        printf("\n");
    printf("                                  GAME OVER\n");
    for( int i = 0; i < 21; i++)
        printf("\n");
} 
void print_load(char c){
    for( int i = 0; i < 20; i++)
        printf("\n");
    printf("                            TRANSFERINDO ARQUIVO %c...\n", c);
    for( int i = 0; i < 21; i++)
        printf("\n");
}
void print_view(char** view){
    char c;
    for (int i=0; i<40; i++){
        for (int j=0; j<40; j++){
            c = view[i][j];
            switch (c) {
                case '-':
                    printf("  ");
                    break;
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
}
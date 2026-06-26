#include <stdint.h>

#ifndef TESOURA_H
#define TESOURA_H
#define TAM_PEDACO (31 * sizeof(char))

#define ACK 0
#define NACK 1
#define VISUALIZACAO 2
#define INICIALIZACAO 3
#define DADOS 4
#define TXT 5
#define JPG 6
#define MP4 7
#define ARQ 8
#define MOVE 9
#define DIREITA 10
#define ESQUERDA 11
#define CIMA 12
#define BAIXO 13
#define ERROS 15
#define END 15
#define FIM 16

#define COD_TIMEOUT -1
#define COD_ARQ1 21
#define COD_ARQ2 22
#define COD_ARQ3 23
#define COD_ARQ4 24
#define COD_ARQ5 25
#define COD_ARQ6 26
#define COD_ARQ7 27
#define COD_ARQ8 28
#define COD_ARQ9 29
#define COD_ARQ10 30

unsigned long long int file_size(char *name);
/* modificar esses daqui em diante */
int envia_instrucao(uint8_t instr, void* obj, int sock, char* nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori, int tipo);
int recebe_instrucao(char* saida, int sock, char* nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori);
int envia_arquivo( char* entrada, int sock, char* nome_rede, uint8_t* MAC_dest, uint8_t* MAC_ori);
int recebe_arquivo(char* saida, int sock, char* nome_rede, uint8_t* MAC_dest, uint8_t* MAC_ori);
void read_board(char* nomearquivo, char** saida);

#endif
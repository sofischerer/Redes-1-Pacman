#ifndef TESOURA_H
#define TESOURA_H
#define TAM_PEDACO (31 * sizeof(char))

// /* ??? n sei se vai precisar */
// #define ACK 0x00
// #define NACK 0x01

// #define INSTR_TIMEOUT 0x02
// #define INSTR_END 0x03

// #define INSTR_NEWGAME 0x04

// #define INSTR_MANDA_BOARD 0x05
// #define INSTR_MANDA_ARQUIVO 0x06
// #define INSTR_MANDA_ASDW 0x07

// #define INSTR_MOVE_UP 0x0A
// #define INSTR_MOVE_RIGHT 0x0B
// #define INSTR_MOVE_DOWN 0x0C
// #define INSTR_MOVE_LEFT 0x0D
// #define INSTR_MOVE_QUIT 0x0E

// #define INSTR_ARQ_PAS_1 0x10
// #define INSTR_ARQ_PAS_2 0x11
// #define INSTR_ARQ_PAS_3 0x12
// #define INSTR_ARQ_PAS_4 0x13
// #define INSTR_ARQ_PAS_5 0x14
// #define INSTR_ARQ_PAS_6 0x15

// #define INSTR_ARQ_FAN_R 0x1A
// #define INSTR_ARQ_FAN_B 0x1B
// #define INSTR_ARQ_FAN_G 0x1C
// #define INSTR_ARQ_FAN_Y 0x1D

#define ACK 0
#define NACK 1
#define VISUALIZACAO 2
#define INICIALIZACAO 3
#define DADOS 4
#define TXT 5
#define JPG 6
#define MP4 7
#define DIREITA 10
#define ESQUERDA 11
#define CIMA 12
#define BAIXO 13
#define ERROS 15
#define END 15
#define FIM 16

#define COD_TIMEOUT -1

unsigned long long int file_size(char *name);
/* modificar esses daqui em diante */
int envia_instrucao(uint8_t instr, void* obj, int sock, char* nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori);
int recebe_instrucao(char* saida, int sock, char* nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori);
int envia_arquivo( char* entrada, int sock, char* nome_rede, uint8_t* MAC_dest, uint8_t* MAC_ori);
int recebe_arquivo(char* saida, int sock, char* nome_rede, uint8_t* MAC_dest, uint8_t* MAC_ori);
void read_board(char* nomearquivo, char** saida);

#endif
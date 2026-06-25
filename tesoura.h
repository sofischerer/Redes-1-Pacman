#ifndef TESOURA_H
#define TESOURA_H
#define TAM_PEDACO (31 * sizeof(char))

/* ??? n sei se vai precisar */
#define ACK 0x00
#define NACK 0x01

#define INSTR_TIMEOUT 0x02
#define INSTR_END 0x03

#define INSTR_NEWGAME 0x04

#define INSTR_MANDA_BOARD 0x05
#define INSTR_MANDA_ARQUIVO 0x06
#define INSTR_MANDA_ASDW 0x07

#define INSTR_MOVE_UP 0x0A
#define INSTR_MOVE_RIGHT 0x0B
#define INSTR_MOVE_DOWN 0x0C
#define INSTR_MOVE_LEFT 0x0D
#define INSTR_MOVE_QUIT 0x0E

#define INSTR_ARQ_PAS_1 0x10
#define INSTR_ARQ_PAS_2 0x11
#define INSTR_ARQ_PAS_3 0x12
#define INSTR_ARQ_PAS_4 0x13
#define INSTR_ARQ_PAS_5 0x14
#define INSTR_ARQ_PAS_6 0x15

#define INSTR_ARQ_FAN_R 0x1A
#define INSTR_ARQ_FAN_B 0x1B
#define INSTR_ARQ_FAN_G 0x1C
#define INSTR_ARQ_FAN_Y 0x1D



unsigned long long int file_size(char *name);
/* modificar esses daqui em diante */
uint8_t envia_instrucao( uint8_t instr);
uint8_t recebe_instrucao( );
int envia_arquivo( char* entrada, char* mudar);
int recebe_arquivo( char* mudar, char* saida);
void read_board(char* nomearquivo, char** saida);

#endif
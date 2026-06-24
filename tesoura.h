#ifndef TESOURA_H
#define TESOURA_H
#define TAM_PEDACO (31 * sizeof(char))

unsigned long long int file_size(char *name);
int envia_arquivo( char* entrada, char* saida);
int recebe_arquivo( char* entrada, char* saida);

#endif
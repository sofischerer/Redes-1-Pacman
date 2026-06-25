#include <stdint.h>

int cria_raw_socket(char* nome_interface_rede);

void envia_pacote(int sock, char* nome_rede, uint8_t* dados, int tamanho, int tipo, int sequencia, uint8_t* MAC_dest, uint8_t* MAC_ori);

int recebe_pacote(int sock, uint8_t* dados, int* tamanho, int* sequencia, int* tipo);

int transmissao(uint8_t **dados, int *tamanhos, int seq_inicial, int *tipos, int sock, char * nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori, int tam_janela);

void receber(uint8_t **dados, int *tamanhos, int *tipos, int sock, char * nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori);

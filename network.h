#include <stdint.h>

int cria_raw_socket(char* nome_interface_rede);

void recebe_pacote(int sock, uint8_t* mensagem);

void envia_pacote(int sock, char* nome_rede, uint8_t* dados, int tamanho, int tipo, int sequencia);
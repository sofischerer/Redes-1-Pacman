int cria_raw_socket(char* nome_interface_rede);

int recebe_pacote(int socket);

int enviar_pacote(int socket, char* buffer, int size);
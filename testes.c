#include <stdio.h>
#include "network.h"

int main(int argc, char *argv[]){
    if (argc == 1){
        printf("Erro, forma de chamar './programa 0' para servidor, './programa 1' para client\n");
        return 0;
    }

    int sock = cria_raw_socket("lo");

    if (argv[1][0] == '0'){
        envia_pacote(sock, "lo", NULL, 0, 0, 0);
    }
    else{
        while(1){
            recebe_pacote(sock, NULL);
        }
    }
    
}
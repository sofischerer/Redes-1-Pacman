#include <stdio.h>
#include <string.h>      // Necessário para strncpy e memcpy
#include <sys/ioctl.h>   // Necessário para ioctl
#include <net/if.h>      // Necessário para struct ifreq
#include <unistd.h>      // Necessário para close() se precisar
#include "network.h"
#include <linux/if_packet.h>

int main(int argc, char *argv[]){
    if (argc == 1){
        printf("Erro, forma de chamar './programa 0' para servidor, './programa 1' para client\n");
        return 0;
    }

    char *interface = "lo";
    int sock = cria_raw_socket(interface);
    
    uint8_t bcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    uint8_t meu_mac[6]; // Array para guardar o MAC da minha máquina

    // --- PARTE NOVA: Pegando o MAC da máquina ---
    struct ifreq ifr;
    // Copia o nome da interface para a estrutura de requisição
    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    
    // Faz a requisição ao kernel usando o próprio socket que você criou
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        perror("Erro ao obter o MAC address");
        return 1;
    }
    
    // Copia os 6 bytes do MAC retornado para a nossa variável meu_mac
    memcpy(meu_mac, ifr.ifr_hwaddr.sa_data, 6);

    // Mostra o MAC obtido no terminal para conferência
    printf("MAC obtido (%s): %02x:%02x:%02x:%02x:%02x:%02x\n", interface,
           meu_mac[0], meu_mac[1], meu_mac[2], meu_mac[3], meu_mac[4], meu_mac[5]);
    // --------------------------------------------

    if (argv[1][0] == '0'){
        // Agora você pode passar 'meu_mac' como parâmetro se a sua função envia_pacote aceitar
        envia_pacote(sock, interface, NULL, 0, 0, 0, bcast, meu_mac);
    }
    else{
        while(1){
            recebe_pacote(sock, NULL);
        }
    }
    
    return 0;
}
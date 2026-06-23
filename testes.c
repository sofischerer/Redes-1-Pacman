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

    int tamanho, sequencia, tipo;

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
        // uint8_t dados[5] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01};
        uint8_t dados[31] = {
            0x01, 0x02, 0x03, 0x04, 0x05,
            0x06, 0x07, 0x08, 0x09, 0x0A,
            0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x10, 0x11, 0x12, 0x13, 0x14,
            0x15, 0x16, 0x17, 0x18, 0x19,
            0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
            0x1F
        };
        tamanho = 31;
        tipo = 15;
        sequencia = 31;

        // Agora você pode passar 'meu_mac' como parâmetro se a sua função envia_pacote aceitar
        envia_pacote(sock, interface, dados, tamanho, tipo, sequencia, bcast, meu_mac);
    }
    else{
        uint8_t dados[31] = {0};
        while(1){
            recebe_pacote(sock, dados, &tamanho, &sequencia, &tipo);
        }
    }
    
    return 0;
}
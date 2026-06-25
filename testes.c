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
    uint8_t mac_dest[6];
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
        uint8_t matriz[5][31] = {0};
        int tamanhos[5] = {31, 5, 2, 1, 15};
        int tipos[5] = {15};
        int sequencias[5] = {31};
        int tam_janela = 5;

        // Agora você pode passar 'meu_mac' como parâmetro se a sua função envia_pacote aceitar
        int wa = transmissao((uint8_t**)matriz, tamanhos, 0, tipos, sock, interface, bcast, meu_mac, tam_janela);
        printf("%d\n", wa);
    }
    else{
        uint8_t dados[31] = {0};
        while(1){
            recebe_pacote(sock, dados, &tamanho, &sequencia, &tipo);
        }
    }
    
    return 0;
}
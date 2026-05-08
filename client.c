#include "network.h"
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

int main(){
    char* mensagem = "Teste";
    int sock = cria_raw_socket("lo");


    int ifindex = if_nametoindex("lo");
    struct sockaddr_ll pacote = {0};
    pacote.sll_family = AF_PACKET;
    pacote.sll_protocol = htons(ETH_P_ALL);
    pacote.sll_ifindex = ifindex;

    int x = connect(sock, (struct sockaddr*) &pacote, sizeof(pacote));
    if (x == -1) printf("Falha no connect\n");
    perror("connect");

    int teste = send(sock, mensagem, 5, 0);
    if (teste == -1) printf("Falha no send\n");
    perror("send");

    return teste;
}
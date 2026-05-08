
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zlib.h>

//Definição do protocolo
//CRC é colocado manualmente depois
struct __attribute__((packed)) protocolo{
    uint8_t inicio; //Marcador de inicio - 01111110
    uint16_t tam_seq_tip; //Auto-explicativo
};


uint8_t calcular_crc(uint8_t *dados, size_t tamanho){
    uint8_t crc = 0;

    for(size_t i = 0; i < tamanho; i++){
        crc ^= dados[i];

        for(int j = 0; j < 8; j++){
            if(crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

int cria_sock(char* nome_rede){
    //Cria a socket
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1){
        fprintf(stderr, "Erro ao criar socket.\n");
        exit(-1);
    }

    //Faz bind na socket
    int ifindex = if_nametoindex(nome_rede);

    struct sockaddr_ll endereco = {0};
    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;

    if (bind(sock, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        fprintf(stderr, "Erro ao fazer bind no socket\n");
        exit(-1);
    }

    struct packet_mreq mr = {0};
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        fprintf(stderr, "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente.\n");
        exit(-1);
    }

    return sock;
}

void envia_pacote(int sock, char* nome_rede, uint8_t* mensagem, int tamanho, int tipo, int sequencia){
    //Cria o "envelope" da ethernet
    unsigned char buffer_eth[ETH_FRAME_LEN] = {0};

    //Pega a rede que será usada
    int ifindex = if_nametoindex(nome_rede);

    //Monta a estrutura do endereço do destino
    struct sockaddr_ll destino = {0};
    destino.sll_family   = AF_PACKET;
    destino.sll_ifindex  = ifindex;
    destino.sll_protocol = htons(ETH_P_ALL);
    destino.sll_halen    = ETH_ALEN;

    //Coloca MAC Adress como 0, preciso usar um mac válido no escopo do trabalho?
    memset(destino.sll_addr, 0, ETH_ALEN);

    struct ethhdr *eth = (struct ethhdr *) buffer_eth;

    //Coloca o MAC Adress como 0 tanto no destino quanto na origem
    memset(eth->h_dest, 0, ETH_ALEN);
    memset(eth->h_source, 0, ETH_ALEN);

    //ID do meu protocolo
    eth->h_proto = htons(0x8888);

    //Cria o header e o buffer máximo
    struct protocolo header;
    int header_size = sizeof(header);
    uint8_t buffer[35] = {0}; //3 bytes de header + max de 31 bytes de dados + 1 byte de CRC

    //Extrai informações para o header
    uint16_t campo = 0;         //Zera o campo para poder fazer um OR funcional
    campo |= (tamanho << 11);   //Coloca o tamanho bit a bit nos primeiros 5 bits
    campo |= (sequencia << 5);  //Coloca a sequencia bit a bit nos próximos 6 bits
    campo |= tipo;              //Coloca o tipo nos ultimos 5 bits

    //Preenche o header
    header.inicio = 0x7E;                           // 7E = 01111110. Só um byte, não precisa de htons
    header.tam_seq_tip = htons(campo);   //Usa htons para garantir que o formato está correto

    //Transfere o header e a mensagem para o buffer
    memcpy(buffer, &header, header_size);
    memcpy(buffer+header_size, mensagem, tamanho);

    //Calcula o CRC e coloca no buffer
    buffer[header_size+tamanho] = calcular_crc(buffer, header_size+tamanho);

    //Coloca o buffer no frame ETH
    memcpy(buffer_eth+sizeof(struct ethhdr), buffer, header_size+tamanho+1);
}

int main(){

}

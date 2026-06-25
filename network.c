#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/time.h>

#define TIMEOUT_LIMIT 16
#define TIMEOUT 1000

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

int cria_raw_socket(char* nome_interface_rede) {
    // Cria arquivo para o socket sem qualquer protocolo
    int soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        if (soquete == -1) {
            fprintf(stderr, "Erro ao criar socket: Verifique se você é root!\n");
        exit(-1);
    }
 
    int ifindex = if_nametoindex(nome_interface_rede);
 
    struct sockaddr_ll endereco = {0};
    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if (bind(soquete, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        fprintf(stderr, "Erro ao fazer bind no socket\n");
        exit(-1);
    }
 
    struct packet_mreq mr = {0};
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        fprintf(stderr, "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente.\n");
        exit(-1);
    }
 
    return soquete;
}

void envia_pacote(int sock, char* nome_rede, uint8_t* dados, int tamanho, int tipo, int sequencia, uint8_t* MAC_dest, uint8_t* MAC_ori){
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

    //Coloca MAC Adress na sock
    memcpy(destino.sll_addr, MAC_dest, ETH_ALEN);

    struct ethhdr *eth = (struct ethhdr *) buffer_eth;

    //Coloca o MAC Adress no destino na parte do eth
    memcpy(eth->h_dest, MAC_dest, ETH_ALEN);
    memcpy(eth->h_source, MAC_ori, ETH_ALEN);

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

    //Transfere o header e os dados para o buffer
    memcpy(buffer, &header, header_size);
    memcpy(buffer+header_size, dados, tamanho);

    //Calcula o CRC e coloca no buffer
    buffer[header_size+tamanho] = calcular_crc(buffer, header_size+tamanho);

    //Coloca o buffer no frame ETH
    memcpy(buffer_eth+sizeof(struct ethhdr), buffer, header_size+tamanho+1);

    //Envio direcionado, um pouco de overkill mas melhor deixar mais versátil
    int tamanho_total = sizeof(struct ethhdr) + header_size + tamanho + 1;
    sendto(sock, buffer_eth, tamanho_total, 0, (struct sockaddr*)&destino, sizeof(destino));

    //ADICIONAR PARA E ESPERA

    //ADICIONAR TIMEOUT
}


int recebe_pacote(int sock, uint8_t* dados, int* tamanho, int* sequencia, int* tipo){
    unsigned char frame[ETH_FRAME_LEN];
    
    //Usados para retirar a duplicação da loopback
    struct sockaddr_ll origem;
    socklen_t origem_len = sizeof(origem);

    int resposta = recvfrom(sock, frame, sizeof(frame), 0, (struct sockaddr *)&origem, &origem_len);

    // Testando para retirar pacotes duplicados
    if (origem.sll_pkttype == PACKET_OUTGOING) {
        return 1; 
    }
    //Verificando se é nosso protocolo, tá sendo útil para testar na lo
    struct ethhdr *eth = (struct ethhdr *) frame;
    if (ntohs(eth->h_proto) != 0x8888) {
        return 1;
    }
    //Destrinchando o frame eth
    uint8_t* payload = frame+sizeof(struct ethhdr);
    int payload_len = resposta-sizeof(struct ethhdr);
    struct protocolo* p = (struct protocolo*) payload;
    uint16_t cabecalho = ntohs(p->tam_seq_tip);

    //Tirando os dados do nosso protocolo
    *tamanho = (cabecalho >> 11) & 0x1F;
    *sequencia = (cabecalho >> 5) & 0x3F;
    *tipo = cabecalho & 0x1F;
    memcpy(dados, payload+sizeof(struct protocolo), *tamanho);
    
    //Verificação de CRC
    int crc = calcular_crc(payload, payload_len-1);
    if (crc != payload[payload_len - 1]) return -1;

    //Prints de teste pq o debugger do meu pc está explodindo por algum motivo
    /*
    printf("inicio = 0x%02X\n", p->inicio);
    printf("tam = %u\n", *tamanho);
    printf("seq = %u\n", *sequencia);
    printf("tip = %u\n", *tipo);
    printf("CRC recalculado: %d\n", crc);
    printf("CRC da mensagem: %d\n",payload[payload_len - 1]);
    for (int i = 0; i<(*tamanho); i++){
        printf("%d ", dados[i]);
    }
    printf("\n");
    // */

    return 0;

}

//Função besta para pegar o horario do momento em ms
long now_ms(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int transmissao(uint8_t **dados, int *tamanhos, int seq_inicial, int *tipos, int sock, char * nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori, int tam_janela){
    int seq = seq_inicial;
    int tentativas = 0;
    uint8_t buffer[31] = {0};
    int temp_tam, temp_seq, temp_tipo;
//goto pq a mari me influencia demais
timeout:
    tentativas++;
    seq = seq_inicial;
    for (int i = 0; i<tam_janela; i++){
        if (seq > 63) seq = 0;
        else seq++;
        envia_pacote(sock, nome_rede, dados[i], tamanhos[i], tipos[i], seq, MAC_dest, MAC_ori);
    }
    if (tentativas>=TIMEOUT_LIMIT) return -1;

    //Começa a contar o timeout
    long inicio = now_ms();
    while(1){
        int resposta = recebe_pacote(sock, buffer, &temp_tam, &temp_seq, &temp_tipo);
        
        //Verifica casos de erro
        if (now_ms() - inicio >= TIMEOUT) goto timeout;
        if (resposta == 1) continue;
        if (resposta == 0) break;
    }
    //Se tudo está certo, verifica se recebeu bonitinho
    if (temp_tipo == -1) return temp_seq;
    return ++seq;
}

int receber(uint8_t **dados, int *tamanhos, int seq_inicial, int *tipos, int sock, char * nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori){

}
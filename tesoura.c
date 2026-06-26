#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tesoura.h"
#include "network.h"
#define TAM_PEDACO (31 * sizeof(char))

unsigned long long int file_size(char *name){
    FILE *fp = fopen(name, "rb");

    unsigned long long int size = -1;
    if( !fp) return -1;

    fseek( fp, 0, SEEK_END);
    size = ftell( fp);
    fclose( fp);

    return size;
}

int envia_arquivo( char* entrada, int sock, char* nome_rede, uint8_t* MAC_dest, uint8_t* MAC_ori){
    FILE *fp;
    char buffer[TAM_PEDACO];
    
    /* TRASH */
    // FILE* fp2;
    // fp2 = fopen(mudar, "wb");
    // if( !fp2) return -1;
    /* ----- */

    fp = fopen(entrada, "rb");
    if( !fp) return -1;

    unsigned long long int max = file_size(entrada) / TAM_PEDACO;
    uint8_t **janela = calloc(5, sizeof(uint8_t*));
    for (int i=0;i<5;i++) janela[i]=calloc(31, sizeof(uint8_t));
    int tamanhos[5] = {0};
    int seq = 0;
    int status = 0;
    int tipos[5] = {0};
    int j=0;
    for (unsigned long long int i = 0; i < max; i++){
        if( fread( buffer, sizeof( char), TAM_PEDACO, fp)){
            if (j<5){
                for (int k=0; k<TAM_PEDACO; k++) janela[j][k] = buffer[k];
                tamanhos[j] = TAM_PEDACO;
                tipos[j] = 4;
                j++;
            }
            if (j>=5){
                status = transmissao(janela, tamanhos, &seq, tipos, sock, nome_rede, MAC_dest, MAC_ori, 5);
                if (status == -1) return -1;
                j=0;
            }
            // fwrite( buffer, sizeof( char), TAM_PEDACO, fp2);/* trocar por enviar o buffer*/
        
            /* se der timeout retorna -1 */
            /* fazer */
        }
    }
    /* pode ser que o ultimo pedaco seja menor que o TAM_PEDACO */
    if( max % TAM_PEDACO != 0){
        memset( buffer, 0, sizeof(buffer));
        fread( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp);
        for (int k=0; k<(max % TAM_PEDACO) + 1; k++) janela[j][k] = buffer[k];
        tamanhos[j] = (max % TAM_PEDACO) + 1;
        tipos[j] = 4;
        j++;

        // fwrite( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp2);;/* trocar por enviar o buffer. cuidado com o tamanho */
        
        /* se der timeout retorna -1 */
        /* fazer */

    }
    if (j != 0){
        status = transmissao(janela, tamanhos, &seq, tipos, sock, nome_rede, MAC_dest, MAC_ori, 5);
        if (status == -1) return -1;
    }

    /* TRASH */
    // fclose(fp2);
    /* ----- */

    fclose(fp);
    return 0;
}

int recebe_arquivo(char* saida, int sock, char* nome_rede, uint8_t* MAC_dest, uint8_t* MAC_ori){
    char buffer[TAM_PEDACO];
    FILE* fp;
    fp = fopen(saida, "wb");
    if( !fp) return -1;
    
    /* TRASH */
    // FILE *fp;
    // fp = fopen(mudar, "rb");
    // if( !fp) return -1;
    /* ----- */

    uint8_t **janela = calloc(5, sizeof(uint8_t*));
    for (int i=0;i<5;i++) janela[i]=calloc(31, sizeof(uint8_t));
    int tamanhos[5] = {0};
    int seq = 0;
    int status = 0;
    int tipos[5] = {0};


    receber(janela, tamanhos, tipos, sock, nome_rede, MAC_dest, MAC_ori, 1);
    if (tipos[0] != 3) return -1;

    while(1){
        receber(janela, tamanhos, tipos, sock, nome_rede, MAC_dest, MAC_ori, 5);
        for (int i=0; i<5;i++){
            if (tipos[i] == 0) break;
            fwrite(janela[i], sizeof(uint8_t), tamanhos[i], fp);
        }
    }

    // unsigned long long int max = file_size(mudar) / TAM_PEDACO;
    // for (unsigned long long int i = 0; i < max; i+=5){
    //     if( fread( buffer, sizeof( char), TAM_PEDACO, fp)){

    //         fwrite( buffer, sizeof( char), TAM_PEDACO, fp);/* trocar por enviar o buffer*/
        
    //         /* se der timeout retorna -1 */
    //         /* fazer */
    //     }
    // }
    // /* pode ser que o ultimo pedaco seja menor que o TAM_PEDACO */
    // if( max % TAM_PEDACO != 0){
    //     memset( buffer, 0, sizeof(buffer));
    //     fread( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp);

    //     fwrite( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp);;/* trocar por enviar o buffer. cuidado com o tamanho */
        
    //     /* se der timeout retorna -1 */
    //     /* fazer */

    // }


    fclose(fp);


    // fclose(fp);
    return 0;
}

int envia_instrucao(uint8_t instr, void* obj, int sock, char* nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori, int tipo){
    int tamanhos[5] = {0};
    int tipos[5] = {0};
    int seq = 0;
    int n=0;
    uint8_t** buffer = calloc(5, sizeof(uint8_t*));
    for (int i=0; i<5; i++) buffer[i] = calloc(31, sizeof(uint8_t));

    //Inicia comunicação
    tipos[0] = instr;

    //Achei que ia precisar tratar caso a caso mas é tudo igual, por isso comecei com switch case e acabei com um if grande, preguiça de mudar, funciona
    switch (instr) {
        case 2: //Visualizacao
        case 3: //Inicialização
            if (envia_arquivo(obj, sock, nome_rede, MAC_dest, MAC_ori) == COD_TIMEOUT) return COD_TIMEOUT;
        case 5: //TXT
        case 6: //JPG
        case 7: //MP4
        buffer[0][0]=tipo;
            if (transmissao(buffer, tamanhos, &seq, tipos, sock, nome_rede, MAC_dest, MAC_ori, 1) == -1) return COD_TIMEOUT;
            //Manda arquivo
            if (envia_arquivo(obj, sock, nome_rede, MAC_dest, MAC_ori) == COD_TIMEOUT) return COD_TIMEOUT;
            break;
        default:
            if (transmissao(NULL, tamanhos, &seq, tipos, sock, nome_rede, MAC_dest, MAC_ori, 1) == -1) return COD_TIMEOUT;
            break;
    }
}
int recebe_instrucao(char* saida, int sock, char* nome_rede, uint8_t *MAC_dest, uint8_t *MAC_ori){
    int tamanhos[5] = {0};
    int tipos[5] = {0};
    int seq = 0;
    uint8_t** buffer = calloc(5, sizeof(uint8_t*));
    for (int i=0; i<5; i++) buffer[i] = calloc(31, sizeof(uint8_t));

    //Inicia comunicação
    receber(buffer, tamanhos, tipos, sock, nome_rede, MAC_dest, MAC_ori, 1);
    if(tipos[0]>3 && tipos[0]<8) return buffer[0][0];
    //Transferencia de dados caso necessario
    if (tipos[0] == 4){
        if (recebe_arquivo(saida, sock, nome_rede, MAC_dest, MAC_ori) == COD_TIMEOUT) return COD_TIMEOUT;
    }
    else{
        return tipos[0];
    }
}
void read_board(char* nomearquivo, char** saida){
    FILE *fp = fopen(nomearquivo, "r");
    char linha[100];
    for (int i=0; i<40; i++){
        fgets(linha, 100, fp);
        for (int j=0; j<40; j++)
            saida[i][j] = linha[2*j];
    }
    fclose(fp);
}
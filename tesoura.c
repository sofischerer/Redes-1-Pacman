#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tesoura.h"
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

int envia_arquivo( char* entrada, char* mudar){
    FILE *fp;
    char buffer[TAM_PEDACO];
    
    /* TRASH */
    FILE* fp2;
    fp2 = fopen(mudar, "wb");
    if( !fp2) return -1;
    /* ----- */

    fp = fopen(entrada, "rb");
    if( !fp) return -1;

    unsigned long long int max = file_size(entrada) / TAM_PEDACO;
    for (unsigned long long int i = 0; i < max; i++){
        if( fread( buffer, sizeof( char), TAM_PEDACO, fp)){

            fwrite( buffer, sizeof( char), TAM_PEDACO, fp2);/* trocar por enviar o buffer*/
        
            /* se der timeout retorna -1 */
            /* fazer */
        }
    }
    /* pode ser que o ultimo pedaco seja menor que o TAM_PEDACO */
    if( max % TAM_PEDACO != 0){
        memset( buffer, 0, sizeof(buffer));
        fread( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp);

        fwrite( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp2);;/* trocar por enviar o buffer. cuidado com o tamanho */
        
        /* se der timeout retorna -1 */
        /* fazer */

    }

    /* TRASH */
    fclose(fp2);
    /* ----- */

    fclose(fp);
    return 0;
}

int recebe_arquivo( char* mudar, char* saida){
    char buffer[TAM_PEDACO];
    FILE* fp2;
    fp2 = fopen(saida, "wb");
    if( !fp2) return -1;
    
    /* TRASH */
    FILE *fp;
    fp = fopen(mudar, "rb");
    if( !fp) return -1;
    /* ----- */

    unsigned long long int max = file_size(mudar) / TAM_PEDACO;
    for (unsigned long long int i = 0; i < max; i++){
        if( fread( buffer, sizeof( char), TAM_PEDACO, fp)){

            fwrite( buffer, sizeof( char), TAM_PEDACO, fp2);/* trocar por enviar o buffer*/
        
            /* se der timeout retorna -1 */
            /* fazer */
        }
    }
    /* pode ser que o ultimo pedaco seja menor que o TAM_PEDACO */
    if( max % TAM_PEDACO != 0){
        memset( buffer, 0, sizeof(buffer));
        fread( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp);

        fwrite( buffer, sizeof( char), (max % TAM_PEDACO) + 1, fp2);;/* trocar por enviar o buffer. cuidado com o tamanho */
        
        /* se der timeout retorna -1 */
        /* fazer */

    }


    /* TRASH */
    fclose(fp2);
    /* ----- */


    fclose(fp);
    return 0;
}

uint8_t envia_instrucao( uint8_t instr){
    return 0x00;
}
uint8_t recebe_instrucao( ){
    return 0x00;
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
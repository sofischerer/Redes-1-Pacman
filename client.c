#include <stdio.h>
#include <string.h>

void preencher_tabuleiro(FILE* entrada, char** matriz){

    if (entrada == NULL){

    }

    char linha[100];
    for (int i=0; i<40; i++){
        fgets(linha, 100, entrada);
        for (int j=0; j<40; j++){
            matriz[i][j] = linha[2*j];
        }        
    }

}
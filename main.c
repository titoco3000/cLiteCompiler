#include <stdio.h>

#include "lexico.h"
int main(int argc, char *argv[]){
    FILE *entrada;

    if (argc > 1)
        entrada = fopen(argv[1], "r");
    else
        entrada = fopen("exemplos/ex1.clite", "r");

    if(entrada){
        Token* tokens = analiseLexica(entrada);

        printTokenList(tokens);
        
        liberarTokens(tokens);
    }
    else{
        printf("Falha ao abrir o arquivo\n");
    }
}
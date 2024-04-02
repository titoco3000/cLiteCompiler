/*
Compiladores
Trabalho 1: Analisador Léxico e Sintático

Henrique Lee Amorim - R.A. 10389994 
Tito Guidotti - R.A. 10388629

Abril de 2024
*/
#include <stdio.h>

#include "lexico.h"
#include "sintatico.h"

int main(int argc, char *argv[]){
    FILE *entrada;

    //se foi passado arquivo como arg, abre ele
    if (argc > 1)
        entrada = fopen(argv[1], "r");
    //se não, abre primeiro exemplo
    else{
        entrada = fopen("exemplos/ex2.clite", "r");
        //se ainda não conseguiu, tenta procurar uma pasta para cima (util dependendo do editor)
        if(!entrada)
            entrada = fopen("../exemplos/ex2.clite", "r");
    }

    //se conseguiu abrir
    if(entrada){
        //cria lista de tokens
        Token* tokens = analiseLexica(entrada);

        //printTokenList(tokens);
        analiseSintatica(tokens);
        
        //libera memória
        liberarTokens(tokens);
    }
    else{
        printf("Falha ao abrir o arquivo\n");
    }
}
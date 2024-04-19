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

/*
Retorna 1 se conseguiu abrir, 0 se não
*/
int analisarArquivo(char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    // se conseguiu abrir
    if (arquivo)
    {
        // cria lista de tokens
        Token *tokens = analiseLexica(arquivo);

        analiseSintatica(tokens);

        // libera memória
        liberarTokens(tokens);
        fclose(arquivo);

        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{

    // se não foram passados arquivos como arg, abre exemplo
    if (argc == 1)
    {
        if (!analisarArquivo("exemplos/ex1.clite"))
        {
            // se ainda não conseguiu, tenta procurar uma pasta para cima (util dependendo do editor)
            if (!analisarArquivo("../exemplos/ex1.clite"))
                printf("Falha ao abrir o exemplo\n");
        }
    }
    else
        for (int i = 1; i < argc; i++)
        {
            printf("Abrindo %s\n\n", argv[i]);
            if (!analisarArquivo(argv[i]))
                printf("Falha ao abrir o arquivo %s\n", argv[i]);
            printf("\n");
        }
}
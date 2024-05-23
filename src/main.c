/*
Compiladores
Trabalho 2: Analisador Léxico, Sintático, Semântico e Geração de Código MEPA

Henrique Lee Amorim - R.A. 10389994
Tito Guidotti - R.A. 10388629

Maio de 2024
*/
#include <stdio.h>

#include "lexico.h"
#include "sintatico.h"
#include "file_helper.h"
#include "semantico.h"

typedef enum
{
    ANALISE_LEXICA,
    ANALISE_SINTATICA,
    ANALISE_SEMANTICA
} TipoDeAnalise;

TipoDeAnalise tipoDeAnaliseAtual = ANALISE_SINTATICA;

/*
Retorna 1 se conseguiu abrir, 0 se não
*/
int analisarArquivo(char *nomeArquivo, TipoDeAnalise tipoDeAnalise)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    int linha = 1;
    // se conseguiu abrir
    if (arquivo)
    {
        if (tipoDeAnalise == ANALISE_LEXICA)
        {
            Lexico lex = criar_lex(arquivo);
            while (lex.token.tipo != TOKEN_EOF && lex.token.tipo != TOKEN_ERRO)
            {
                printToken(&lex.token);
                consumir_token(&lex);
            }
            if (lex.token.tipo == TOKEN_ERRO)
            {
                printToken(&lex.token);
            }
            else
            {
                printf("Analise lexica concluida com sucesso\n");
            }
        }
        else if (tipoDeAnalise == ANALISE_SINTATICA)
        {
            TabelaDeSimbolos tabelaDeSimbolos = CriarTabelaDeSimbolos();
            programa(arquivo, &tabelaDeSimbolos);
            LiberarTabelaDeSimbolos(tabelaDeSimbolos);
        }

        return 1;
    }
    return 0;
}

/*
Analisa arquivo, imprimindo info antes e depois e lidando com o caso
de falha ao abrir
*/
void atuarEmArquivo(char *addr)
{
    printf("Abrindo %s\n\n", addr);
    if (!analisarArquivo(addr, tipoDeAnaliseAtual))
        printf("Falha ao abrir o arquivo %s\n", addr);
    printf("\n");
}

/*
Recebe lista de arquivos e/ou pastas para compilar.
Se for pasta, entra recursivamente em cada subpasta e, se for arquivo,
compila e imprime resultados (código MEPA e/ou erros)
*/
int main(int argc, char *argv[])
{

    // se não foram passados arquivos como arg, abre exemplo
    if (argc == 1)
    {
        if (!analisarArquivo("exemplos/ex4.clite", tipoDeAnaliseAtual))
        {
            // se ainda não conseguiu, tenta procurar uma pasta para cima (util dependendo do editor)
            if (!analisarArquivo("../exemplos/ex4.clite", tipoDeAnaliseAtual))
                printf("Falha ao abrir o exemplo\n");
        }
    }
    else
        for (int i = 1; i < argc; i++)
        {
            if (is_directory(argv[i]))
                applyFunctionToAllFilesRecursive(argv[i], atuarEmArquivo);
            else
                atuarEmArquivo(argv[i]);
        }
}
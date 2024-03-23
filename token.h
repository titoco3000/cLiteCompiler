#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum
{
    // unica letra
    TOKEN_ABRE_PARENTESES,
    TOKEN_FECHA_PARENTESES,
    TOKEN_ABRE_CHAVES,
    TOKEN_FECHA_CHAVES,
    TOKEN_VIRGULA,
    TOKEN_PONTO_E_VIRGULA,
    // somente nao-alfanumeriocos
    TOKEN_ATRIBUICAO,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_COMP_MENOR,
    TOKEN_COMP_MENOR_OU_IGUAL,
    TOKEN_COMP_MAIOR,
    TOKEN_COMP_MAIOR_OU_IGUAL,
    TOKEN_COMP_IGUAL,
    TOKEN_COMP_DIFERENTE,
    // operacoes
    TOKEN_OP_SOMA,
    TOKEN_OP_SUB,
    TOKEN_OP_MULT,
    TOKEN_OP_DIV,
    // customisaveis
    TOKEN_IDENTIFICADOR,
    TOKEN_NUMERO,
    // palavras-chave
    TOKEN_TIPO_INT,
    TOKEN_TIPO_BOOL,
    TOKEN_TIPO_VOID,
    TOKEN_MAIN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_SCANF,
    TOKEN_PRINTF,
    // marcadores
    TOKEN_EOF,
    TOKEN_ERRO
} TokenType;

typedef struct Token
{
    TokenType tipo;
    void *conteudo;
    int linha;
    struct Token *prox;
} Token;

Token *CriarToken(TokenType tipo, int linha)
{
    Token *t = (Token *)malloc(sizeof(Token));
    t->tipo = tipo;
    t->conteudo = 0;
    t->linha = linha;
    t->prox = 0;
    return t;
}

Token *CriarTokenIdentifier(char *str, int linha)
{
    char *copia = (char *)malloc(strlen(str) + 1);

    strcpy(copia, str);

    Token *t = CriarToken(TOKEN_IDENTIFICADOR, linha);
    t->conteudo = (void *)copia;
    return t;
}

Token *CriarTokenErro(char *str, int linha)
{
    char *copia = (char *)malloc(strlen(str) + 1);

    strcpy(copia, str);

    Token *t = CriarToken(TOKEN_ERRO, linha);
    t->conteudo = (void *)copia;
    return t;
}

Token *CriarTokenNumero(int numero, int linha)
{
    char *copia = (char *)malloc(sizeof(int));

    *copia = numero;

    Token *t = CriarToken(TOKEN_TIPO_INT, linha);
    t->conteudo = (void *)copia;
    return t;
}

void InserirTokenNaLista(Token **lista, Token *novo)
{
    if (*lista == 0)
        *lista = novo;
    else
    {
        Token *t = *lista;
        while (t->prox != 0)
            t = t->prox;
        t->prox = novo;
    }
}

void printTipoToken(Token *token)
{
    if (token)
    {
        switch (token->tipo)
        {
        case TOKEN_ABRE_PARENTESES:
            printf("abre_par");
            break;
        case TOKEN_FECHA_PARENTESES:
            printf("fecha_par");
            break;
        case TOKEN_ABRE_CHAVES:
            printf("abre_chaves");
            break;
        case TOKEN_FECHA_CHAVES:
            printf("fecha_chaves");
            break;
        case TOKEN_VIRGULA:
            printf("virgula");
            break;
        case TOKEN_PONTO_E_VIRGULA:
            printf("ponto_virgula");
            break;
        case TOKEN_ATRIBUICAO:
            printf("atribuicao");
            break;
        case TOKEN_OR:
            printf("or");
            break;
        case TOKEN_AND:
            printf("and");
            break;
        case TOKEN_COMP_MENOR:
            printf("menor");
            break;
        case TOKEN_COMP_MENOR_OU_IGUAL:
            printf("menor_igual");
            break;
        case TOKEN_COMP_MAIOR:
            printf("maior");
            break;
        case TOKEN_COMP_MAIOR_OU_IGUAL:
            printf("maior_igual");
            break;
        case TOKEN_COMP_IGUAL:
            printf("igual");
            break;
        case TOKEN_COMP_DIFERENTE:
            printf("diferente");
            break;
        case TOKEN_OP_SOMA:
            printf("soma");
            break;
        case TOKEN_OP_SUB:
            printf("subtracao");
            break;
        case TOKEN_OP_MULT:
            printf("multiplicacao");
            break;
        case TOKEN_OP_DIV:
            printf("divisao");
            break;
        case TOKEN_TIPO_BOOL:
            printf("bool");
            break;
        case TOKEN_TIPO_VOID:
            printf("void");
            break;
        case TOKEN_MAIN:
            printf("main");
            break;
        case TOKEN_IF:
            printf("if");
            break;
        case TOKEN_ELSE:
            printf("else");
            break;
        case TOKEN_WHILE:
            printf("while");
            break;
        case TOKEN_SCANF:
            printf("scanf");
            break;
        case TOKEN_PRINTF:
            printf("printf");
            break;
        case TOKEN_TIPO_INT:
            printf("int");
            break;
        case TOKEN_IDENTIFICADOR:
            printf("identificador");
            break;
        case TOKEN_NUMERO:
            printf("numero");
            break;
        case TOKEN_EOF:
            printf("eof");
            break;
        case TOKEN_ERRO:
            printf("erro");
            break;
        default:
            printf("Erro de programacao: faltou algum item no enum\n");
            exit(1);
            break;
        }
        token = token->prox;
    }
    else
        printf("NULL\n");
}

void printTokenList(Token *lista)
{
    while (lista)
    {
        printf("#  %d:", lista->linha);
        printTipoToken(lista);
        switch (lista->tipo)
        {
        case TOKEN_NUMERO:
            printf(" (%d)", *((int *)(lista->conteudo)));
            break;
        case TOKEN_IDENTIFICADOR:
        case TOKEN_ERRO:
            printf(" (%s)", ((char *)(lista->conteudo)));
            break;
        }
        printf("\n");
        lista = lista->prox;
    }

}

void liberarTokens(Token *token)
{
    if (token)
    {
        if (token->conteudo)
            free(token->conteudo);
        free(token->prox);
        free(token);
    }
}
#endif
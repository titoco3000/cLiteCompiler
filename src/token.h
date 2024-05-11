#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//tipos possíveis de token
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
    TOKEN_COMENTARIO,
    // palavras-chave
    TOKEN_TIPO_INT,
    TOKEN_TIPO_BOOL,
    TOKEN_TIPO_VOID,
    TOKEN_BOOL_TRUE,
    TOKEN_BOOL_FALSE,
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

//struct para armazenar tipo de token e informações relacionadas
typedef struct Token
{
    TokenType tipo;
    void *conteudo; //Token pode ou não ter conteudo. É um void* para apontar para um bloco no heap de qualquer tipo
    int linha;
} Token;

//Construtores

Token CriarToken(TokenType tipo, int linha)
{
    //como é para ficar numa lista, sempre alocado do heap
    Token t;
    t.tipo = tipo;
    t.conteudo = NULL;
    t.linha = linha;
    return t;
}

// Cria token guardando o identifier
Token CriarTokenIdentifier(char *str, int linha)
{
    //copia para garantir que é o dono do bloco de memória no heap
    char *copia = (char *)malloc(strlen(str) + 1);
    strcpy(copia, str);

    Token t = CriarToken(TOKEN_IDENTIFICADOR, linha);
    t.conteudo = (void *)copia;
    return t;
}

// Cria token guardando uma mensagem de erro
Token CriarTokenErro(char *str, int linha)
{
    char *copia = (char *)malloc(strlen(str) + 1);

    strcpy(copia, str);

    Token t = CriarToken(TOKEN_ERRO, linha);
    t.conteudo = (void *)copia;
    return t;
}

// Cria token guardando o numero
Token CriarTokenNumero(int numero, int linha)
{
    int *copia = (int *)malloc(sizeof(int));

    *copia = numero;

    Token t = CriarToken(TOKEN_NUMERO, linha);
    t.conteudo = (void *)copia;
    return t;
}

Token CopiarToken(Token* original){
    if(original->tipo == TOKEN_IDENTIFICADOR)
        return CriarTokenIdentifier((char*)original->conteudo, original->linha);
    if(original->tipo == TOKEN_ERRO)
        return CriarTokenErro((char*)original->conteudo, original->linha);
    if(original->tipo == TOKEN_NUMERO)
        return CriarTokenNumero(*((int*)original->conteudo), original->linha);
    return CriarToken(original->tipo, original->linha);
    
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
        case TOKEN_BOOL_TRUE:
            printf("true");
            break;
        case TOKEN_BOOL_FALSE:
            printf("false");
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
        case TOKEN_COMENTARIO:
            printf("comentário");
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
    }
    else
        printf("NULL\n");
}

void printToken(Token *token){
    printf("#  %d:", token->linha);
    printTipoToken(token);
    switch (token->tipo)
    {
    case TOKEN_NUMERO:
        printf("\t\t| %d", *((int *)(token->conteudo)));
        break;
    case TOKEN_IDENTIFICADOR:
    case TOKEN_ERRO:
        printf("\t| %s", ((char *)(token->conteudo)));
        break;
    }
    printf("\n");
}

void liberarTokens(Token token)
{
    if (token.conteudo)
        free(token.conteudo);
}
#endif
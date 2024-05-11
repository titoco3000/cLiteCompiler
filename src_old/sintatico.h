#ifndef SINTATICO_H
#define SINTATICO_H

#include "token.h"

#include <ctype.h>
#include <string.h>
Token *lista;

static void proximo();
static void consomeToken(TokenType token);
static void programa(Token *tokens);
static void declaracoes();
static void comandos();
static void comando();
static void bloco_comandos();
static void comando_atribuicao();
static void comando_if();
static void comando_while();
static void comando_scanf();
static void comando_printf();
static void expressao();
static void expressao_logica();
static void expressao_relacional();
static void expressao_adicao();
static void expressao_multi();
static void expressao();
static void lista_variaveis();
static void operando();

/*
Recebe uma lista de tokens e imprime cada um até encontrar um erro léxico ou sintático.
Imprime resultado.
*/
void analiseSintatica(Token *tokens)
{
    programa(tokens);
}

static void proximo() { lista = lista->prox; }

static void consomeToken(TokenType esperado)
{
    if (lista->tipo == TOKEN_COMENTARIO && esperado != TOKEN_COMENTARIO)
    {
        printf("#  %d:\t", lista->linha);
        printTipoToken(lista);
        printf("\n");

        proximo();
    }

    if (lista->tipo == esperado)
    {
        if (lista->conteudo != NULL)
        {
            printf("#  %d:\t", lista->linha);
            printTipoToken(lista);

            if (lista->tipo == TOKEN_IDENTIFICADOR)
                printf(" | %s\n", ((char *)(lista->conteudo)));
            else if (lista->tipo == TOKEN_NUMERO)
                printf("        | %d\n", *((int *)(lista->conteudo)));
        }
        else
        {
            printf("#  %d:\t", lista->linha);
            printTipoToken(lista);
            printf("\n");
        }
        proximo();
    }
    else
    {
        if (lista->tipo == TOKEN_ERRO)
        {
            printf("#  %d:\tErro Lexico   | %s\n", lista->linha, ((char *)(lista->conteudo)));
            exit(1);
        }
        else
        {

            printf("#  %d Erro sintatico: esperado ", lista->linha);
            Token *a = CriarToken(esperado, 2);

            printTipoToken(a);
            free(a);
            printf(" encontrado ");
            printTipoToken(lista);
            printf("\n");
            exit(1);
        }
    }
}
static void eof()
{
    int linha = lista->linha;
    consomeToken(TOKEN_EOF);
    printf("..............\n%d linhas analisadas, programa sintaticamente correto\n", linha);
}
static void programa(Token *l)
{
    lista = l;
    consomeToken(TOKEN_TIPO_INT);
    consomeToken(TOKEN_MAIN);
    consomeToken(TOKEN_ABRE_PARENTESES);
    consomeToken(TOKEN_TIPO_VOID);
    consomeToken(TOKEN_FECHA_PARENTESES);
    consomeToken(TOKEN_ABRE_CHAVES);
    declaracoes();
    comandos();
    consomeToken(TOKEN_FECHA_CHAVES);
    eof();
}
static void declaracoes()
{
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);

    if (lista->tipo == TOKEN_TIPO_INT)
    {
        consomeToken(TOKEN_TIPO_INT);
        lista_variaveis();
        consomeToken(TOKEN_PONTO_E_VIRGULA);
        declaracoes();
    }
    else if (lista->tipo == TOKEN_TIPO_BOOL)
    {
        consomeToken(TOKEN_TIPO_BOOL);
        lista_variaveis();
        consomeToken(TOKEN_PONTO_E_VIRGULA);
        declaracoes();
    }
}

static void comandos()
{
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    while (
        lista->tipo == TOKEN_ABRE_CHAVES ||
        lista->tipo == TOKEN_IDENTIFICADOR || lista->tipo == TOKEN_IF ||
        lista->tipo == TOKEN_WHILE || lista->tipo == TOKEN_SCANF ||
        lista->tipo == TOKEN_PRINTF || lista->tipo == TOKEN_PONTO_E_VIRGULA)
    {
        comando();
    }
}

static void comando()
{
    switch (lista->tipo)
    {
    case TOKEN_ABRE_CHAVES:
        bloco_comandos();
        break;
    case TOKEN_IDENTIFICADOR:
        comando_atribuicao();
        break;
    case TOKEN_IF:
        comando_if();
        break;
    case TOKEN_WHILE:
        comando_while();
        break;
    case TOKEN_SCANF:
        comando_scanf();
        break;
    case TOKEN_PRINTF:
        comando_printf();
        break;
    case TOKEN_PONTO_E_VIRGULA:
        consomeToken(TOKEN_PONTO_E_VIRGULA);
        break;
    }
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
}
static void bloco_comandos(){
    consomeToken(TOKEN_ABRE_CHAVES);
    comandos();
    consomeToken(TOKEN_FECHA_CHAVES);
}
static void lista_variaveis()
{
    consomeToken(TOKEN_IDENTIFICADOR);
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    while (lista->tipo == TOKEN_VIRGULA)
    {
        consomeToken(TOKEN_VIRGULA);
        consomeToken(TOKEN_IDENTIFICADOR);
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
}

static void comando_atribuicao()
{
    consomeToken(TOKEN_IDENTIFICADOR);
    consomeToken(TOKEN_ATRIBUICAO);
    expressao();
    consomeToken(TOKEN_PONTO_E_VIRGULA);
}
static void expressao()
{
    expressao_logica();
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    while (lista->tipo == TOKEN_OR)
    {
        consomeToken(TOKEN_OR);
        expressao_logica();
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
}

static void expressao_logica()
{
    expressao_relacional();
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    while (lista->tipo == TOKEN_AND)
    {
        consomeToken(TOKEN_AND);
        expressao_relacional();
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
}
static void expressao_relacional()
{
    expressao_adicao();
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    switch (lista->tipo)
    {
    case TOKEN_COMP_MENOR:
        consomeToken(TOKEN_COMP_MENOR);
        expressao_adicao();
        break;
    case TOKEN_COMP_MENOR_OU_IGUAL:
        consomeToken(TOKEN_COMP_MENOR_OU_IGUAL);
        expressao_adicao();
        break;
    case TOKEN_COMP_MAIOR:
        consomeToken(TOKEN_COMP_MAIOR);
        expressao_adicao();
        break;
    case TOKEN_COMP_MAIOR_OU_IGUAL:
        consomeToken(TOKEN_COMP_MAIOR_OU_IGUAL);
        expressao_adicao();
        break;
    case TOKEN_COMP_IGUAL:
        consomeToken(TOKEN_COMP_IGUAL);
        expressao_adicao();
        break;
    case TOKEN_COMP_DIFERENTE:
        consomeToken(TOKEN_COMP_DIFERENTE);
        expressao_adicao();
        break;
    default:
        break;
    }
}
static void expressao_adicao()
{
    expressao_multi();
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    while (lista->tipo == TOKEN_OP_SOMA)
    {
        consomeToken(TOKEN_OP_SOMA);
        expressao_multi();
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
    while (lista->tipo == TOKEN_OP_SUB)
    {
        consomeToken(TOKEN_OP_SUB);
        expressao_multi();
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
}
static void expressao_multi()
{
    operando();
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    while (lista->tipo == TOKEN_OP_MULT)
    {
        consomeToken(TOKEN_OP_MULT);
        operando();
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
    while (lista->tipo == TOKEN_OP_DIV)
    {
        consomeToken(TOKEN_OP_DIV);
        operando();
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
}
static void operando()
{
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    if (lista->tipo == TOKEN_IDENTIFICADOR)
    {
        consomeToken(TOKEN_IDENTIFICADOR);
    }
    else if (lista->tipo == TOKEN_NUMERO)
    {
        consomeToken(TOKEN_NUMERO);
    }
    else if (lista->tipo == TOKEN_BOOL_TRUE)
    {
        consomeToken(TOKEN_BOOL_TRUE);
    }
    else if (lista->tipo == TOKEN_BOOL_FALSE)
    {
        consomeToken(TOKEN_BOOL_FALSE);
    }
    else
    {
        consomeToken(TOKEN_ABRE_PARENTESES);
        expressao();
        consomeToken(TOKEN_FECHA_PARENTESES);
    }
}

static void comando_if()
{
    consomeToken(TOKEN_IF);
    consomeToken(TOKEN_ABRE_PARENTESES);
    expressao();
    consomeToken(TOKEN_FECHA_PARENTESES);
    comandos();
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    if (lista->tipo == TOKEN_ELSE)
    {
        consomeToken(TOKEN_ELSE);
        comandos();
    }
}
static void comando_while()
{
    consomeToken(TOKEN_WHILE);
    consomeToken(TOKEN_ABRE_PARENTESES);
    expressao();
    consomeToken(TOKEN_FECHA_PARENTESES);
    comandos();
}

static void comando_scanf()
{
    consomeToken(TOKEN_SCANF);
    consomeToken(TOKEN_ABRE_PARENTESES);
    lista_variaveis();
    consomeToken(TOKEN_FECHA_PARENTESES);
    consomeToken(TOKEN_PONTO_E_VIRGULA);
}

static void comando_printf()
{
    consomeToken(TOKEN_PRINTF);
    consomeToken(TOKEN_ABRE_PARENTESES);
    expressao();
    while (lista->tipo == TOKEN_COMENTARIO)
        consomeToken(TOKEN_COMENTARIO);
    while (lista->tipo == TOKEN_VIRGULA)
    {
        consomeToken(TOKEN_VIRGULA);
        expressao();
        while (lista->tipo == TOKEN_COMENTARIO)
            consomeToken(TOKEN_COMENTARIO);
    }
    consomeToken(TOKEN_FECHA_PARENTESES);
    consomeToken(TOKEN_PONTO_E_VIRGULA);
}
#endif
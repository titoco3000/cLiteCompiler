#ifndef SINTATICO_H
#define SINTATICO_H

#include "token.h"
#include "lexico.h"

#include <ctype.h>
#include <string.h>

/*
Deve ser chamado antes de comparações
*/
static void obterProximoTokenQueNaoComentario(Lexico* lex){
    while (lex->token.tipo == TOKEN_COMENTARIO){
        printToken(&lex->token);
        consumir_token(lex);
    }
}

void consome_token_com_tipo(Lexico *lex, TokenType tipo){
    obterProximoTokenQueNaoComentario(lex);
    if(lex->token.tipo != tipo){
        printf("#  %d Erro sintatico: esperado ", lex->token.linha);
        Token esperado = CriarToken(tipo, lex->token.linha);

        printTipoToken(&esperado);
        printf(" encontrado ");
        printTipoToken(&lex->token);
        printf("\n");
        exit(1);
    }
    printToken(&lex->token);
    consumir_token(lex);
}

static void declaracoes(Lexico *lex);
static void comandos(Lexico *lex);
static void comando(Lexico *lex);
static void bloco_comandos(Lexico *lex);
static void comando_atribuicao(Lexico *lex);
static void comando_if(Lexico *lex);
static void comando_while(Lexico *lex);
static void comando_scanf(Lexico *lex);
static void comando_printf(Lexico *lex);
static void expressao(Lexico *lex);
static void expressao_logica(Lexico *lex);
static void expressao_relacional(Lexico *lex);
static void expressao_adicao(Lexico *lex);
static void expressao_multi(Lexico *lex);
static void expressao(Lexico *lex);
static void lista_variaveis(Lexico *lex);
static void operando(Lexico *lex);
static void eof(Lexico *lex);


void analiseSintatica(FILE* entrada){
    Lexico lex = criar_lex(entrada);
    consome_token_com_tipo(&lex, TOKEN_TIPO_INT);
    consome_token_com_tipo(&lex, TOKEN_MAIN);
    consome_token_com_tipo(&lex, TOKEN_ABRE_PARENTESES);
    consome_token_com_tipo(&lex, TOKEN_TIPO_VOID);
    consome_token_com_tipo(&lex, TOKEN_FECHA_PARENTESES);
    consome_token_com_tipo(&lex, TOKEN_ABRE_CHAVES);
    declaracoes(&lex);
    comandos(&lex);
    consome_token_com_tipo(&lex, TOKEN_FECHA_CHAVES);
    eof(&lex);
}

static void declaracoes(Lexico *lex)
{
    obterProximoTokenQueNaoComentario(lex);
    if (lex->token.tipo == TOKEN_TIPO_INT)
    {
        consome_token_com_tipo(lex, TOKEN_TIPO_INT);
        lista_variaveis(lex);
        consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
        declaracoes(lex);
    }
    else if (lex->token.tipo == TOKEN_TIPO_BOOL)
    {
        consome_token_com_tipo(lex, TOKEN_TIPO_BOOL);
        lista_variaveis(lex);
        consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
        declaracoes(lex);
    }
}

static void comandos(Lexico *lex)
{
    obterProximoTokenQueNaoComentario(lex);
    while (
        lex->token.tipo == TOKEN_ABRE_CHAVES ||
        lex->token.tipo == TOKEN_IDENTIFICADOR || lex->token.tipo == TOKEN_IF ||
        lex->token.tipo == TOKEN_WHILE || lex->token.tipo == TOKEN_SCANF ||
        lex->token.tipo == TOKEN_PRINTF || lex->token.tipo == TOKEN_PONTO_E_VIRGULA)
    {
        comando(lex);
        obterProximoTokenQueNaoComentario(lex);
    }
}

static void comando(Lexico* lex)
{
    obterProximoTokenQueNaoComentario(lex);
    switch (lex->token.tipo)
    {
    case TOKEN_ABRE_CHAVES:
        bloco_comandos(lex);
        break;
    case TOKEN_IDENTIFICADOR:
        comando_atribuicao(lex);
        break;
    case TOKEN_IF:
        comando_if(lex);
        break;
    case TOKEN_WHILE:
        comando_while(lex);
        break;
    case TOKEN_SCANF:
        comando_scanf(lex);
        break;
    case TOKEN_PRINTF:
        comando_printf(lex);
        break;
    case TOKEN_PONTO_E_VIRGULA:
        consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
        break;
    }
}

static void bloco_comandos(Lexico *lex){
    consome_token_com_tipo(lex, TOKEN_ABRE_CHAVES);
    comandos(lex);
    consome_token_com_tipo(lex, TOKEN_FECHA_CHAVES);
}

static void lista_variaveis(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_VIRGULA)
    {
        consome_token_com_tipo(lex, TOKEN_VIRGULA);
        consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
        obterProximoTokenQueNaoComentario(lex);
    }
}

static void comando_atribuicao(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
    consome_token_com_tipo(lex, TOKEN_ATRIBUICAO);
    expressao(lex);
    consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
}

static void expressao(Lexico *lex)
{
    expressao_logica(lex);
    while (lex->token.tipo == TOKEN_COMENTARIO)
        consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    while (lex->token.tipo == TOKEN_OR)
    {
        consome_token_com_tipo(lex, TOKEN_OR);
        expressao_logica(lex);
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
}

// DAQUI PRA FRENTE FALTA

static void expressao_logica(Lexico *lex)
{
    expressao_relacional(lex);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_AND)
    {
        consome_token_com_tipo(lex, TOKEN_AND);
        expressao_relacional(lex);
        obterProximoTokenQueNaoComentario(lex);
    }
}
static void expressao_relacional(Lexico *lex)
{
    expressao_adicao(lex);
    obterProximoTokenQueNaoComentario(lex);
    switch (lex->token.tipo)
    {
    case TOKEN_COMP_MENOR:
        consome_token_com_tipo(lex, TOKEN_COMP_MENOR);
        expressao_adicao(lex);
        break;
    case TOKEN_COMP_MENOR_OU_IGUAL:
        consome_token_com_tipo(lex, TOKEN_COMP_MENOR_OU_IGUAL);
        expressao_adicao(lex);
        break;
    case TOKEN_COMP_MAIOR:
        consome_token_com_tipo(lex, TOKEN_COMP_MAIOR);
        expressao_adicao(lex);
        break;
    case TOKEN_COMP_MAIOR_OU_IGUAL:
        consome_token_com_tipo(lex, TOKEN_COMP_MAIOR_OU_IGUAL);
        expressao_adicao(lex);
        break;
    case TOKEN_COMP_IGUAL:
        consome_token_com_tipo(lex, TOKEN_COMP_IGUAL);
        expressao_adicao(lex);
        break;
    case TOKEN_COMP_DIFERENTE:
        consome_token_com_tipo(lex, TOKEN_COMP_DIFERENTE);
        expressao_adicao(lex);
        break;
    default:
        break;
    }
}
static void expressao_adicao(Lexico *lex)
{
    expressao_multi(lex);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_OP_SOMA)
    {
        consome_token_com_tipo(lex, TOKEN_OP_SOMA);
        expressao_multi(lex);
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
    while (lex->token.tipo == TOKEN_OP_SUB)
    {
        consome_token_com_tipo(lex, TOKEN_OP_SUB);
        expressao_multi(lex);
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
}
static void expressao_multi(Lexico *lex)
{
    operando(lex);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_OP_MULT)
    {
        consome_token_com_tipo(lex, TOKEN_OP_MULT);
        operando(lex);
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
    while (lex->token.tipo == TOKEN_OP_DIV)
    {
        consome_token_com_tipo(lex, TOKEN_OP_DIV);
        operando(lex);
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
}
static void operando(Lexico *lex)
{
    obterProximoTokenQueNaoComentario(lex);
    if (lex->token.tipo == TOKEN_IDENTIFICADOR)
    {
        consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
    }
    else if (lex->token.tipo == TOKEN_NUMERO)
    {
        consome_token_com_tipo(lex, TOKEN_NUMERO);
    }
    else if (lex->token.tipo == TOKEN_BOOL_TRUE)
    {
        consome_token_com_tipo(lex, TOKEN_BOOL_TRUE);
    }
    else if (lex->token.tipo == TOKEN_BOOL_FALSE)
    {
        consome_token_com_tipo(lex, TOKEN_BOOL_FALSE);
    }
    else
    {
        consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
        expressao(lex);
        consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    }
}

static void comando_if(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_IF);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    expressao(lex);
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    comandos(lex);
    obterProximoTokenQueNaoComentario(lex);
    if (lex->token.tipo == TOKEN_ELSE)
    {
        consome_token_com_tipo(lex, TOKEN_ELSE);
        comandos(lex);
    }
}
static void comando_while(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_WHILE);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    expressao(lex);
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    comandos(lex);
}

static void comando_scanf(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_SCANF);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    lista_variaveis(lex);
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
}

static void comando_printf(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_PRINTF);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    expressao(lex);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_VIRGULA)
    {
        consome_token_com_tipo(lex, TOKEN_VIRGULA);
        expressao(lex);
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
}

static void eof(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_EOF);
    printf("..............\n%d linhas analisadas, programa sintaticamente correto\n", lex->linha);
}

#endif
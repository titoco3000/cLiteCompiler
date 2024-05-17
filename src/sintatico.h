#ifndef SINTATICO_H
#define SINTATICO_H

#include "token.h"
#include "lexico.h"
#include "semantico.h"

#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define EXIT_METHOD() printf("\n");pthread_exit(0);

/*
Deve ser chamado antes de comparações
*/
static void obterProximoTokenQueNaoComentario(Lexico *lex)
{
    while (lex->token.tipo == TOKEN_COMENTARIO)
    {
        //printToken(&lex->token);
        consumir_token(lex);
    }
}

void consome_token_com_tipo(Lexico *lex, TokenType tipo)
{
    obterProximoTokenQueNaoComentario(lex);
    if(lex->token.tipo == TOKEN_ERRO){
        printf("#  %d Erro léxico: %s\n", lex->token.linha, (char*)lex->token.conteudo);
        EXIT_METHOD();
    }
    if (lex->token.tipo != tipo)
    {
        printf("#  %d Erro sintatico: esperado ", lex->token.linha);
        Token esperado = CriarToken(tipo, lex->token.linha);

        printTipoToken(&esperado);
        printf(" encontrado ");
        printTipoToken(&lex->token);
        printf("\n");
        EXIT_METHOD();
    }
    //printToken(&lex->token);
    consumir_token(lex);
}

static void verificar_redeclaracao(Lexico *lex, TabelaDeSimbolos *tabela)
{
    obterProximoTokenQueNaoComentario(lex);
    Simbolo *simboloNaTabela = ProcurarNaTabelaDeSimbolos(tabela, (char *)lex->token.conteudo);
    if (simboloNaTabela)
    {
        printf("#  %d Erro semantico: redeclaração de \"%s\", declarando anteriormente na linha %d\n ", lex->token.linha, simboloNaTabela->identificador, simboloNaTabela->linha);
        EXIT_METHOD();
    }
    simboloNaTabela = CriarSimbolo((char *)lex->token.conteudo, lex->token.linha);
    InserirNaTabelaDeSimbolos(tabela, simboloNaTabela);
}
static Simbolo *verificar_declaracao(Lexico *lex, TabelaDeSimbolos *tabela)
{
    obterProximoTokenQueNaoComentario(lex);
    Simbolo *simboloNaTabela = ProcurarNaTabelaDeSimbolos(tabela, (char *)lex->token.conteudo);
    if (!simboloNaTabela){
        printf("#  %d: Erro semantico: uso de variavel não declarada (%s)\n ", lex->token.linha, (char *)lex->token.conteudo);
        EXIT_METHOD();
    }
    return simboloNaTabela;
}

static void declaracoes(Lexico *lex, TabelaDeSimbolos *tabela);
static void lista_variaveis_declaracao(Lexico *lex, TabelaDeSimbolos *tabela);
static void comandos(Lexico *lex, TabelaDeSimbolos *tabela);
static void comando(Lexico *lex, TabelaDeSimbolos *tabela);
static void bloco_comandos(Lexico *lex, TabelaDeSimbolos *tabela);
static void comando_atribuicao(Lexico *lex, TabelaDeSimbolos *tabela);
static void comando_if(Lexico *lex, TabelaDeSimbolos *tabela);
static void comando_while(Lexico *lex, TabelaDeSimbolos *tabela);
static void comando_scanf(Lexico *lex, TabelaDeSimbolos *tabela);
static void comando_printf(Lexico *lex, TabelaDeSimbolos *tabela);
static void expressao(Lexico *lex, TabelaDeSimbolos *tabela);
static void expressao_logica(Lexico *lex, TabelaDeSimbolos *tabela);
static void expressao_relacional(Lexico *lex, TabelaDeSimbolos *tabela);
static void expressao_adicao(Lexico *lex, TabelaDeSimbolos *tabela);
static void expressao_multi(Lexico *lex, TabelaDeSimbolos *tabela);
static void expressao(Lexico *lex, TabelaDeSimbolos *tabela);
static void lista_variaveis_scanf(Lexico *lex, TabelaDeSimbolos *tabela);
static void operando(Lexico *lex, TabelaDeSimbolos *tabela);
static void fim_de_arquivo(Lexico *lex);

void programa(FILE *entrada, TabelaDeSimbolos *tabela)
{
    Lexico lex = criar_lex(entrada);
    consome_token_com_tipo(&lex, TOKEN_TIPO_INT);
    consome_token_com_tipo(&lex, TOKEN_MAIN);
    consome_token_com_tipo(&lex, TOKEN_ABRE_PARENTESES);
    consome_token_com_tipo(&lex, TOKEN_TIPO_VOID);
    consome_token_com_tipo(&lex, TOKEN_FECHA_PARENTESES);
    consome_token_com_tipo(&lex, TOKEN_ABRE_CHAVES);
    printf("    INPP\n");
    declaracoes(&lex, tabela);
    printf("    AMEM %d\n", ContarSimbolos(tabela));
    comandos(&lex, tabela);
    consome_token_com_tipo(&lex, TOKEN_FECHA_CHAVES);
    fim_de_arquivo(&lex);
}

static void declaracoes(Lexico *lex, TabelaDeSimbolos *tabela)
{
    obterProximoTokenQueNaoComentario(lex);
    if (lex->token.tipo == TOKEN_TIPO_INT)
    {
        consome_token_com_tipo(lex, TOKEN_TIPO_INT);
        lista_variaveis_declaracao(lex, tabela);
        consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
        declaracoes(lex, tabela);
    }
    else if (lex->token.tipo == TOKEN_TIPO_BOOL)
    {
        consome_token_com_tipo(lex, TOKEN_TIPO_BOOL);
        lista_variaveis_declaracao(lex, tabela);
        consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
        declaracoes(lex, tabela);
    }
}

static void comandos(Lexico *lex, TabelaDeSimbolos *tabela)
{
    obterProximoTokenQueNaoComentario(lex);
    while (
        lex->token.tipo == TOKEN_ABRE_CHAVES ||
        lex->token.tipo == TOKEN_IDENTIFICADOR || lex->token.tipo == TOKEN_IF ||
        lex->token.tipo == TOKEN_WHILE || lex->token.tipo == TOKEN_SCANF ||
        lex->token.tipo == TOKEN_PRINTF || lex->token.tipo == TOKEN_PONTO_E_VIRGULA)
    {
        comando(lex, tabela);
        obterProximoTokenQueNaoComentario(lex);
    }
}

static void comando(Lexico *lex, TabelaDeSimbolos *tabela)
{
    obterProximoTokenQueNaoComentario(lex);
    switch (lex->token.tipo)
    {
    case TOKEN_ABRE_CHAVES:
        bloco_comandos(lex, tabela);
        break;
    case TOKEN_IDENTIFICADOR:
        comando_atribuicao(lex, tabela);
        break;
    case TOKEN_IF:
        comando_if(lex, tabela);
        break;
    case TOKEN_WHILE:
        comando_while(lex, tabela);
        break;
    case TOKEN_SCANF:
        comando_scanf(lex, tabela);
        break;
    case TOKEN_PRINTF:
        comando_printf(lex, tabela);
        break;
    case TOKEN_PONTO_E_VIRGULA:
        consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
        break;
    }
}

static void bloco_comandos(Lexico *lex, TabelaDeSimbolos *tabela)
{
    consome_token_com_tipo(lex, TOKEN_ABRE_CHAVES);
    comandos(lex, tabela);
    consome_token_com_tipo(lex, TOKEN_FECHA_CHAVES);
}

static void lista_variaveis_declaracao(Lexico *lex, TabelaDeSimbolos *tabela)
{
    verificar_redeclaracao(lex, tabela);
    consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_VIRGULA)
    {
        consome_token_com_tipo(lex, TOKEN_VIRGULA);
        verificar_redeclaracao(lex, tabela);
        consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
        obterProximoTokenQueNaoComentario(lex);
    }
}
static void lista_variaveis_scanf(Lexico *lex, TabelaDeSimbolos *tabela)
{
    int addr;
    if(lex->token.tipo==TOKEN_IDENTIFICADOR)
        addr = verificar_declaracao(lex, tabela)->endereco;
    
    consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
    printf("    LEIT\n    ARMZ %d\n",addr);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_VIRGULA)
    {
        consome_token_com_tipo(lex, TOKEN_VIRGULA);
        if(lex->token.tipo==TOKEN_IDENTIFICADOR)
            addr = verificar_declaracao(lex, tabela)->endereco;
        consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
        printf("    LEIT\n    ARMZ %d\n",addr);
        obterProximoTokenQueNaoComentario(lex);
    }
}

static void comando_atribuicao(Lexico *lex, TabelaDeSimbolos *tabela)
{
    int addr = verificar_declaracao(lex, tabela)->endereco;
    consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
    consome_token_com_tipo(lex, TOKEN_ATRIBUICAO);
    expressao(lex, tabela);
    consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
    printf("    ARMZ %d\n",addr);
}

static void expressao(Lexico *lex, TabelaDeSimbolos *tabela)
{
    expressao_logica(lex, tabela);
    while (lex->token.tipo == TOKEN_COMENTARIO)
        consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    while (lex->token.tipo == TOKEN_OR)
    {
        consome_token_com_tipo(lex, TOKEN_OR);
        expressao_logica(lex, tabela);
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
}

static void expressao_logica(Lexico *lex, TabelaDeSimbolos *tabela)
{
    expressao_relacional(lex, tabela);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_AND)
    {
        consome_token_com_tipo(lex, TOKEN_AND);
        expressao_relacional(lex, tabela);
        obterProximoTokenQueNaoComentario(lex);
    }
}
static void expressao_relacional(Lexico *lex, TabelaDeSimbolos *tabela)
{
    expressao_adicao(lex, tabela);
    obterProximoTokenQueNaoComentario(lex);
    switch (lex->token.tipo)
    {
    case TOKEN_COMP_MENOR:
        consome_token_com_tipo(lex, TOKEN_COMP_MENOR);
        expressao_adicao(lex, tabela);
        break;
    case TOKEN_COMP_MENOR_OU_IGUAL:
        consome_token_com_tipo(lex, TOKEN_COMP_MENOR_OU_IGUAL);
        expressao_adicao(lex, tabela);
        break;
    case TOKEN_COMP_MAIOR:
        consome_token_com_tipo(lex, TOKEN_COMP_MAIOR);
        expressao_adicao(lex, tabela);
        break;
    case TOKEN_COMP_MAIOR_OU_IGUAL:
        consome_token_com_tipo(lex, TOKEN_COMP_MAIOR_OU_IGUAL);
        expressao_adicao(lex, tabela);
        break;
    case TOKEN_COMP_IGUAL:
        consome_token_com_tipo(lex, TOKEN_COMP_IGUAL);
        expressao_adicao(lex, tabela);
        break;
    case TOKEN_COMP_DIFERENTE:
        consome_token_com_tipo(lex, TOKEN_COMP_DIFERENTE);
        expressao_adicao(lex, tabela);
        break;
    default:
        break;
    }
}
static void expressao_adicao(Lexico *lex, TabelaDeSimbolos *tabela)
{
    expressao_multi(lex, tabela);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_OP_SOMA)
    {
        consome_token_com_tipo(lex, TOKEN_OP_SOMA);
        expressao_multi(lex, tabela);
        printf("    SOMA\n");
        obterProximoTokenQueNaoComentario(lex);
    }
    while (lex->token.tipo == TOKEN_OP_SUB)
    {
        consome_token_com_tipo(lex, TOKEN_OP_SUB);
        expressao_multi(lex, tabela);
        printf("    SUBT\n");
        obterProximoTokenQueNaoComentario(lex);
    }
}
static void expressao_multi(Lexico *lex, TabelaDeSimbolos *tabela)
{
    operando(lex, tabela);
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_OP_MULT)
    {
        consome_token_com_tipo(lex, TOKEN_OP_MULT);
        operando(lex, tabela);
        printf("    MULT\n");
        obterProximoTokenQueNaoComentario(lex);
    }
    while (lex->token.tipo == TOKEN_OP_DIV)
    {
        consome_token_com_tipo(lex, TOKEN_OP_DIV);
        operando(lex, tabela);
        printf("    DIVI\n");
        obterProximoTokenQueNaoComentario(lex);
    }
}
static void operando(Lexico *lex, TabelaDeSimbolos *tabela)
{
    obterProximoTokenQueNaoComentario(lex);
    if (lex->token.tipo == TOKEN_IDENTIFICADOR)
    {
        int addr = verificar_declaracao(lex,tabela)->endereco;
        printf("    CRVL %d\n",addr);
        consome_token_com_tipo(lex, TOKEN_IDENTIFICADOR);
    }
    else if (lex->token.tipo == TOKEN_NUMERO)
    {
        printf("    CRCT %d\n",*(int*)lex->token.conteudo);
        consome_token_com_tipo(lex, TOKEN_NUMERO);
    }
    else if (lex->token.tipo == TOKEN_BOOL_TRUE)
    {
        printf("    CRCT 1\n");
        consome_token_com_tipo(lex, TOKEN_BOOL_TRUE);
    }
    else if (lex->token.tipo == TOKEN_BOOL_FALSE)
    {
        printf("    CRCT 0\n");
        consome_token_com_tipo(lex, TOKEN_BOOL_FALSE);
    }
    else
    {
        consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
        expressao(lex, tabela);
        consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    }
}

static void comando_if(Lexico *lex, TabelaDeSimbolos *tabela)
{
    
    consome_token_com_tipo(lex, TOKEN_IF);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    expressao(lex, tabela);
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    int end_if = CriarNovoLabelNaTabelaDeSimbolos(tabela);
    int end_else;
    printf("    DSVF L%d\n", end_if);
    comando(lex, tabela);
    obterProximoTokenQueNaoComentario(lex);
    if (lex->token.tipo == TOKEN_ELSE){
        end_else  = CriarNovoLabelNaTabelaDeSimbolos(tabela);
        printf("    DSVS L%d\n", end_else);
    }
    printf("L%d  NADA\n",end_if);
    if (lex->token.tipo == TOKEN_ELSE)
    {
        consome_token_com_tipo(lex, TOKEN_ELSE);
        comando(lex, tabela);
        printf("L%d  NADA\n",end_else);
    }
}
static void comando_while(Lexico *lex, TabelaDeSimbolos *tabela)
{
    int end_while = CriarNovoLabelNaTabelaDeSimbolos(tabela);
    consome_token_com_tipo(lex, TOKEN_WHILE);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    expressao(lex, tabela);
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    printf("    DSVF L%d\n", end_while);
    comando(lex, tabela);
    printf("L%d  NADA\n",end_while);
}

static void comando_scanf(Lexico *lex, TabelaDeSimbolos *tabela)
{
    consome_token_com_tipo(lex, TOKEN_SCANF);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    lista_variaveis_scanf(lex, tabela);
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
}

static void comando_printf(Lexico *lex, TabelaDeSimbolos *tabela)
{
    consome_token_com_tipo(lex, TOKEN_PRINTF);
    consome_token_com_tipo(lex, TOKEN_ABRE_PARENTESES);
    expressao(lex, tabela);
    printf("    IMPR\n");
    obterProximoTokenQueNaoComentario(lex);
    while (lex->token.tipo == TOKEN_VIRGULA)
    {
        consome_token_com_tipo(lex, TOKEN_VIRGULA);
        expressao(lex, tabela);
        printf("    IMPR\n");
        while (lex->token.tipo == TOKEN_COMENTARIO)
            consome_token_com_tipo(lex, TOKEN_COMENTARIO);
    }
    consome_token_com_tipo(lex, TOKEN_FECHA_PARENTESES);
    consome_token_com_tipo(lex, TOKEN_PONTO_E_VIRGULA);
}

static void fim_de_arquivo(Lexico *lex)
{
    consome_token_com_tipo(lex, TOKEN_EOF);
    printf("    PARA\n");
}

#endif
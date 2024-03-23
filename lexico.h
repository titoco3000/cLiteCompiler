#ifndef LEXICO_H
#define LEXICO_H

#include "token.h"
#include <ctype.h>
#include <string.h>

#define LEX_BUFFER_SIZE 100

static void consomeChar(char *charPtr, FILE* entrada, int *contadorDeLinhas)
{
    if (*charPtr == '\n')
        (*contadorDeLinhas)++;
    *charPtr = getc(entrada);
}

int hexStrToInt(char *str)
{
    int len = strlen(str);
    int num = 0;
    int multi = 1;
    for (int i = len - 1; i >= 0; i--)
    {
        int digito;
        if (isdigit(str[i]))
            digito = str[i] - '0';
        else if (str[i] == 'A')
            digito = 10;
        else if (str[i] == 'B')
            digito = 11;
        else if (str[i] == 'C')
            digito = 12;
        else if (str[i] == 'D')
            digito = 13;
        else if (str[i] == 'E')
            digito = 14;
        else if (str[i] == 'F')
            digito = 15;

        int n = digito * multi;
        num += n;
        multi *= 16;
    }
    return num;
}

int ehCharDeFechamento(char c)
{
    // tudo que pode delimitar o fim de um identificador ou numero
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == ';' || c == '=' || c == '|' || c == '&' || c == '<' || c == '>' || c == '!' || c == '+' || c == '-' || c == '*' || c == '/' || c == '\n' || c == '\t' || c == '\0' || c == ' '||c==EOF;
}
/*
 */
Token *analiseLexica(FILE *entrada)
{
    char lookahead = (char)getc(entrada);
    int linha = 1;
    char buffer[LEX_BUFFER_SIZE];
    int bufferOcup = 0;

    Token *tokenList = 0;

    int erroLexico = 0;

    while (lookahead != '\0' && lookahead != EOF && !erroLexico)
    {
        // primeiro, lida com divisões e comentarios
        if (lookahead == '/')
        {
            consomeChar(&lookahead, entrada, &linha);
            // comentarios simples
            if (lookahead == '/')
            {
                while (lookahead != '\n' || '\0')
                    consomeChar(&lookahead, entrada, &linha);
            }
            // comentarios multilinha
            else if (lookahead == '*')
            {
                while (1)
                {
                    consomeChar(&lookahead, entrada, &linha);
                    if (lookahead == '*')
                    {
                        consomeChar(&lookahead, entrada, &linha);
                        if (lookahead == '/')
                        {
                            consomeChar(&lookahead, entrada, &linha);
                            break;
                        }
                    }
                }
            }
            // divisão
            else
            {
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_OP_DIV, linha));
            }
        }
        else if (lookahead == '(')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_ABRE_PARENTESES, linha));
        }
        else if (lookahead == ')')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_FECHA_PARENTESES, linha));
        }
        else if (lookahead == '{')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_ABRE_CHAVES, linha));
        }
        else if (lookahead == '}')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_FECHA_CHAVES, linha));
        }
        else if (lookahead == ',')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_VIRGULA, linha));
        }
        else if (lookahead == ';')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_PONTO_E_VIRGULA, linha));
        }
        // igualdade e atribuicao
        else if (lookahead == '=')
        {
            consomeChar(&lookahead, entrada, &linha);
            if (lookahead == '=')
            {
                consomeChar(&lookahead, entrada, &linha);
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_COMP_IGUAL, linha));
            }
            else
            {
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_ATRIBUICAO, linha));
            }
        }
        // or (||)
        else if (lookahead == '|')
        {
            consomeChar(&lookahead, entrada, &linha);
            if (lookahead == '|')
            {
                consomeChar(&lookahead, entrada, &linha);
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_OR, linha));
            }
            else
            {
                erroLexico = 1;
                InserirTokenNaLista(&tokenList, CriarTokenErro("Falta do segundo '|'", linha));
            }
        }
        // and (&&)
        else if (lookahead == '&')
        {
            consomeChar(&lookahead, entrada, &linha);
            if (lookahead == '&')
            {
                consomeChar(&lookahead, entrada, &linha);
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_AND, linha));
            }
            else
            {
                erroLexico = 1;
                InserirTokenNaLista(&tokenList, CriarTokenErro("Falta do segundo '&'", linha));
            }
        }
        // menor e menor_igual
        else if (lookahead == '<')
        {
            consomeChar(&lookahead, entrada, &linha);
            if (lookahead == '=')
            {
                consomeChar(&lookahead, entrada, &linha);
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_COMP_MENOR_OU_IGUAL, linha));
            }
            else
            {
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_COMP_MENOR, linha));
            }
        }
        // maior e maior_igual
        else if (lookahead == '>')
        {
            consomeChar(&lookahead, entrada, &linha);
            if (lookahead == '=')
            {
                consomeChar(&lookahead, entrada, &linha);
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_COMP_MAIOR_OU_IGUAL, linha));
            }
            else
            {
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_COMP_MAIOR, linha));
            }
        }
        else if (lookahead == '+')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_OP_SOMA, linha));
        }
        else if (lookahead == '-')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_OP_SUB, linha));
        }
        else if (lookahead == '*')
        {
            consomeChar(&lookahead, entrada, &linha);
            InserirTokenNaLista(&tokenList, CriarToken(TOKEN_OP_MULT, linha));
        }
        // numero
        else if (lookahead == '0')
        {
            consomeChar(&lookahead, entrada, &linha);
            if (lookahead == 'x')
            {
                consomeChar(&lookahead, entrada, &linha);
                bufferOcup = 0;
                while (isdigit(lookahead) || lookahead == 'A' || lookahead == 'B' || lookahead == 'C' || lookahead == 'D' || lookahead == 'E' || lookahead == 'F')
                {
                    buffer[bufferOcup++] = lookahead;
                    consomeChar(&lookahead, entrada, &linha);
                }
                if (ehCharDeFechamento(lookahead))
                {
                    buffer[bufferOcup] = 0;
                    int numero = hexStrToInt(buffer);
                    InserirTokenNaLista(&tokenList, CriarTokenNumero(numero, linha));
                }
                else
                {
                    erroLexico = 1;
                    InserirTokenNaLista(&tokenList, CriarTokenErro("numero emendado com caracteres não permitidos", linha));
                }
            }
            else
            {
                erroLexico = 1;
                InserirTokenNaLista(&tokenList, CriarTokenErro("após prefixo '0', deve vir 'x'", linha));
            }
        }
        else if (lookahead == '_')
        {
            consomeChar(&lookahead, entrada, &linha);
            bufferOcup = 0;
            buffer[bufferOcup++] = '_';
            if (isalpha(lookahead))
            {
                buffer[bufferOcup++] = lookahead;
                consomeChar(&lookahead, entrada, &linha);
                while (!ehCharDeFechamento(lookahead))
                {
                    buffer[bufferOcup++] = lookahead;
                    consomeChar(&lookahead, entrada, &linha);
                }
                buffer[bufferOcup] = 0;
                InserirTokenNaLista(&tokenList, CriarTokenIdentifier(buffer, linha));
            }
            else
            {
                erroLexico = 1;
                InserirTokenNaLista(&tokenList, CriarTokenErro("Identificador deve começar com underline + letra", linha));
            }
        }
        // espaços em branco
        else if (lookahead == '\n' || lookahead == ' ' || lookahead == '\t')
        {
            consomeChar(&lookahead, entrada, &linha);
        }

        else
        {
            bufferOcup = 0;
            while (!ehCharDeFechamento(lookahead) && !erroLexico)
            {
                if (isalpha(lookahead))
                {
                    buffer[bufferOcup++] = lookahead;
                    consomeChar(&lookahead, entrada, &linha);
                }
                else
                {
                    char msg[36] = "char invalido em palavra-chave(\' \')";
                    msg[32] = lookahead;
                    erroLexico = 1;
                    InserirTokenNaLista(&tokenList, CriarTokenErro(msg, linha));
                }
            }
            buffer[bufferOcup] = 0;
            if (strcmp(buffer, "int") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_TIPO_INT, linha));
            else if (strcmp(buffer, "bool") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_TIPO_BOOL, linha));
            else if (strcmp(buffer, "void") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_TIPO_VOID, linha));
            else if (strcmp(buffer, "main") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_MAIN, linha));
            else if (strcmp(buffer, "if") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_IF, linha));
            else if (strcmp(buffer, "else") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_ELSE, linha));
            else if (strcmp(buffer, "while") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_WHILE, linha));
            else if (strcmp(buffer, "scanf") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_SCANF, linha));
            else if (strcmp(buffer, "printf") == 0)
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_PRINTF, linha));
            else
            {
                erroLexico = 1;
                InserirTokenNaLista(&tokenList, CriarTokenErro("Termo não reconhecido", linha));
            }
        }
    }
    if (!erroLexico)
        InserirTokenNaLista(&tokenList, CriarToken(TOKEN_EOF, linha));

    return tokenList;
}
#endif

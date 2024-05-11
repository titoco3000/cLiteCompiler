#ifndef LEXICO_H
#define LEXICO_H

#include "token.h"
#include <ctype.h>
#include <string.h>

#define LEX_BUFFER_SIZE 100

// faz charPtr apontar para o proximo, e modifica o contador de linhas se necessario
static void consomeChar(char *charPtr, FILE *entrada, int *contadorDeLinhas)
{
    if (*charPtr == '\n')
        (*contadorDeLinhas)++;
    *charPtr = getc(entrada);
}

// verifica se é um digito de um numero hexadecimal
static int isHexDigit(char c)
{
    return isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F';
}

// Converte numero hex em uma string para inteiro
static int hexStrToInt(char *str)
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

        num += digito * multi;
        multi *= 16;
    }
    return num;
}

// Retorna lista de tokens
Token *analiseLexica(FILE *entrada)
{
    char lookahead = getc(entrada);
    int linha = 1;

    char buffer[LEX_BUFFER_SIZE];
    int bufferOcup = 0;

    Token *tokenList = NULL;

    // bool
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
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_COMENTARIO, linha));
                while (lookahead != '\n' && lookahead != EOF)
                    consomeChar(&lookahead, entrada, &linha);
                  
            }
            // comentarios multilinha
            else if (lookahead == '*')
            {
                InserirTokenNaLista(&tokenList, CriarToken(TOKEN_COMENTARIO, linha));
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
                    if(lookahead == EOF){
                        InserirTokenNaLista(&tokenList, CriarTokenErro("Comentario inacabado",linha));
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
            // todo numero começa com 0x
            consomeChar(&lookahead, entrada, &linha);
            if (lookahead == 'x')
            {
                consomeChar(&lookahead, entrada, &linha);
                bufferOcup = 0;
                // enquanto encontrar digitos, vai adicionando no buffer
                while (isHexDigit(lookahead))
                {
                    buffer[bufferOcup++] = lookahead;
                    consomeChar(&lookahead, entrada, &linha);
                }
                // se chegou num fim válido
                if (!isalpha(lookahead))
                {
                    // adiciona fim da string
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
        // identificador
        else if (lookahead == '_')
        {
            consomeChar(&lookahead, entrada, &linha);
            bufferOcup = 0;
            buffer[bufferOcup++] = '_';
            // primeiro char do identificador deve ser uma letra
            if (isalpha(lookahead))
            {
                buffer[bufferOcup++] = lookahead;
                consomeChar(&lookahead, entrada, &linha);
                while (isalnum(lookahead))
                {
                    buffer[bufferOcup++] = lookahead;
                    consomeChar(&lookahead, entrada, &linha);
                }
                // adiciona fim da string
                buffer[bufferOcup] = 0;
                if(bufferOcup>16){
                    erroLexico = 1;
                    InserirTokenNaLista(&tokenList, CriarTokenErro("Identificador com mais de 15 caracteres", linha));
                }
                else
                    InserirTokenNaLista(&tokenList, CriarTokenIdentifier(buffer, linha));
            }
            else
            {
                erroLexico = 1;
                InserirTokenNaLista(&tokenList, CriarTokenErro("Identificador deve começar com underline + letra", linha));
            }
        }
        // espaços em branco são ignorados
        else if (lookahead == '\n' || lookahead == '\r' || lookahead == ' ' || lookahead == '\t')
        {
            consomeChar(&lookahead, entrada, &linha);
        }

        // palavras-chave
        else
        {
            bufferOcup = 0;
            while (isalnum(lookahead) && !erroLexico)
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
            // adiciona fim da string
            buffer[bufferOcup] = 0;

            // compara lista de palavras-chave
            char *palavrasChave[11] = {"int", "bool", "void", "main", "if", "else", "while", "scanf", "printf", "true", "false"};
            TokenType tiposChave[11] = {TOKEN_TIPO_INT, TOKEN_TIPO_BOOL, TOKEN_TIPO_VOID, TOKEN_MAIN, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_SCANF, TOKEN_PRINTF, TOKEN_BOOL_TRUE, TOKEN_BOOL_FALSE};

            int achou = 0;
            for (int i = 0; (i < 11 && !achou); i++)
            {
                if (strcmp(buffer, palavrasChave[i]) == 0){
                    InserirTokenNaLista(&tokenList, CriarToken(tiposChave[i], linha));
                    achou = 1;
                }
            }
            if(!achou)
            {
                erroLexico = 1;
                InserirTokenNaLista(&tokenList, CriarTokenErro("Palavra-chave não reconhecida", linha));
            }
        }
    }
    if (!erroLexico)
        InserirTokenNaLista(&tokenList, CriarToken(TOKEN_EOF, linha));

    return tokenList;
}
#endif

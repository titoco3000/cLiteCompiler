#ifndef LEXICO_H
#define LEXICO_H

#include "token.h"
#include <ctype.h>
#include <string.h>

#define LEX_BUFFER_SIZE 100

typedef struct Lexico
{
    FILE *arquivo;
    int linha;
    char lookahead;
    Token token;
} Lexico;

// faz charPtr apontar para o proximo, e modifica o contador de linhas se necessario
static void consomeChar(Lexico* lex)
{
    if (lex->lookahead == '\n')
        lex->linha++;
    lex->lookahead = getc(lex->arquivo);
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

Token obter_proximo_token(Lexico* lex)
{
    char buffer[LEX_BUFFER_SIZE];
    int bufferOcup = 0;

    // pula espaços vazios
    while (lex->lookahead == '\n' || lex->lookahead == '\r' || lex->lookahead == ' ' || lex->lookahead == '\t')
    {
        consomeChar(lex);
    }
    // primeiro, lida com divisões e comentarios
    if (lex->lookahead == '/')
    {
        consomeChar(lex);
        // comentarios simples
        if (lex->lookahead == '/')
        {
            while (lex->lookahead != '\n' && lex->lookahead != EOF)
                consomeChar(lex);
            return CriarToken(TOKEN_COMENTARIO, lex->linha);
        }
        // comentarios multilinha
        else if (lex->lookahead == '*')
        {
            Token t = CriarToken(TOKEN_COMENTARIO, lex->linha);
            while (1)
            {
                consomeChar(lex);
                if (lex->lookahead == '*')
                {
                    consomeChar(lex);
                    if (lex->lookahead == '/')
                    {
                        consomeChar(lex);
                        break;
                    }
                }
                if (lex->lookahead == EOF)
                {
                    return CriarTokenErro("Comentario inacabado", lex->linha);
                }
            }
            return t;
        }
        // divisão
        else
            return CriarToken(TOKEN_OP_DIV, lex->linha);
    }
    // tokens demarcados por um unico char, sem duvidas
    else if (lex->lookahead == '(')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_ABRE_PARENTESES, lex->linha);
    }
    else if (lex->lookahead == ')')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_FECHA_PARENTESES, lex->linha);
    }
    else if (lex->lookahead == '{')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_ABRE_CHAVES, lex->linha);
    }
    else if (lex->lookahead == '}')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_FECHA_CHAVES, lex->linha);
    }
    else if (lex->lookahead == ',')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_VIRGULA, lex->linha);
    }
    else if (lex->lookahead == ';')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_PONTO_E_VIRGULA, lex->linha);
    }
    // igualdade e atribuicao
    else if (lex->lookahead == '=')
    {
        consomeChar(lex);
        if (lex->lookahead == '=')
        {
            consomeChar(lex);
            return CriarToken(TOKEN_COMP_IGUAL, lex->linha);
        }
        else
            return CriarToken(TOKEN_ATRIBUICAO, lex->linha);
    }
    // or (||)
    else if (lex->lookahead == '|')
    {
        consomeChar(lex);
        if (lex->lookahead == '|')
        {
            consomeChar(lex);
            return CriarToken(TOKEN_OR, lex->linha);
        }
        else
            return CriarTokenErro("Falta do segundo '|'", lex->linha);
    }
    // and (&&)
    else if (lex->lookahead == '&')
    {
        consomeChar(lex);
        if (lex->lookahead == '&')
        {
            consomeChar(lex);
            return CriarToken(TOKEN_AND, lex->linha);
        }
        else
            return CriarTokenErro("Falta do segundo '&'", lex->linha);
    }
    // menor e menor_igual
    else if (lex->lookahead == '<')
    {
        consomeChar(lex);
        if (lex->lookahead == '=')
        {
            consomeChar(lex);
            return CriarToken(TOKEN_COMP_MENOR_OU_IGUAL, lex->linha);
        }
        else
            return CriarToken(TOKEN_COMP_MENOR, lex->linha);
    }
    // maior e maior_igual
    else if (lex->lookahead == '>')
    {
        consomeChar(lex);
        if (lex->lookahead == '=')
        {
            consomeChar(lex);
            return CriarToken(TOKEN_COMP_MAIOR_OU_IGUAL, lex->linha);
        }
        else
            return CriarToken(TOKEN_COMP_MAIOR, lex->linha);
    }
    else if (lex->lookahead == '+')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_OP_SOMA, lex->linha);
    }
    else if (lex->lookahead == '-')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_OP_SUB, lex->linha);
    }
    else if (lex->lookahead == '*')
    {
        consomeChar(lex);
        return CriarToken(TOKEN_OP_MULT, lex->linha);
    }
    else if (lex->lookahead == EOF)
        return CriarToken(TOKEN_EOF, lex->linha);
    // numero
    else if (lex->lookahead == '0')
    {
        // todo numero começa com 0x
        consomeChar(lex);
        if (lex->lookahead == 'x')
        {
            consomeChar(lex);
            bufferOcup = 0;
            // enquanto encontrar digitos, vai adicionando no buffer
            while (isHexDigit(lex->lookahead))
            {
                buffer[bufferOcup++] = lex->lookahead;
                consomeChar(lex);
            }
            // se chegou num fim válido
            if (!isalpha(lex->lookahead))
            {
                // adiciona fim da string
                buffer[bufferOcup] = 0;
                int numero = hexStrToInt(buffer);
                return CriarTokenNumero(numero, lex->linha);
            }
            else
                return CriarTokenErro("numero invalido", lex->linha);
        }
        else
            return CriarTokenErro("após prefixo '0', deve vir 'x'", lex->linha);
    }
    // identificador
    else if (lex->lookahead == '_')
    {
        consomeChar(lex);
        bufferOcup = 0;
        buffer[bufferOcup++] = '_';
        // primeiro char do identificador deve ser uma letra
        if (isalpha(lex->lookahead))
        {
            buffer[bufferOcup++] = lex->lookahead;
            consomeChar(lex);
            while (isalnum(lex->lookahead) && bufferOcup <= 16)
            {
                buffer[bufferOcup++] = lex->lookahead;
                consomeChar(lex);
            }
            // adiciona fim da string
            buffer[bufferOcup] = 0;
            if (bufferOcup > 16)
                return CriarTokenErro("Identificador com mais de 15 caracteres", lex->linha);
            else
                return CriarTokenIdentifier(buffer, lex->linha);
        }
        else
            return CriarTokenErro("Identificador deve começar com underline + letra", lex->linha);
    }
    // palavras-chave
    else
    {
        bufferOcup = 0;
        while (isalnum(lex->lookahead))
        {
            if (isalpha(lex->lookahead))
            {
                buffer[bufferOcup++] = lex->lookahead;
                consomeChar(lex);
            }
            else
            {
                char msg[36] = "char invalido em palavra-chave(\' \')";
                msg[32] = lex->lookahead;
                return CriarTokenErro(msg, lex->linha);
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
            if (strcmp(buffer, palavrasChave[i]) == 0)
            {
                return CriarToken(tiposChave[i], lex->linha);
                achou = 1;
            }
        }
        if (!achou)
            return CriarTokenErro("Palavra-chave não reconhecida", lex->linha);
    }
}

Lexico criar_lex(FILE* arquivo){
    Lexico lex;
    lex.arquivo = arquivo;
    lex.lookahead = getc(arquivo);
    lex.linha = 1;
    lex.token = obter_proximo_token(&lex);
    return lex;
}

void consumir_token(Lexico *lex){
    liberarToken(lex->token);
    lex->token = obter_proximo_token(lex);
}

#endif
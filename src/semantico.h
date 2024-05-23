#ifndef SEMANTICO_H
#define SEMANTICO_H

#include <string.h>
#include "token.h"


typedef struct Simbolo
{
    char* identificador;
    int endereco;
    int linha;
    struct Simbolo *prox;
} Simbolo;

typedef struct TabelaDeSimbolos
{
    Simbolo* lista;
    int labels;

} TabelaDeSimbolos;

TabelaDeSimbolos CriarTabelaDeSimbolos(){
    TabelaDeSimbolos tabela;
    tabela.lista = 0;
    tabela.labels = 0;
    return tabela;
}
Simbolo* CriarSimbolo(char* identificador, int linha){
    Simbolo* s = (Simbolo*)malloc(sizeof(Simbolo));
    s->identificador = (char*)malloc(sizeof(identificador));
    strcpy(s->identificador, identificador);
    s->prox = 0;
    s->linha = linha;
    return s;
}

void LiberarSimbolo(Simbolo *s){
    free(s->identificador);
    free(s);
}

Simbolo* ProcurarNaTabelaDeSimbolos(TabelaDeSimbolos *tabela, char* identificador){
    Simbolo *prox = tabela->lista;
    while (prox!=0)
    {
        if(strcmp(identificador, (char*)prox->identificador) == 0)
            return prox;
        prox = prox->prox;
    }
    return 0;
}

void InserirNaTabelaDeSimbolos(TabelaDeSimbolos *tabela, Simbolo* novo){
    int addr = 0;
    Simbolo** lista = &(tabela->lista);
    while (*lista != 0){
        lista = &((*lista)->prox);
        addr++;
    }
    
    *lista = novo;
    novo->endereco = addr;
}

int ContarSimbolos(TabelaDeSimbolos *tabela){
    int qtd = 0;
    Simbolo *proximo = tabela->lista;
    while (proximo)
    {
        qtd++;
        proximo = proximo->prox;
    }
    return qtd;
}

int CriarNovoLabelNaTabelaDeSimbolos(TabelaDeSimbolos* tabela){
    return tabela->labels++ + 1;
}

void LiberarTabelaDeSimbolos(TabelaDeSimbolos tabela){
    Simbolo *prox = tabela.lista;
    while (prox!=0)
    {
        Simbolo *temp = prox->prox;
        LiberarSimbolo(prox);
        prox = temp;
    } 
}

#endif
# Compilador CLite

CLite é uma linguagem baseada na linguagem C, porém mais simples:

- Todas as variaveis começam com <code>_</code>;
- Os unicos tipos disponíveis são <code>int</code> e <code>bool</code>;
- A unica função é a <code>main</code>

## Implementação

O compilador é dividido por etapas:

### Análise Léxica

Recebe um arquivo e retorna uma lista de <code>tokens</code>.

### Análise Sintática

Verifica se a ordem dos tokens está de acordo com a gramática.

## Uso:

Para compilar & executar

    gcc src/main.c -o build/clitec.exe && ./build/clitec.exe exemplos

Atenção: em Windows, a compilação de pastas funciona, mas não a de subpastas.


## Tabela Mepa

| Cód   | Descrição 
| ----- | --------  
| INPP  | Inicia programa      
| AMEM  | Aloca memoria
| ARMZ  | Armazena
| SOMA  | Soma      
| SUBT  | Subtração 
| MULT  | Multiplicação
| DIVI  | Divisão
| INVR  | Inverte o numero em S
| CONJ  | and
| DISJ  | or
| NEGA  | !
| CMME  | <
| CMMA  | >
| CMIG  | ==
| CMDG  | !=
| CMEG  | <=
| CMAG  | >=
| CRCT  | Carrega constante na variavel
| CRVL  | Carrega valor de S na variavel
| LEIT  | Leitura
| IMPR  | Imprime
| PARA  | Encerra o programa
| DSVS  | Desvia sempre
| DSVF  | Desvia se falso
| NADA  | Instrução vazia


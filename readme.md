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

### Resto (a ser feito)

## Uso:

Para compilar & executar

    gcc src/main.c -o build/clitec.exe && ./build/clitec.exe exemplos/ex1.clite

## Questões
- Existe um limite pro tamanho de identificadores?
- Devemos separar definição e implementação?

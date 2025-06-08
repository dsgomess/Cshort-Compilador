#ifndef ANALEX_H
#define ANALEX_H

#include <stdio.h> // Adicionado para o tipo FILE

// --- DEFINIÇÕES DE TAMANHO ---
#define TAM_MAX_LEXEMA 50 // Tamanho máximo para identificadores e literais string
#define TAM_NUM 20        // Tamanho máximo para o buffer de números

// --- CATEGORIAS DE TOKEN ---
enum TOKEN_CAT {
    ID = 1,
    PAL_RESERV,
    SINAL,
    CONST_CHAR,
    CONST_FLOAT,
    CONST_INT,
    LITERAL,
    OP_ARIT,
    OP_LOGIC,
    OP_RELAC,
    FIM_PROG // Fim de programa (EOF)
    
};

// --- CÓDIGOS PARA PALAVRAS RESERVADAS ---
enum PAL_RESERV {
    VOID = 1, CHAR, INT, FLOAT, BOOL,
    IF, ELSE, WHILE, FOR, RETURN
};

// --- CÓDIGOS PARA SINAIS DE PONTUAÇÃO ---
enum SINAIS {
    ABRE_PAR = 1, FECHA_PAR,
    ABRE_CHAVE, FECHA_CHAVE,
    ABRE_COL, FECHA_COL,
    VIRGULA, PONTO_VIRGULA,
    REFERENCIA
};

// --- CÓDIGOS PARA OPERADORES ---
enum OP_ARIT {
    ATRIBUICAO = 1, ADICAO, SUBTRACAO, MULTIPLICACAO, DIVISAO
};

enum OP_RELAC {
    IGUALDADE = 1, DIFERENTE, MENOR_IGUAL, MAIOR_IGUAL,
    MENOR_QUE, MAIOR_QUE
};

enum OP_LOGIC {
    AND_LOGIC = 1, OR_LOGIC, NOT_LOGIC
};

// --- ESTRUTURA DO TOKEN ---
typedef struct {
    enum TOKEN_CAT cat;
    union {
        int codigo;
        // 'indice' pode ser útil para a tabela de símbolos no futuro
        int indice;
        char lexema[TAM_MAX_LEXEMA];
        int valInt;
        float valFloat;
        char caractere;
    };
} TOKEN;

// --- PROTÓTIPO DA FUNÇÃO DO ANALISADOR LÉXICO ---

TOKEN AnaLex(FILE *fd);

#endif // Fim de ANALEX_H
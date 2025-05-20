#include <stdio.h>

#ifndef ANALEXDOBLOCK_H

#define ANALEXDOBLOCK_H

#define TAM_MAX_LEXEMA 31

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
    FIM_EXPR,
    FIM_PROG
};


enum PAL_RESERV {
    VOID = 1,
    CHAR,
    INT,
    FLOAT,
    BOOL,
    IF,
    ELSE,
    WHILE,
    FOR,
    RETURN
};

enum SINAIS {
    ABRE_PAR = 1,
    FECHA_PAR,
    ABRE_CHAVE,
    FECHA_CHAVE,
    ABRE_COL,
    FECHA_COL,
    VIRGULA,
    PONTO_VIRGULA
};


enum OP_ARIT
{
    ATRIBUICAO = 1, // =
    ADICAO,         // +
    SUBTRACAO,      // -
    MULTIPLICACAO,  // *
    DIVISAO         // /
};

enum OP_RELAC
{
    IGUALDADE = 1, // ==
    DIFERENTE,     // !=
    MENOR_IGUAL,   // <=
    MAIOR_IGUAL,   // >=
    MENOR_QUE,     // <
    MAIOR_QUE      // >
};

enum OP_LOGIC
{
    AND_LOGIC = 1, // &&
    OR_LOGIC,      // ||
    NOT_LOGIC      // !
};

typedef struct {
    enum TOKEN_CAT cat;
    union {
        int codigo;
        int indice;
        char lexema[TAM_MAX_LEXEMA];
        int valInt;
        float valFloat;
        char caractere;
    };
} TOKEN;

extern TOKEN tk;
extern FILE *fd;
TOKEN AnaLex(FILE *);

#endif
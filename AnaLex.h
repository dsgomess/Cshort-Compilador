#include <stdio.h>

#ifndef ANALEXDOBLOCK_H

#define ANALEXDOBLOCK_H

#define TAM_MAX_LEXEMA 31

enum TOKEN_CAT {
    ID = 1, //1 identificador
    PAL_RESERV, //2 palavra reservada
    SINAL, //3 sinal
    CONST_CHAR, //4 constante char
    CONST_FLOAT, //5 constante float
    CONST_INT, //6 constante int
    LITERAL,  //7 literal
    OP_ARIT,  //8 operador aritmetico
    OP_LOGIC, //9 operador logico
    OP_RELAC, //10 operador relacional
    FIM_EXPR, //11 fim de expressao
    FIM_PROG  //12 fim de programa
};


enum PAL_RESERV {
    VOID = 1, //1 void
    CHAR, // 2 char
    INT, // 3 inteiro
    FLOAT, // 4 ponto flutuante
    BOOL, // 5 booleano
    IF, // 6 se
    ELSE, // 7 senao
    WHILE,  // 8 enquanto
    FOR, // 9 para
    RETURN // 10 retornar
};

enum SINAIS {
    ABRE_PAR = 1,   // 1 abre parenteses (
    FECHA_PAR, // 2 fecha parenteses )
    ABRE_CHAVE, // 3 abre chave {
    FECHA_CHAVE, // 4 fecha chave }
    ABRE_COL, // 5 abre colchete [
    FECHA_COL, // 6 fecha colchete ]
    VIRGULA, // 7 virgula ,
    PONTO_VIRGULA,  // 8 ponto e virgula ;
    REFERENCIA // 9 referencia &
};


enum OP_ARIT
{
    ATRIBUICAO = 1, // 1 =
    ADICAO,         // 2 +
    SUBTRACAO,      // 3 -
    MULTIPLICACAO,  // 4 *
    DIVISAO         // 5 /
};

enum OP_RELAC
{
    IGUALDADE = 1, // 1 ==
    DIFERENTE,     // 2 !=
    MENOR_IGUAL,   // 3 <=
    MAIOR_IGUAL,   // 4 >=
    MENOR_QUE,     // 5 <
    MAIOR_QUE      // 6 >
};

enum OP_LOGIC
{
    AND_LOGIC = 1, // 1 &&
    OR_LOGIC,      // 2 ||
    NOT_LOGIC      // 3 !
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
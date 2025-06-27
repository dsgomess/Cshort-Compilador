#ifndef ANASINT_CSHORT_H
#define ANASINT_CSHORT_H

#include "AnaLex.h" // Inclui o nosso analisador léxico
#include <stdio.h>

// --- Variáveis Globais ---
// Estas são as mesmas que você já tinha, elas continuarão sendo úteis.
extern TOKEN tk;
extern TOKEN lookahead; // Token de lookahead para prever o próximo token
extern FILE *fd;
extern int contLinha;

// Funções do analisador sintático para a linguagem Cshort

void prog();        // Regra inicial: prog ::= { decl ';' | func }

void decl();        // Regra: decl ::= tipo decl_var { ',' decl_var } | (protótipo de função)
void func();        // Regra: func ::= ( tipo | void ) id '(' tipos_param ')' '{' ... '}'

void decl_var();    // Regra: decl_var ::= id [ '[' intcon ']' ]
void tipo();        // Regra: tipo ::= char | int | float | bool
void tipos_param(); // Regra: tipos_param ::= void | tipo (id | &id | id['['']']) { ',' ... }

void cmd();         // Regra: cmd ::= if | while | for | return | atrib | chamada_func | bloco
void atrib();       // Regra: atrib ::= id [ '[' expr ']' ] = expr
// A chamada de função é um tipo de comando, podemos tratá-la dentro de cmd() ou fator()

void expr();        // Regra: expr ::= expr_simp [ op_rel expr_simp ]
void expr_simp();   // Regra: expr_simp ::= [ + | - ] termo { ( + | - | || ) termo }
void termo();       // Regra: termo ::= fator { ( * | / | && ) fator }
void fator();       // Regra: fator ::= id | intcon | realcon | charcon | chamada_func | '(' expr ')' | '!' fator
void op_rel();      // Regra: op_rel ::= == | != | <= | < | >= | >

#endif // ANASINT_CSHORT_H
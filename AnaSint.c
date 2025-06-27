#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "AnaLex.h"
#include "AnaSint.h" // Garanta que o nome do seu cabeçalho está correto
#include "FuncAuxCshort.h"

// --- Variáveis Globais para o Parser com Lookahead ---
TOKEN tk;        // O token ATUAL que estamos analisando
TOKEN lookahead; // O PRÓXIMO token, para "espiar" à frente

// --- Protótipos das funções do parser ---
void prog();
void declaracao_global();
void decl_var_resto(int tipo_declaracao);
void func_resto(int tipo_retorno);
void tipo();
void decl_var();
void tipos_param();
void cmd();
void atrib();
void expr();
void expr_simp();
void termo();
void fator();

// --- Funções de Controle de Token com Lookahead ---

// Avança para o próximo token: o lookahead vira o atual, e um novo lookahead é lido.
void proximo_token() {
    tk = lookahead;
    if (tk.cat != FIM_PROG) {
        lookahead = AnaLex(fd);
    }
}

// Consome o token esperado e avança para o próximo
void consome(int esperado) {
    if (tk.cat == esperado || tk.codigo == esperado) {
        proximo_token();
    } else {
        char errMsg[100];
        sprintf(errMsg, "Token inesperado na linha %d. Esperado: %d, Encontrado: %d (cat: %d)", contLinha, esperado, tk.codigo, tk.cat);
        errorSint(contLinha, errMsg);
    }
}

// --- Funções do Parser ---

// REGRA: prog ::= { declaracao_global }
void prog() {
    PrintNodo("Programa", AVANCA);
    // Inicializa o buffer com os dois primeiros tokens
    proximo_token();
    proximo_token();

    while (tk.cat != FIM_PROG) {
        declaracao_global();
    }
    PrintNodo("Fim do Programa", RETROCEDE);
    printf("\n[SUCESSO] Análise sintática concluída.\n");
}

// Função "despachante" que resolve a ambiguidade entre declaração e função
void declaracao_global() {
    int tipo_declaracao;

    if(tk.codigo == VOID) {
        tipo_declaracao = VOID;
        consome(VOID);
    } else {
        tipo_declaracao = tk.codigo;
        tipo();
    }
    
    consome(ID);

    // Agora, com o token ATUAL, podemos decidir com segurança
    if (tk.codigo == ABRE_PAR) {
        func_resto(tipo_declaracao);
    } else if (tk.codigo == VIRGULA || tk.codigo == PONTO_VIRGULA || tk.codigo == ABRE_COL) {
        decl_var_resto(tipo_declaracao);
    } else {
        errorSint(contLinha, "Esperado início de função '(...' ou de declaração de variável ';', ',' ou '['.");
    }
}

// Processa o resto de uma declaração de variável, já sabendo o tipo e o primeiro ID
void decl_var_resto(int tipo_declaracao) {
    PrintNodo("Declaracao de Variaveis", AVANCA);
    // O primeiro ID já foi consumido pelo dispatcher. Agora processamos o resto dele.
    if (tk.codigo == ABRE_COL) {
        consome(ABRE_COL);
        consome(CONST_INT);
        consome(FECHA_COL);
    }

    while (tk.codigo == VIRGULA) {
        consome(VIRGULA);
        decl_var();
    }
    consome(PONTO_VIRGULA);
    PrintNodo("Fim da Declaracao de Variaveis", RETROCEDE);
}

// Processa o resto de uma função, já sabendo o tipo de retorno e o nome
void func_resto(int tipo_retorno) {
    PrintNodo("Funcao", AVANCA);
    consome(ABRE_PAR);
    tipos_param();
    consome(FECHA_PAR);
    
    consome(ABRE_CHAVE);
    // Declarações locais
    while (tk.codigo == INT || tk.codigo == CHAR || tk.codigo == FLOAT || tk.codigo == BOOL) {
        decl();
    }
    // Comandos
    while (tk.codigo != FECHA_CHAVE && tk.cat != FIM_PROG) {
        cmd();
    }
    consome(FECHA_CHAVE);
    PrintNodo("Fim da Funcao", RETROCEDE);
}

// REGRA: decl ::= tipo decl_var { ',' decl_var } ';'  (usado para declarações locais)
void decl() {
    PrintNodo("Declaracao Local", AVANCA);
    tipo();
    decl_var();
    while (tk.codigo == VIRGULA) {
        consome(VIRGULA);
        decl_var();
    }
    consome(PONTO_VIRGULA);
    PrintNodo("Fim da Declaracao Local", RETROCEDE);
}

// REGRA: decl_var ::= id [ '[' intcon ']' ]
void decl_var() {
    PrintNodo("Variavel", AVANCA);
    consome(ID);
    if (tk.codigo == ABRE_COL) {
        consome(ABRE_COL);
        consome(CONST_INT);
        consome(FECHA_COL);
    }
    PrintNodo("Fim da Variavel", RETROCEDE);
}

// REGRA: tipo ::= char | int | float | bool
void tipo() {
    PrintNodo("Tipo", AVANCA);
    if (tk.codigo == CHAR || tk.codigo == INT || tk.codigo == FLOAT || tk.codigo == BOOL) {
        consome(tk.codigo);
    } else {
        errorSint(contLinha, "Tipo (int, char, float, bool) esperado.");
    }
    PrintNodo("Fim do Tipo", RETROCEDE);
}

// REGRA: tipos_param ::= void | tipo (id | &id | id['['']']) { ',' ... }
void tipos_param() {
    PrintNodo("Parametros", AVANCA);
    if (tk.codigo == VOID) {
        consome(VOID);
    } else if (tk.codigo == FECHA_PAR) {
        // Parâmetros vazios: func()
    }
    else {
        tipo();
        if (tk.codigo == REFERENCIA) consome(REFERENCIA);
        consome(ID);
        if (tk.codigo == ABRE_COL) {
            consome(ABRE_COL);
            consome(FECHA_COL);
        }
        while (tk.codigo == VIRGULA) {
            consome(VIRGULA);
            tipo();
            if (tk.codigo == REFERENCIA) consome(REFERENCIA);
            consome(ID);
            if (tk.codigo == ABRE_COL) {
                consome(ABRE_COL);
                consome(FECHA_COL);
            }
        }
    }
    PrintNodo("Fim dos Parametros", RETROCEDE);
}

// REGRA: cmd ::= if | while | for | return | atrib | chamada_func | bloco
// Em AnaSint.c, substitua estas funções:

// REGRA: cmd ::= if | while | for | return | atrib | chamada_func | bloco
void cmd() {
    PrintNodo("Comando", AVANCA);
    if (tk.codigo == IF) {
        consome(IF); // CORRIGIDO
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
        cmd();
        if (tk.codigo == ELSE) {
            consome(ELSE); // CORRIGIDO
            cmd();
        }
    } else if (tk.codigo == WHILE) {
        consome(WHILE); // CORRIGIDO
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
        cmd();
    } else if (tk.codigo == FOR) {
        consome(FOR); // CORRIGIDO
        consome(ABRE_PAR);
        if (tk.codigo != PONTO_VIRGULA) atrib();
        consome(PONTO_VIRGULA);
        if (tk.codigo != PONTO_VIRGULA) expr();
        consome(PONTO_VIRGULA);
        if (tk.codigo != FECHA_PAR) atrib();
        consome(FECHA_PAR);
        cmd();
    } else if (tk.codigo == RETURN) {
        consome(RETURN); // CORRIGIDO
        if (tk.codigo != PONTO_VIRGULA) expr();
        consome(PONTO_VIRGULA);
    } else if (tk.codigo == ABRE_CHAVE) {
        consome(ABRE_CHAVE);
        while (tk.codigo != FECHA_CHAVE && tk.cat != FIM_PROG) cmd();
        consome(FECHA_CHAVE);
    } else if (tk.cat == ID) {
        expr();
        consome(PONTO_VIRGULA);
    } else if (tk.codigo == PONTO_VIRGULA) {
        consome(PONTO_VIRGULA);
    } else {
        errorSint(contLinha, "Comando invalido.");
    }
    PrintNodo("Fim do Comando", RETROCEDE);
}

void atrib() {
    PrintNodo("Atribuicao", AVANCA);
    consome(ID);
    if (tk.codigo == ABRE_COL) {
        consome(ABRE_COL);
        expr();
        consome(FECHA_COL);
    }
    consome(ATRIBUICAO);
    expr();
    PrintNodo("Fim da Atribuicao", RETROCEDE);
}

// REGRA: expr ::= expr_simp [ op_rel expr_simp ]
void expr() {
    PrintNodo("Expressao", AVANCA);
    if (tk.cat == ID && lookahead.codigo == ATRIBUICAO) {
        atrib();
    } else {
        expr_simp();
        if (tk.cat == OP_RELAC) {
            consome(OP_RELAC); // CORRIGIDO
            expr_simp();
        }
    }
    PrintNodo("Fim da Expressao", RETROCEDE);
}

// REGRA: expr_simp ::= [ + | - ] termo { ( + | - | || ) termo }
void expr_simp() {
    PrintNodo("Expressao Simples", AVANCA);
    if (tk.codigo == ADICAO || tk.codigo == SUBTRACAO) {
        consome(tk.codigo); // CORRIGIDO
    }
    termo();
    while (tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR_LOGIC) {
        consome(tk.codigo); // CORRIGIDO
        termo();
    }
    PrintNodo("Fim da Expressao Simples", RETROCEDE);
}

// REGRA: termo ::= fator { ( * | / | && ) fator }
void termo() {
    PrintNodo("Termo", AVANCA);
    fator();
    while (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND_LOGIC) {
        consome(tk.codigo); // CORRIGIDO
        fator();
    }
    PrintNodo("Fim do Termo", RETROCEDE);
}

// REGRA: fator ::= id | id '[' expr ']' | id '(' [expr_list] ')' | constantes | '(' expr ')' | '!' fator
void fator() {
    PrintNodo("Fator", AVANCA);
    if (tk.cat == ID) {
        if (lookahead.codigo == ABRE_PAR) { // Chamada de função
            consome(ID);
            consome(ABRE_PAR);
            if (tk.codigo != FECHA_PAR) {
                expr();
                while (tk.codigo == VIRGULA) {
                    consome(VIRGULA);
                    expr();
                }
            }
            consome(FECHA_PAR);
        } else if (lookahead.codigo == ABRE_COL) { // Acesso a array
            consome(ID);
            consome(ABRE_COL);
            expr();
            consome(FECHA_COL);
        } else { // É apenas uma variável
            consome(ID);
        }
    } else if (tk.cat == CONST_INT) consome(CONST_INT);
    else if (tk.cat == CONST_FLOAT) consome(CONST_FLOAT);
    else if (tk.cat == CONST_CHAR) consome(CONST_CHAR);
    else if (tk.codigo == ABRE_PAR) {
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
    } else if (tk.codigo == NOT_LOGIC) {
        consome(NOT_LOGIC); // CORRIGIDO
        fator();
    } else {
        errorSint(contLinha, "Fator invalido.");
    }
    PrintNodo("Fim do Fator", RETROCEDE);
}
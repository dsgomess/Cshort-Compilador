#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "TabSimb.h"
#include "FuncAuxCshort.h"

TOKEN tk;
TOKEN lookahead;

void proximo_token() {
    tk = lookahead;
    if (tk.cat != FIM_PROG) {
        lookahead = AnaLex(fd);
    }
}

void consome(int esperado) {
    if (tk.cat == esperado || tk.val.codigo == esperado) {
        proximo_token();
    } else {
        char errMsg[100];
        sprintf(errMsg, "Token inesperado na linha %d. Esperado: %d, Encontrado: %d (cat: %d)", contLinha, esperado, tk.val.codigo, tk.cat);
        errorSint(contLinha, errMsg);
    }
}

void prog() {
    ts_inicializa();
    proximo_token();
    proximo_token();
    while (tk.cat != FIM_PROG) {
        declaracao_global();
    }
}

void declaracao_global() {
    int tipo_declaracao;
    char lexema_id[TAM_MAX_LEXEMA];
    if(tk.val.codigo == VOID) {
        tipo_declaracao = TIPO_NA;
        consome(VOID);
    } else {
        tipo_declaracao = tk.val.codigo;
        tipo();
    }
    strcpy(lexema_id, tk.val.lexema);
    consome(ID);
    if (tk.val.codigo == ABRE_PAR) {
        if(ts_insere(lexema_id, CLASSE_PROCEDIMENTO, tipo_declaracao) == NULL){
            errorSint(contLinha, "Procedimento re-declarado.");
        }
        func_resto(tipo_declaracao);
    } else {
        Simbolo* s = ts_insere(lexema_id, CLASSE_VAR_GLOBAL, tipo_declaracao);
        if(s == NULL) {
            errorSint(contLinha, "Variavel global re-declarada.");
        }
        if (tk.val.codigo == ABRE_COL) {
            consome(ABRE_COL);
            if(s) s->is_array = true;
            consome(CONST_INT);
            consome(FECHA_COL);
        }
        decl_var_resto(tipo_declaracao);
    }
}

void decl_var_resto(int tipo_declaracao) {
    while (tk.val.codigo == VIRGULA) {
        consome(VIRGULA);
        char lexema_id[TAM_MAX_LEXEMA];
        strcpy(lexema_id, tk.val.lexema);
        Simbolo* s = ts_insere(lexema_id, CLASSE_VAR_GLOBAL, tipo_declaracao);
        if(s == NULL){
            errorSint(contLinha, "Variavel global re-declarada.");
        }
        consome(ID);
        if (tk.val.codigo == ABRE_COL) {
            if(s) s->is_array = true;
            consome(ABRE_COL);
            consome(CONST_INT);
            consome(FECHA_COL);
        }
    }
    consome(PONTO_VIRGULA);
}

void func_resto(int tipo_retorno) {
    ts_entra_escopo();
    consome(ABRE_PAR);
    tipos_param();
    consome(FECHA_PAR);
    consome(ABRE_CHAVE);
    while (tk.val.codigo == INT || tk.val.codigo == CHAR || tk.val.codigo == FLOAT || tk.val.codigo == BOOL) {
        decl();
    }
    while (tk.val.codigo != FECHA_CHAVE && tk.cat != FIM_PROG) {
        cmd();
    }
    consome(FECHA_CHAVE);
    ts_sai_escopo();
}

void decl() {
    int tipo_atual = tk.val.codigo;
    tipo();
    char lexema_id[TAM_MAX_LEXEMA];
    strcpy(lexema_id, tk.val.lexema);
    Simbolo* s = ts_insere(lexema_id, CLASSE_VAR_LOCAL, tipo_atual);
    if(s == NULL) {
        errorSint(contLinha, "Variavel local re-declarada.");
    }
    decl_var();
    while (tk.val.codigo == VIRGULA) {
        consome(VIRGULA);
        strcpy(lexema_id, tk.val.lexema);
        s = ts_insere(lexema_id, CLASSE_VAR_LOCAL, tipo_atual);
        if(s == NULL) {
            errorSint(contLinha, "Variavel local re-declarada.");
        }
        decl_var();
    }
    consome(PONTO_VIRGULA);
}

void decl_var() {
    consome(ID);
    if (tk.val.codigo == ABRE_COL) {
        consome(ABRE_COL);
        consome(CONST_INT);
        consome(FECHA_COL);
    }
}

void tipo() {
    if (tk.val.codigo == CHAR || tk.val.codigo == INT || tk.val.codigo == FLOAT || tk.val.codigo == BOOL) {
        consome(tk.val.codigo);
    } else {
        errorSint(contLinha, "Tipo (int, char, float, bool) esperado.");
    }
}

void tipos_param() {
    if (tk.val.codigo == VOID) {
        consome(VOID);
        return;
    }
    while(tk.val.codigo == CHAR || tk.val.codigo == INT || tk.val.codigo == FLOAT || tk.val.codigo == BOOL){
        int tipo_param = tk.val.codigo;
        tipo();
        if (tk.val.codigo == REFERENCIA) consome(REFERENCIA);
        char lexema_param[TAM_MAX_LEXEMA];
        strcpy(lexema_param, tk.val.lexema);
        if(ts_insere(lexema_param, CLASSE_PARAMETRO, tipo_param) == NULL){
            errorSint(contLinha, "Parametro re-declarado.");
        }
        consome(ID);
        if(tk.val.codigo == ABRE_COL){
            consome(ABRE_COL);
            consome(FECHA_COL);
        }
        if(tk.val.codigo == VIRGULA){
            consome(VIRGULA);
        } else {
            break;
        }
    }
}

void cmd() {
    if (tk.cat == ID) {
        atrib();
        consome(PONTO_VIRGULA);
    } else if (tk.val.codigo == IF) {
        consome(IF); consome(ABRE_PAR); expr(); consome(FECHA_PAR); cmd();
        if (tk.val.codigo == ELSE) { consome(ELSE); cmd(); }
    } else if (tk.val.codigo == WHILE) {
        consome(WHILE); consome(ABRE_PAR); expr(); consome(FECHA_PAR); cmd();
    } else if (tk.val.codigo == ABRE_CHAVE) {
        consome(ABRE_CHAVE);
        while (tk.val.codigo != FECHA_CHAVE && tk.cat != FIM_PROG) { cmd(); }
        consome(FECHA_CHAVE);
    }
    // ... outras regras de comando ...
    else {
        // Comando vazio ou outro
        consome(PONTO_VIRGULA);
    }
}

void atrib() {
    char lexema_id[TAM_MAX_LEXEMA];
    if(ts_busca(tk.val.lexema) == NULL){
        char msg[100];
        sprintf(msg, "Variavel '%s' nao declarada.", tk.val.lexema);
        errorSint(contLinha, msg);
    }
    strcpy(lexema_id, tk.val.lexema);
    consome(ID);

    if (tk.val.codigo == ABRE_COL) {
        consome(ABRE_COL); expr(); consome(FECHA_COL);
    }
    consome(ATRIBUICAO);

    // Processa a expressão do lado direito, o que vai gerar o código
    // para calcular o valor e colocá-lo no topo da pilha.
    expr();

    // [GERADOR] Agora, emite a instrução para armazenar o valor
    // do topo da pilha na variável.
    printf("STORE %s\n", lexema_id);
}

void expr() {
    expr_simp();
    if (tk.cat == OP_RELAC) {
        // Lógica de geração de código para operadores relacionais (ex: JUMP_IF_FALSE)
        // seria adicionada aqui.
        int op = tk.val.codigo;
        consome(tk.cat);
        expr_simp();
        // [GERADOR] Exemplo para operadores relacionais
        if(op == IGUALDADE) emit("EQ");
        else if(op == DIFERENTE) emit("NE");
        else if(op == MENOR_QUE) emit("LT");
        else if(op == MAIOR_QUE) emit("GT");
        else if(op == MENOR_IGUAL) emit("LE");
        else if(op == MAIOR_IGUAL) emit("GE");

    }
}

void expr_simp() {
    termo();
    while (tk.val.codigo == ADICAO || tk.val.codigo == SUBTRACAO || tk.val.codigo == OR_LOGIC) {
        int op = tk.val.codigo;
        consome(op);
        termo();

        // [GERADOR] Emite a instrução APÓS processar o segundo operando.
        if (op == ADICAO) {
            emit("ADD");
        } else if (op == SUBTRACAO) {
            emit("SUB");
        } else if (op == OR_LOGIC) {
            emit("OR"); // Exemplo para operador lógico
        }
    }
}

void termo() {
    fator();
    while (tk.val.codigo == MULTIPLICACAO || tk.val.codigo == DIVISAO || tk.val.codigo == AND_LOGIC) {
        int op = tk.val.codigo;
        consome(op);
        fator();

        // [GERADOR] Emite a instrução APÓS processar o segundo operando.
        if (op == MULTIPLICACAO) {
            emit("MUL");
        } else if (op == DIVISAO) {
            emit("DIV");
        } else if (op == AND_LOGIC) {
            emit("AND"); // Exemplo para operador lógico
        }
    }
}

void fator() {
    if (tk.cat == ID) {
        char lexema_id[TAM_MAX_LEXEMA];
        strcpy(lexema_id, tk.val.lexema);
        if (ts_busca(lexema_id) == NULL) {
            char msg[100];
            sprintf(msg, "Identificador '%s' nao foi declarado.", lexema_id);
            errorSint(contLinha, msg);
        }

        if (lookahead.val.codigo == ABRE_PAR) { // É uma chamada de função
            // A geração de código para chamada de função seria aqui (ex: CALL <nome>, <n_params>)
            consome(ID);
            consome(ABRE_PAR);
            // ... processar argumentos ...
            consome(FECHA_PAR);
        } else { // É uma variável
            // [GERADOR] Emite LOAD para carregar o valor da variável para a pilha.
            printf("LOAD %s\n", lexema_id);
            consome(ID);
            if(tk.val.codigo == ABRE_COL) {
                // ... geração de código para acesso a array ...
                consome(ABRE_COL); expr(); consome(FECHA_COL);
            }
        }
    } else if (tk.cat == CONST_INT) {
        int valor = tk.val.valInt;
        // [GERADOR] Emite PUSH para colocar uma constante na pilha.
        emit_com_valor_int("PUSH", valor);
        consome(CONST_INT);
    } else if (tk.val.codigo == ABRE_PAR) {
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
    } else if (tk.val.codigo == NOT_LOGIC) {
        consome(NOT_LOGIC);
        fator();
        // [GERADOR] Emite a instrução de negação.
        emit("NOT");
    } else {
        errorSint(contLinha, "Fator invalido.");
    }
}
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

// Em AnaSint.c

void cmd() { // atualizao no cmd para os esquemas de traduçao while if if e as expr condicionais
    if (tk.val.codigo == IF) {
        consome(IF);
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);

        int label_else = novo_label(); 
        emit_salto("JUMP_IF_FALSE", label_else);

        cmd();

        if (tk.val.codigo == ELSE) {
            consome(ELSE);
            int label_saida = novo_label(); 
            emit_salto("JUMP", label_saida); 

            emit_label(label_else);
            cmd();

            emit_label(label_saida); 
        } else {
            emit_label(label_else); 
        }
    } else if (tk.val.codigo == WHILE) {
        consome(WHILE);

        int label_inicio = novo_label(); 
        int label_saida = novo_label();  

        emit_label(label_inicio);

        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
        
        emit_salto("JUMP_IF_FALSE", label_saida); 

        cmd();

        emit_salto("JUMP", label_inicio); 
        emit_label(label_saida); 

    } else if (tk.cat == ID) {
        atrib();
        consome(PONTO_VIRGULA);
    } else if (tk.val.codigo == ABRE_CHAVE) {
        consome(ABRE_CHAVE);
        while (tk.val.codigo != FECHA_CHAVE && tk.cat != FIM_PROG) { cmd(); }
        consome(FECHA_CHAVE);
    }
    // ... outras regras de comando (return, for, etc) ...
    else {
        consome(PONTO_VIRGULA); // Comando vazio
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

Tipo expr() { //void
    Tipo tipo_esq = expr_simp();
    if (tk.cat == OP_RELAC) {
        int op = tk.val.codigo;
        consome(tk.cat);
        Tipo tipo_dir = expr_simp();

        // [SEMÂNTICA] Verifica compatibilidade dos operandos.
        tipo_resultante(tipo_esq, tipo_dir, contLinha);
        
        // [GERADOR] Emite instrução relacional.
        if(op == IGUALDADE) emit("EQ");
        else if(op == DIFERENTE) emit("NE");
        else if(op == MENOR_QUE) emit("LT");
        else if(op == MAIOR_QUE) emit("GT");
        else if(op == MENOR_IGUAL) emit("LE");
        else if(op == MAIOR_IGUAL) emit("GE");

        // [SEMÂNTICA] O resultado de uma expressão relacional é sempre booleano.
        return TIPO_BOOL;
    }
    return tipo_esq;
}

Tipo expr_simp() {//void
    Tipo tipo_esq = termo();
    while (tk.val.codigo == ADICAO || tk.val.codigo == SUBTRACAO) {
        int op = tk.val.codigo;
        consome(op);
        Tipo tipo_dir = termo();

        // [SEMÂNTICA] Verifica compatibilidade e atualiza o tipo resultante.
        tipo_esq = tipo_resultante(tipo_esq, tipo_dir, contLinha);
        
        // [GERADOR] Emite instrução aritmética.
        if (op == ADICAO) emit("ADD");
        else emit("SUB");
    }
    return tipo_esq;
}

Tipo termo() { //void
    Tipo tipo_esq = fator();
    while (tk.val.codigo == MULTIPLICACAO || tk.val.codigo == DIVISAO) {
        int op = tk.val.codigo;
        consome(op);
        Tipo tipo_dir = fator();
        
        // [SEMÂNTICA] Verifica compatibilidade e atualiza o tipo resultante.
        tipo_esq = tipo_resultante(tipo_esq, tipo_dir, contLinha);

        // [GERADOR] Emite instrução aritmética.
        if (op == MULTIPLICACAO) emit("MUL");
        else emit("DIV");
    }
    return tipo_esq;
}

Tipo fator() { // void
    Tipo tipo_retorno = TIPO_NA;
    if (tk.cat == ID) {
        Simbolo* s = ts_busca(tk.val.lexema);
        if (s == NULL) {
            char msg[100];
            sprintf(msg, "Identificador '%s' nao foi declarado.", tk.val.lexema);
            errorSint(contLinha, msg);
        }
        tipo_retorno = s->tipo; // [SEMÂNTICA] Atributo 'tipo' vem da Tabela de Símbolos.
        
        // [GERADOR] Emite LOAD para carregar o valor da variável para a pilha.
        printf("LOAD %s\n", s->lexema);
        
        consome(ID);
        // ... Lógica de chamada de função e acesso a array ...
    } else if (tk.cat == CONST_INT) {
        // [GERADOR]
        emit_com_valor_int("PUSH", tk.val.valInt);
        // [SEMÂNTICA]
        tipo_retorno = TIPO_INT;
        consome(CONST_INT);
    } else if (tk.cat == CONST_FLOAT) {
        // [GERADOR]
        emit_com_valor_float("PUSH", tk.val.valFloat);
        // [SEMÂNTICA]
        tipo_retorno = TIPO_REAL;
        consome(CONST_FLOAT);
    } else if (tk.cat == CONST_CHAR) {
        // [GERADOR]
        emit_com_valor_int("PUSH", tk.val.caractere);
        // [SEMÂNTICA]
        tipo_retorno = TIPO_CHAR;
        consome(CONST_CHAR);
    } else if (tk.val.codigo == ABRE_PAR) {
        consome(ABRE_PAR);
        tipo_retorno = expr();
        consome(FECHA_PAR);
    } else {
        errorSint(contLinha, "Fator invalido.");
    }
    return tipo_retorno;
}
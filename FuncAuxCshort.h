// Em FuncAuxCshort.h

#ifndef FUNCAUXCSHORT_H
#define FUNCAUXCSHORT_H
#include "TabSimb.h" // para verificar a tabela de simbolos tipo
// nova adição
Tipo tipo_resultante(Tipo t1, Tipo t2,int linha);
void verifica_compat_atribuicao(Tipo tipo_var, Tipo tipo_expr, int linha);
void verifica_compat_booleana(Tipo t, int linha);

// Enum para controlar a indentação da árvore sintática
typedef enum {
    AVANCA,
    MANTEM,
    RETROCEDE
} OperacaoNodo;

// Protótipos das funções auxiliares

// Imprime uma mensagem de erro léxico e encerra
void errorLex(int linha, char c);

// Imprime uma mensagem de erro sintático e encerra
void errorSint(int linha, const char* msg); // <<< CORRIGIDO: usa 'const char*'

void errorSem(int linha, const char* msg);
// Imprime um nodo da árvore sintática (versão para strings)
void PrintNodo(const char* nome, OperacaoNodo op);

// Imprime um nodo da árvore sintática (versão para inteiros)
void PrintNodoInt(int valor, OperacaoNodo op);
// novo
void emit(const char* instrucao);
void emit_com_valor_int(const char* instrucao, int valor);
void emit_com_valor_float(const char* instrucao, float valor);

//parte da nova atividade
int novo_label();
void emit_label(int label);
void emit_salto(const char* intrucao, int label);

#endif // FUNCAUXCSHORT_H
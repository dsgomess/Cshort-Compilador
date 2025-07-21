// Em FuncAuxCshort.h

#ifndef FUNCAUXCSHORT_H
#define FUNCAUXCSHORT_H

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

// Imprime um nodo da árvore sintática (versão para strings)
void PrintNodo(const char* nome, OperacaoNodo op);

// Imprime um nodo da árvore sintática (versão para inteiros)
void PrintNodoInt(int valor, OperacaoNodo op);
// novo
void emit(const char* instrucao);
void emit_com_valor_int(const char* instrucao, int valor);

#endif // FUNCAUXCSHORT_H
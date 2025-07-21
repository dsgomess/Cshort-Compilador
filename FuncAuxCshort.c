#include <stdio.h>
#include <stdlib.h>
#include "FuncAuxCshort.h" // Inclui o seu próprio cabeçalho com os protótipos

// --- Implementação das Funções de Erro ---

// Imprime uma mensagem de erro léxico e encerra a compilação.
void errorLex(int linha, char c) {
    fprintf(stderr, "[Erro Léxico] Caractere '%c' inválido na linha: %d\n", c, linha);
    exit(EXIT_FAILURE); // Encerra o programa por causa do erro fatal.
}

// Imprime uma mensagem de erro sintático e encerra a compilação.
void errorSint(int linha, const char* msg) {
    fprintf(stderr, "[Erro Sintático] %s na linha: %d\n", msg, linha);
    exit(EXIT_FAILURE); // Encerra o programa por causa do erro fatal.
}


// --- Implementação das Funções de Impressão da Árvore ---

// Variável estática para controlar o nível de indentação da árvore.
static int nivel_indentacao = 0;

// Imprime um nodo da árvore (versão para strings)
void PrintNodo(const char* nome, OperacaoNodo op) {
    if (op == RETROCEDE) {
        nivel_indentacao--;
    }

    for (int i = 0; i < nivel_indentacao; i++) {
        printf("  ");
    }

    printf("%s\n", nome);

    if (op == AVANCA) {
        nivel_indentacao++;
    }
}

// Imprime um nodo da árvore (versão para inteiros)
void PrintNodoInt(int valor, OperacaoNodo op) {
    if (op == RETROCEDE) {
        nivel_indentacao--;
    }

    for (int i = 0; i < nivel_indentacao; i++) {
        printf("  ");
    }

    printf("%d\n", valor);

    if (op == AVANCA) {
        nivel_indentacao++;
    }
} // <<< A CHAVE DE FECHAMENTO ESTAVA FALTANDO AQUI

// --- Implementação das Funções do Gerador de Código ---

// Emite uma instrução sem argumento
void emit(const char* instrucao) {
    printf("%s\n", instrucao);
}

// Emite uma instrução com um argumento inteiro
void emit_com_valor_int(const char* instrucao, int valor) {
    printf("%s %d\n", instrucao, valor);
}
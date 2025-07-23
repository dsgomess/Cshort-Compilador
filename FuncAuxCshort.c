#include <stdio.h>
#include <stdlib.h>
#include "FuncAuxCshort.h" // Inclui o seu próprio cabeçalho com os protótipos
#include "TabSimb.h" // verificar tabela tipo

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
void errorSem(int linha, const char* msg) {
    fprintf(stderr, "[Erro Semantico] %s na linha: %d\n", msg, linha);
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

// Emite uma instrução com um argumento float (ex: PUSH 3.14)
void emit_com_valor_float(const char* instrucao, float valor) {
    printf("%s %f\n", instrucao, valor);
}
// nova atividade
static int proximo_label = 0;

// Retorna um número de label único.
int novo_label() {
    return proximo_label++;
}

// Emite uma instrução de label no formato "L<numero>:"
void emit_label(int label) {
    printf("L%d:\n", label);
}

// Emite uma instrução de salto no formato "INSTRUCAO L<numero>"
void emit_salto(const char* instrucao, int label) {
    printf("%s L%d\n", instrucao, label);
}
Tipo tipo_resultante(Tipo t1, Tipo t2, int linha) {
    if ((t1 == TIPO_INT || t1 == TIPO_CHAR) && (t2 == TIPO_INT || t2 == TIPO_CHAR)) {
        return TIPO_INT; // int e char são compatíveis, e o resultado é promovido para int[cite: 373, 394].
    }
    if (t1 == TIPO_REAL && t2 == TIPO_REAL) {
        return TIPO_REAL;
    }
    if (t1 == TIPO_BOOL || t2 == TIPO_BOOL) {
        errorSem(linha, "Tipo 'bool' invalido para operacao aritmetica.");
    }
    // Cshort não permite operações entre int/char e float[cite: 395].
    errorSem(linha, "Tipos incompativeis para operacao aritmetica.");
    return TIPO_NA; // Não deve chegar aqui
}

// Verifica se a atribuição é válida.
// Baseado na regra 3.3.3.1 da especificação Cshort.
void verifica_compat_atribuicao(Tipo tipo_var, Tipo tipo_expr, int linha) {
    if (tipo_var == tipo_expr) return; // Tipos iguais são sempre compatíveis.

    // int é compatível com char e vice-versa[cite: 373, 406].
    if ((tipo_var == TIPO_INT && tipo_expr == TIPO_CHAR) || (tipo_var == TIPO_CHAR && tipo_expr == TIPO_INT)) {
        return;
    }
    
    // Expressão relacional (bool) é compatível com int[cite: 376].
    if (tipo_var == TIPO_INT && tipo_expr == TIPO_BOOL) {
        return;
    }

    errorSem(linha, "Tipos incompativeis para atribuicao.");
}

// Verifica se o tipo é compatível com operações lógicas/condicionais.
// Baseado na regra 3.3.3.3 e 3.2.2.7 da especificação Cshort.
void verifica_compat_booleana(Tipo t, int linha) {
    if (t != TIPO_BOOL) {
        errorSem(linha, "Expressao condicional deve ser do tipo 'bool'.");
    }
}
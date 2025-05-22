#include <stdio.h>
#include <stdlib.h>
#include "Analex.h"
#include "FuncAuxCshort.h"

FILE *fd;
TOKEN tk;


// Função para imprimir as informações de cada token
void printToken(TOKEN tk) {
    printf("Token -> Categoria: %d | ", tk.cat);

    switch (tk.cat) {
        case ID:
            printf("ID: %s\n", tk.lexema);
            break;
        case PAL_RESERV:
            printf("Palavra Reservada (código): %d\n", tk.codigo);
            break;
        case CONST_INT:
            printf("Constante Inteira: %d\n", tk.valInt);
            break;
        case CONST_FLOAT:
            printf("Constante Real: %f\n", tk.valFloat);
            break;
        case CONST_CHAR:
            printf("Constante Char: '%c'\n", tk.caractere);
            break;
        case LITERAL:
            printf("Literal (string): \"%s\"\n", tk.lexema);
            break;
        case OP_ARIT:
            printf("Operador Aritmético (código): %d\n", tk.codigo);
            break;
        case OP_RELAC:
            printf("Operador Relacional (código): %d\n", tk.codigo);
            break;
        case OP_LOGIC:
            printf("Operador Lógico (código): %d\n", tk.codigo);
            break;
        case SINAL:
            printf("Sinal (código): %d\n", tk.codigo);
            break;
        case FIM_PROG:
            printf("Fim de Programa.\n");
            break;
        default:
            printf("Token não reconhecido.\n");
            break;
    }
}

int main() {
    fd = fopen("teste.cshort", "r");

    if (fd == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo 'teste.cshort'.\n");
        return 1;
    }

    printf("[========== INICIO - Analise Lexica ==========]\n");


    do {
        tk = AnaLex(fd);
        printToken(tk);
    } while (tk.cat != FIM_PROG);

    printf("[========== FIM - Analise Lexica ==========]\n");

    fclose(fd);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "Analex.h"
#include "FuncAuxCshort.h"
#include "AnaSint.h"


TOKEN tk;
TOKEN lookahead;
FILE *fd;

// Função para imprimir as informações de cada token
void printToken(TOKEN tk) {
    // Primeira etapa: verificar a categoria
    switch (tk.cat) {
        case ID:
            printf("ID : %s\n", tk.lexema);
            break;

        case PAL_RESERV:
            printf("Palavra Reservada ");
            switch (tk.codigo) {
                case 1: printf(": void\n"); break;
                case 2: printf(": char\n"); break;
                case 3: printf(": int\n"); break;
                case 4: printf(": float\n"); break;
                case 5: printf(": bool\n"); break;
                case 6: printf(": if\n"); break;
                case 7: printf(": else\n"); break;
                case 8: printf(": while\n"); break;
                case 9: printf(": for\n"); break;
                case 10: printf(": return\n"); break;
                default: printf(": Desconhecida\n"); break;
            }
            break;

        case SINAL:
            printf("Sinal ");
            switch (tk.codigo) {
                case 1: printf(": (\n"); break;
                case 2: printf(": )\n"); break;
                case 3: printf(": {\n"); break;
                case 4: printf(": }\n"); break;
                case 5: printf(": [\n"); break;
                case 6: printf(": ]\n"); break;
                case 7: printf(": ,\n"); break;
                case 8: printf(": ;\n"); break;
                case 9: printf(": &\n"); break;
                default: printf(": Desconhecido\n"); break;
            }
            break;

        case CONST_CHAR:
            printf("Constante Char : '%c'\n", tk.caractere);
            break;

        case CONST_INT:
            printf("Constante Inteira : %d\n", tk.valInt);
            break;

        case CONST_FLOAT:
            printf("Constante Real : %f\n", tk.valFloat);
            break;

        case LITERAL:
            printf("Literal : \"%s\"\n", tk.lexema);
            break;

        case OP_ARIT:
            printf("Operador Aritmético ");
            switch (tk.codigo) {
                case 1: printf(": =\n"); break;
                case 2: printf(": +\n"); break;
                case 3: printf(": -\n"); break;
                case 4: printf(": *\n"); break;
                case 5: printf(": /\n"); break;
                default: printf(": Desconhecido\n"); break;
            }
            break;

        case OP_RELAC:
            printf("Operador Relacional ");
            switch (tk.codigo) {
                case 1: printf(": ==\n"); break;
                case 2: printf(": !=\n"); break;
                case 3: printf(": <=\n"); break;
                case 4: printf(": >=\n"); break;
                case 5: printf(": <\n"); break;
                case 6: printf(": >\n"); break;
                default: printf(": Desconhecido\n"); break;
            }
            break;

        case OP_LOGIC:
            printf("Operador Lógico ");
            switch (tk.codigo) {
                case 1: printf(": &&\n"); break;
                case 2: printf(": ||\n"); break;
                case 3: printf(": !\n"); break;
                default: printf(": Desconhecido\n"); break;
            }
            break;

        case FIM_PROG:
            printf("Fim de Programa.\n");
            break;

        default:
            printf("Token desconhecido.\n");
            break;
    }
}


int main() {
    // Abrir o arquivo de teste
    fd = fopen("teste.cshort", "r");
    if (fd == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo 'teste.cshort'.\n");
        return 1;
    }

    // --- ETAPA 1: ANÁLISE LÉXICA PURA ---
    // Vamos imprimir todos os tokens que o léxico gera, para inspeção.
    printf("\n[--- ETAPA 1: Verificando Saída do Analisador Léxico ---]\n\n");
    do {
        tk = AnaLex(fd);
        printToken(tk);
    } while (tk.cat != FIM_PROG);
    printf("\n[--- FIM DA ETAPA 1 ---]\n\n");


    // --- ETAPA 2: ANÁLISE SINTÁTICA ---
    printf("[--- ETAPA 2: Executando Análise Sintática ---]\n\n");
    
    // VOLTA O PONTEIRO DE LEITURA PARA O INÍCIO DO ARQUIVO
    rewind(fd); 
    
    // Chama o parser para analisar do zero
    prog();

    // Fecha o arquivo no final de tudo
    fclose(fd);
    return 0;
}
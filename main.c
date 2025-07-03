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


int main(int argc, char *argv[]) {

    // 1. Verifica se o usuário digitou o nome do arquivo ao executar
    if (argc < 2) {
        printf("Erro: Faltou o nome do arquivo de entrada.\n");
        printf("Modo de usar: .\\lexmain nome_do_arquivo.cshort\n");
        exit(1); // Encerra o programa se não houver nome de arquivo
    }

    // 2. Tenta abrir o arquivo que o usuário digitou (argv[1])
    fd = fopen(argv[1], "r"); 

    // 3. Verifica se o arquivo realmente existe e pôde ser aberto
    if (fd == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", argv[1]);
        exit(1);
    }

    // 4. Se tudo deu certo, chama o seu analisador sintático
    prog(); 

    // 5. Fecha o arquivo no final
    fclose(fd);
    return 0;
}
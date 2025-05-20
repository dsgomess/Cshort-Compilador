#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FuncAuxCshort.h"

// Variável global usada para indentação em PrintNodo e PrintNodoInt
char TABS[50] = "";

void errorSint(int contLinha, char caracter[])
{
    printf("\n[Erro Sintático] Caractere '%s' inválido na linha: %d\n", caracter, contLinha);
    exit(1);
}

void errorLex(int contLinha, char caracter)
{
    char c_str[2];
    c_str[0] = caracter;
    c_str[1] = '\0';
    printf("\n[Erro Léxico] Caractere '%s' inválido na linha: %d\n", c_str, contLinha);
    exit(1);
}

void PrintNodo(char info[], int movim)
{
    if (movim == AVANCA)
    {
        printf("%s%s\n", TABS, info);
        strcat(TABS, "\t");
    }
    else if (movim == MANTEM)
    {
        printf("%s%s\n", TABS, info);
    }
    else if (movim == RETROCEDE)
    {
        TABS[strlen(TABS) - 1] = '\0';
    }
}

void PrintNodoInt(int val, int movim)
{
    if (movim == AVANCA)
    {
        printf("%s%d\n", TABS, val);
        strcat(TABS, "\t");
    }
    else if (movim == MANTEM)
    {
        printf("%s%d\n", TABS, val);
    }
    else if (movim == RETROCEDE)
    {
        TABS[strlen(TABS) - 1] = '\0';
    }
}

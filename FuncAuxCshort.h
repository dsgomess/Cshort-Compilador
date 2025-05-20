#ifndef FUNCAUXCSHORT_H
#define FUNCAUXCSHORT_H

// Constantes para indentação usada nas funções PrintNodo
#define AVANCA 1
#define MANTEM 2
#define RETROCEDE 3

// Declaração da string de indentação (definida em FuncAuxCshort.c)
extern char TABS[50];

// Funções auxiliares
void errorSint(int contLinha, char caracter[]);
void errorLex(int contLinha, char caracter);

void PrintNodo(char info[], int movim);
void PrintNodoInt(int val, int movim);

#endif

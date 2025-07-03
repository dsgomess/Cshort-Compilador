#ifndef TABSIMB_H // É uma boa prática adicionar um _H ao final do nome do #define
#define TABSIMB_H

#include <stdio.h>
#include <stdlib.h>
#include "AnaLex.h"
#include <stdbool.h>

#define TAM_MAX_TAB 100
#define TAM_CATEGORIA 4

// Definição da estrutura TabSimb
typedef struct
{
    char lexema[TAM_MAX_LEXEMA];
    int tipo;
    int escopo;
    char classe[TAM_CATEGORIA];     
    bool zombie;
    
} TabSimb;

// Definição da estrutura TabIdef
typedef struct
{
    int tamTab;
    TabSimb tabela_simb[TAM_MAX_TAB];
} TabIdef;

// Declaração de variáveis externas
extern TabIdef tabela_idef;
extern TOKEN tk;
extern int escopoAtual;

// Declaração de funções
void Iniciar_tabela();
void Imprimir_tabela();
int Buscar_tabela(const char lexema[]);
int Buscar_escopo(char lexema[], int escopo);
// ALTERADO: O parâmetro 'categoria' foi renomeado para 'classe'
int Insercao_tabela(char lexema[], int escopo, int tipo, char classe[TAM_CATEGORIA], bool zombie);
int Remover_ultimo();

#endif
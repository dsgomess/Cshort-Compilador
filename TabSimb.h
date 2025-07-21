#ifndef TABSIMB_H
#define TABSIMB_H

#include <stdbool.h>

// -- Constantes e Macros --
#define TAM_MAX_LEXEMA 50
#define MAX_TABELA_HASH 512

// -- Estruturas e Enums --

// Escopo: indica o escopo no qual o identificador é definido.
typedef enum {
    ESCOPO_GLOBAL = 0, // (GLB) externo a um procedimento
    ESCOPO_LOCAL  = 1  // (LCL) interno a um procedimento
} Escopo;

// Tipo: tipo declarado ou implícito de um identificador.
typedef enum {
    TIPO_INT  = 0, // (INT)
    TIPO_REAL = 1, // (REAL)
    TIPO_CHAR = 2, // (CHAR)
    TIPO_BOOL = 3, // (BOOL)
    TIPO_NA   = 9  // (X) N/A
} Tipo;

// Classe (ou Categoria): indica a categoria do identificador.
typedef enum {
    CLASSE_VAR_GLOBAL     = 0, // (VG)
    CLASSE_VAR_LOCAL      = 1, // (VL)
    CLASSE_PROCEDIMENTO   = 2, // (PRC)
    CLASSE_PARAMETRO      = 3, // (PAR)
    CLASSE_PROTOTIPO      = 4  // (PRT)
} Classe;

// Zumbi: indica que um parâmetro não está mais "vivo".
typedef enum {
    ZOMBIE_VIVO  = 0, // (VIV) está vivo
    ZOMBIE_MORTO = 1, // (ZMB) virou zumbi
    ZOMBIE_NA    = 9  // (X) N/A
} Zombie;

// A entrada principal da Tabela de Símbolos
typedef struct Simbolo {
    char lexema[TAM_MAX_LEXEMA];
    Escopo escopo;
    Classe classe;
    Tipo tipo;
    Zombie zombie;
    int nivel;

    bool is_array; // <<< ADICIONE ESTA LINHA

    // Ponteiro para o próximo símbolo em caso de colisão na hash
    struct Simbolo *proximo;
} Simbolo;


// --- API da Tabela de Símbolos ---

// Inicializa a tabela de símbolos e o controle de escopo.
void ts_inicializa();

// Sinaliza a entrada em um novo escopo aninhado.
void ts_entra_escopo();

// Sinaliza a saída de um escopo, marcando símbolos locais e params como "zumbis".
void ts_sai_escopo();

// Insere um novo símbolo na tabela. Retorna ponteiro para o símbolo ou NULL se houver erro.
Simbolo* ts_insere(char* lexema, Classe classe, Tipo tipo);

// Busca por um símbolo. Retorna o mais próximo no escopo ou NULL.
Simbolo* ts_busca(char* lexema);

// Imprime toda a tabela de símbolos (para depuração).
void ts_imprime();

#endif // TABSIMB_H
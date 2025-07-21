#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TabSimb.h"

// Tabela de Símbolos Global (única)
static Simbolo* tabela_hash[MAX_TABELA_HASH];
// Contador para o nível de escopo atual
static int nivel_atual = 0;

// Função de Hash
static unsigned int hash(char *lexema) {
    unsigned int hash_val = 0;
    while (*lexema != '\0') {
        hash_val = 31 * hash_val + *lexema++;
    }
    return hash_val % MAX_TABELA_HASH;
}

void ts_inicializa() {
    nivel_atual = 0;
    for (int i = 0; i < MAX_TABELA_HASH; i++) {
        tabela_hash[i] = NULL;
    }
    printf("[TABELA DE SÍMBOLOS] Tabela inicializada. Escopo global (nível 0) ativo.\n");
}

void ts_entra_escopo() {
    nivel_atual++;
    printf("[TABELA DE SÍMBOLOS] Entrou no escopo de nível: %d\n", nivel_atual);
}

void ts_sai_escopo() {
    printf("[TABELA DE SÍMBOLOS] Saindo do escopo de nível: %d\n", nivel_atual);
    if (nivel_atual == 0) return; // Não pode sair do escopo global

    // Marca os símbolos do nível atual como "zumbis"
    for (int i = 0; i < MAX_TABELA_HASH; i++) {
        Simbolo* s = tabela_hash[i];
        while (s != NULL) {
            if (s->nivel == nivel_atual) {
                // Parâmetros e variáveis locais ficam "mortos" (zumbis)
                // para não serem encontrados em futuras buscas de escopo.
                s->zombie = ZOMBIE_MORTO;
            }
            s = s->proximo;
        }
    }
    nivel_atual--;
}

Simbolo* ts_insere(char* lexema, Classe classe, Tipo tipo) {
    unsigned int indice = hash(lexema);

    // Verifica se já existe um símbolo com mesmo lexema NO MESMO NÍVEL
    Simbolo* s = tabela_hash[indice];
    while (s != NULL) {
        if (strcmp(lexema, s->lexema) == 0 && s->nivel == nivel_atual) {
            // Erro: Re-declaração no mesmo escopo/nível
            return NULL;
        }
        s = s->proximo;
    }

    // Cria e preenche o novo símbolo
    Simbolo* novo = (Simbolo*) malloc(sizeof(Simbolo));
    strcpy(novo->lexema, lexema);
    novo->classe = classe;
    novo->tipo = tipo;
    novo->nivel = nivel_atual;
    novo->escopo = (nivel_atual == 0) ? ESCOPO_GLOBAL : ESCOPO_LOCAL;

    novo->is_array = false; // Inicializa como não-array
    
    // Define o estado zumbi inicial
    if (classe == CLASSE_VAR_GLOBAL || classe == CLASSE_PROCEDIMENTO || classe == CLASSE_PROTOTIPO) {
        novo->zombie = ZOMBIE_NA;
    } else {
        novo->zombie = ZOMBIE_VIVO;
    }
    
    // Insere na tabela
    novo->proximo = tabela_hash[indice];
    tabela_hash[indice] = novo;

    printf("[TABELA DE SÍMBOLOS] Inseriu '%s' (classe: %d, nível: %d)\n", lexema, classe, nivel_atual);
    return novo;
}

Simbolo* ts_busca(char* lexema) {
    unsigned int indice = hash(lexema);
    Simbolo* candidato = NULL;

    Simbolo* s = tabela_hash[indice];
    while (s != NULL) {
        if (strcmp(lexema, s->lexema) == 0) {
            // Encontrou um símbolo com o mesmo nome.
            // Agora, verifica se ele é acessível do escopo atual.
            if (s->nivel <= nivel_atual && s->zombie != ZOMBIE_MORTO) {
                // Se o candidato ainda não foi encontrado, ou se este símbolo
                // é de um escopo mais interno (nível maior) que o candidato anterior,
                // ele se torna o novo candidato.
                if (candidato == NULL || s->nivel > candidato->nivel) {
                    candidato = s;
                }
            }
        }
        s = s->proximo;
    }

    // Retorna o melhor candidato encontrado (o do escopo mais próximo)
    return candidato;
}

void ts_imprime() {
    printf("\n--- CONTEÚDO COMPLETO DA TABELA DE SÍMBOLOS ---\n");
    for (int i = 0; i < MAX_TABELA_HASH; i++) {
        Simbolo* s = tabela_hash[i];
        if (s) {
            while (s != NULL) {
                printf("Lexema: '%-15s' | Nível: %d | Escopo: %d | Classe: %d | Tipo: %d | Zombie: %d\n", 
                       s->lexema, s->nivel, s->escopo, s->classe, s->tipo, s->zombie);
                s = s->proximo;
            }
        }
    }
    printf("----------------------------------------------\n\n");
}
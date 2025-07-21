#ifndef ANASINT_H
#define ANASINT_H

#include <stdio.h>
#include "AnaLex.h"

// --- Variáveis Globais (usadas em múltiplos arquivos) ---
extern TOKEN tk;
extern TOKEN lookahead;
extern FILE *fd;
extern int contLinha;

// --- Protótipos das Funções do Analisador Sintático ---
void prog();
void declaracao_global();
void decl_var_resto(int tipo_declaracao);
void func_resto(int tipo_retorno);
void decl();
void decl_var();
void tipo();
void tipos_param();
void cmd();
void atrib();
void expr();
void expr_simp();
void termo();
void fator();

#endif // ANASINT_H
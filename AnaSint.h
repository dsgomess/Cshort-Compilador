#ifndef ANASINT_H
#define ANASINT_H

#include <stdio.h>
#include "AnaLex.h"
#include "TabSimb.h"

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
Tipo expr(); // void
Tipo expr_simp(); //void
Tipo termo();//void
Tipo fator();//void

#endif // ANASINT_H
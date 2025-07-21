#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "AnaLex.h"
#include "FuncAuxCshort.h"

// Variáveis globais (se ainda precisar delas fora desta função)
int contLinha = 1;

//---------------------------------------------------------//
//                      FUNÇÃO ANALEX                      //
//---------------------------------------------------------//
TOKEN AnaLex(FILE *fd)
{
    int estado = 0;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_MAX_LEXEMA] = ""; 
    char digitos[TAM_NUM] = "";       
    char c;
    TOKEN token;

    while (1) {
        c = fgetc(fd);

        switch (estado) {
            case 0: // ESTADO INICIAL
                if (c == ' ' || c == '\t') {}
                else if (c == '\n') { contLinha++; }
                else if (c == '/') { estado = 18; }
                else if (c == '\'') { estado = 9; }
                else if (c == '\"') { estado = 15; }
                else if (isalpha(c)) { tamL = 0; lexema[tamL++] = c; lexema[tamL] = '\0'; estado = 1; }
                else if (isdigit(c)) { tamD = 0; digitos[tamD++] = c; digitos[tamD] = '\0'; estado = 4; }
                else if (c == '=') { estado = 21; }
                else if (c == '!') { estado = 24; }
                else if (c == '<') { estado = 27; }
                else if (c == '>') { estado = 30; }
                else if (c == '&') { estado = 39; }
                else if (c == '|') { estado = 42; }
                else if (c == '+') { token.cat = OP_ARIT; token.val.codigo = ADICAO; return token; }
                else if (c == '-') { token.cat = OP_ARIT; token.val.codigo = SUBTRACAO; return token; }
                else if (c == '*') { token.cat = OP_ARIT; token.val.codigo = MULTIPLICACAO; return token; }
                else if (c == '{') { token.cat = SINAL; token.val.codigo = ABRE_CHAVE; return token; }
                else if (c == '}') { token.cat = SINAL; token.val.codigo = FECHA_CHAVE; return token; }
                else if (c == '(') { token.cat = SINAL; token.val.codigo = ABRE_PAR; return token; }
                else if (c == ')') { token.cat = SINAL; token.val.codigo = FECHA_PAR; return token; }
                else if (c == '[') { token.cat = SINAL; token.val.codigo = ABRE_COL; return token; }
                else if (c == ']') { token.cat = SINAL; token.val.codigo = FECHA_COL; return token; }
                else if (c == ',') { token.cat = SINAL; token.val.codigo = VIRGULA; return token; }
                else if (c == ';') { token.cat = SINAL; token.val.codigo = PONTO_VIRGULA; return token; }
                else if (c == EOF) { token.cat = FIM_PROG; return token; }
                else { errorLex(contLinha, c); }
                break;
            
            case 1:
                if (isalnum(c) || c == '_') {
                    if (tamL < TAM_MAX_LEXEMA - 1) { lexema[tamL++] = c; }
                } else {
                    lexema[tamL] = '\0';
                    ungetc(c, fd);
                    if (strcmp("void", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = VOID; }
                    else if (strcmp("char", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = CHAR; }
                    else if (strcmp("int", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = INT; }
                    else if (strcmp("float", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = FLOAT; }
                    else if (strcmp("bool", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = BOOL; }
                    else if (strcmp("if", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = IF; }
                    else if (strcmp("else", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = ELSE; }
                    else if (strcmp("while", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = WHILE; }
                    else if (strcmp("for", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = FOR; }
                    else if (strcmp("return", lexema) == 0) { token.cat = PAL_RESERV; token.val.codigo = RETURN; }
                    else { token.cat = ID; strcpy(token.val.lexema, lexema); }
                    return token;
                }
                break;

            case 4:
                if (isdigit(c)) { if (tamD < TAM_NUM - 1) digitos[tamD++] = c; }
                else if (c == '.') { if (tamD < TAM_NUM - 1) { digitos[tamD++] = c; estado = 6; } }
                else {
                    ungetc(c, fd);
                    token.cat = CONST_INT;
                    token.val.valInt = atoi(digitos);
                    return token;
                }
                break;
            case 6:
                if (isdigit(c)) { if (tamD < TAM_NUM - 1) { digitos[tamD++] = c; estado = 7; } }
                else { errorLex(contLinha, c); }
                break;
            case 7:
                if (isdigit(c)) { if (tamD < TAM_NUM - 1) digitos[tamD++] = c; }
                else {
                    ungetc(c, fd);
                    token.cat = CONST_FLOAT;
                    token.val.valFloat = atof(digitos);
                    return token;
                }
                break;

            case 9:
                if (c == '\\') { estado = 12; }
                else if (isprint(c) && c != '\'') { estado = 10; token.val.caractere = c; }
                else { errorLex(contLinha, c); }
                break;
            case 10:
                if (c == '\'') { token.cat = CONST_CHAR; return token; }
                else { errorLex(contLinha, c); }
                break;
            case 12:
                if (c == 'n') { token.val.caractere = '\n'; estado = 13; }
                else if (c == '0') { token.val.caractere = '\0'; estado = 13; }
                else if (c == 't') { token.val.caractere = '\t'; estado = 13; }
                else { errorLex(contLinha, c); }
                break;
            case 13:
                if (c == '\'') { token.cat = CONST_CHAR; return token; }
                else { errorLex(contLinha, c); }
                break;
            
            case 15:
                if (c == '\"') {
                    lexema[tamL] = '\0';
                    token.cat = LITERAL;
                    strcpy(token.val.lexema, lexema);
                    return token;
                } else if (c == '\n' || c == EOF) { errorLex(contLinha, c); }
                else if (isprint(c)) { if (tamL < TAM_MAX_LEXEMA - 1) lexema[tamL++] = c; }
                else { errorLex(contLinha, c); }
                break;
            
            case 18:
                if (c == '*') { estado = 19; }
                else { ungetc(c, fd); token.cat = OP_ARIT; token.val.codigo = DIVISAO; return token; }
                break;
            case 19:
                if (c == '*') { estado = 20; }
                else if (c == '\n') { contLinha++; }
                else if (c == EOF) { errorLex(contLinha, c); }
                break;
            case 20:
                if (c == '/') { estado = 0; }
                else if (c == '*') {}
                else if (c == EOF) { errorLex(contLinha, c); }
                else { estado = 19; }
                break;
            
            case 21:
                if (c == '=') { token.cat = OP_RELAC; token.val.codigo = IGUALDADE; return token; }
                else { ungetc(c, fd); token.cat = OP_ARIT; token.val.codigo = ATRIBUICAO; return token; }
                break;
            case 24:
                if (c == '=') { token.cat = OP_RELAC; token.val.codigo = DIFERENTE; return token; }
                else { ungetc(c, fd); token.cat = OP_LOGIC; token.val.codigo = NOT_LOGIC; return token; }
                break;
            case 27:
                if (c == '=') { token.cat = OP_RELAC; token.val.codigo = MENOR_IGUAL; return token; }
                else { ungetc(c, fd); token.cat = OP_RELAC; token.val.codigo = MENOR_QUE; return token; }
                break;
            case 30:
                if (c == '=') { token.cat = OP_RELAC; token.val.codigo = MAIOR_IGUAL; return token; }
                else { ungetc(c, fd); token.cat = OP_RELAC; token.val.codigo = MAIOR_QUE; return token; }
                break;
            case 39:
                if (c == '&') { token.cat = OP_LOGIC; token.val.codigo = AND_LOGIC; return token; }
                else { ungetc(c, fd); token.cat = SINAL; token.val.codigo = REFERENCIA; return token; }
                break;
            case 42:
                if (c == '|') { token.cat = OP_LOGIC; token.val.codigo = OR_LOGIC; return token; }
                else { errorLex(contLinha, c); }
                break;
        }
    }
}
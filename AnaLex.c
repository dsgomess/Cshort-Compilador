// Inclua o AnaLex.h corretamente
#include "AnaLex.h"

// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// REMOVA OS DEFINES DAQUI! ELES DEVEM ESTAR NO AnaLex.h
// #define TAM_LEXEMA 50
// #define TAM_LITERAL 50
// #define TAM_NUM 20

// Variáveis globais (se ainda precisar delas fora desta função)
int contLinha = 1;

//---------------------------------------------------------//
//             FUNÇÃO ANALEX - VERSÃO CORRIGIDA            //
//---------------------------------------------------------//
TOKEN AnaLex(FILE *fd)
{
    int estado = 0;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_MAX_LEXEMA] = ""; // Usar a constante do .h
    char digitos[TAM_NUM] = "";       // Usar a constante do .h
    char c;
    TOKEN token;

    while (1)
    {
        c = fgetc(fd);

        switch (estado)
        {
        case 0: // ESTADO INICIAL
            if (c == ' ' || c == '\t') {
                // Ignora espaços em branco
            }
            else if (c == '\n') {
                contLinha++;
            }
            else if (c == '/') {
                estado = 18; // Vai para o estado de comentário/divisão
            }
            else if (c == '\'') {
                estado = 9; // Inicia constante de caractere (SEM modificar lexema)
            }
            else if (c == '\"') {
                tamL = 0; // Prepara para ler string
                lexema[tamL] = '\0';
                estado = 15;
            }
            else if (isalpha(c)) { // Início de um ID
                tamL = 0; // Prepara para ler ID
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
                estado = 1;
            }
            else if (isdigit(c)) { // Início de um número
                tamD = 0; // Prepara para ler número
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
                estado = 4;
            }
            else if (c == '=') {
                estado = 21;
            }
            else if (c == '!') {
                estado = 24;
            }
            else if (c == '<') {
                estado = 27;
            }
            else if (c == '>') {
                estado = 30;
            }
            else if (c == '&') {
                estado = 39;
            }
            else if (c == '|') {
                estado = 42;
            }
            // --- Tokens de um único caractere que podem ser retornados imediatamente ---
            else if (c == '+') { token.cat = OP_ARIT; token.codigo = ADICAO; return token; }
            else if (c == '-') { token.cat = OP_ARIT; token.codigo = SUBTRACAO; return token; }
            else if (c == '*') { token.cat = OP_ARIT; token.codigo = MULTIPLICACAO; return token; }
            else if (c == '{') { token.cat = SINAL; token.codigo = ABRE_CHAVE; return token; }
            else if (c == '}') { token.cat = SINAL; token.codigo = FECHA_CHAVE; return token; }
            else if (c == '(') { token.cat = SINAL; token.codigo = ABRE_PAR; return token; }
            else if (c == ')') { token.cat = SINAL; token.codigo = FECHA_PAR; return token; }
            else if (c == '[') { token.cat = SINAL; token.codigo = ABRE_COL; return token; }
            else if (c == ']') { token.cat = SINAL; token.codigo = FECHA_COL; return token; }
            else if (c == ',') { token.cat = SINAL; token.codigo = VIRGULA; return token; }
            else if (c == ';') { token.cat = SINAL; token.codigo = PONTO_VIRGULA; return token; }
            else if (c == EOF) {
                token.cat = FIM_PROG;
                return token;
            }
            else {
                errorLex(contLinha, c);
            }
            break;

        case 1: // Lendo o resto de um ID
            if (isalpha(c) || isdigit(c) || c == '_') {
                if (tamL < TAM_MAX_LEXEMA - 1) {
                    lexema[tamL++] = c;
                } else {
                    printf("\n[Erro Léxico] Identificador muito longo na linha %d.\n", contLinha);
                    while(isalnum(c = fgetc(fd)) || c == '_');
                    ungetc(c, fd);
                    exit(1);
                }
            }
            else {
                lexema[tamL] = '\0';
                ungetc(c, fd);

                if (strcmp("void", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = VOID; }
                else if (strcmp("char", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = CHAR; }
                else if (strcmp("int", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = INT; }
                else if (strcmp("float", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = FLOAT; }
                else if (strcmp("bool", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = BOOL; }
                else if (strcmp("if", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = IF; }
                else if (strcmp("else", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = ELSE; }
                else if (strcmp("while", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = WHILE; }
                else if (strcmp("for", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = FOR; }
                else if (strcmp("return", lexema) == 0) { token.cat = PAL_RESERV; token.codigo = RETURN; }
                else {
                    token.cat = ID;
                    strcpy(token.lexema, lexema);
                }
                return token;
            }
            break;

        // --- LÓGICA PARA NÚMEROS (INT/FLOAT) ---
        case 4:
            if (isdigit(c)) {
                if (tamD < TAM_NUM - 1) { digitos[tamD++] = c; digitos[tamD] = '\0'; }
                else { printf("\n[Erro Léxico] Constante numérica muito longa na linha %d.\n", contLinha); exit(1); }
            }
            else if (c == '.') {
                if (tamD < TAM_NUM - 1) { digitos[tamD++] = c; digitos[tamD] = '\0'; estado = 6; }
                else { printf("\n[Erro Léxico] Constante numérica muito longa na linha %d.\n", contLinha); exit(1); }
            }
            else {
                ungetc(c, fd);
                token.cat = CONST_INT;
                token.valInt = atoi(digitos);
                return token;
            }
            break;
        case 6:
            if (isdigit(c)) {
                if (tamD < TAM_NUM - 1) { digitos[tamD++] = c; digitos[tamD] = '\0'; estado = 7; }
                else { printf("\n[Erro Léxico] Constante real muito longa na linha %d.\n", contLinha); exit(1); }
            }
            else { printf("\n[Erro Léxico] Constante real mal formada na linha %d. Dígito esperado após o '.'.\n", contLinha); exit(1); }
            break;
        case 7:
            if (isdigit(c)) {
                if (tamD < TAM_NUM - 1) { digitos[tamD++] = c; digitos[tamD] = '\0'; }
                else { printf("\n[Erro Léxico] Constante real muito longa na linha %d.\n", contLinha); exit(1); }
            }
            else {
                ungetc(c, fd);
                token.cat = CONST_FLOAT;
                token.valFloat = atof(digitos);
                return token;
            }
            break;

        // --- LÓGICA PARA CONSTANTE DE CARACTERE ---
        case 9:
            if (c == '\\') { estado = 12; }
            else if (isprint(c) && c != '\'') { estado = 10; token.caractere = c; }
            else { errorLex(contLinha, c); }
            break;
        case 10:
            if (c == '\'') { token.cat = CONST_CHAR; return token; }
            else { errorLex(contLinha, c); }
            break;
        case 12:
            if (c == 'n') { token.caractere = '\n'; estado = 13; }
            else if (c == '0') { token.caractere = '\0'; estado = 13; }
            else if (c == 't') { token.caractere = '\t'; estado = 13; }
            else { errorLex(contLinha, c); }
            break;
        case 13:
            if (c == '\'') { token.cat = CONST_CHAR; return token; }
            else { errorLex(contLinha, c); }
            break;

        // --- LÓGICA PARA STRING LITERAL ---
        case 15:
            if (c == '\"') {
                lexema[tamL] = '\0';
                token.cat = LITERAL;
                strcpy(token.lexema, lexema);
                return token;
            }
            else if (c == '\n' || c == EOF) {
                printf("\n[Erro Léxico] Literal string não terminado na linha: %d\n", contLinha);
                exit(1);
            }
            else if (isprint(c)) {
                if (tamL < TAM_MAX_LEXEMA - 1) { lexema[tamL++] = c; }
                else { printf("\n[Erro Léxico] Literal string excedeu o tamanho máximo na linha: %d\n", contLinha); exit(1); }
            }
            else { errorLex(contLinha, c); }
            break;

        // --- LÓGICA PARA COMENTÁRIOS E OPERADORES DE DOIS CARACTERES ---
        case 18: // Leu um '/'
            if (c == '*') { estado = 19; } // Início de comentário
            else { ungetc(c, fd); token.cat = OP_ARIT; token.codigo = DIVISAO; return token; }
            break;
        case 19: // Dentro do comentário
            if (c == '*') { estado = 20; } // Possível fim de comentário
            else if (c == EOF) { errorLex(contLinha, c); }
            else if (c == '\n') { contLinha++; }
            break;
        case 20: // Leu um '*' dentro de um comentário
            if (c == '/') { estado = 0; } // Fim do comentário
            else if (c == '*') { /* continua em estado=20 */ }
            else if (c == EOF) { errorLex(contLinha, c); }
            else { if (c == '\n') {contLinha++;} estado = 19; } // Não era fim, volta ao normal
            break;

        // Restante da lógica de operadores de 2 caracteres
        case 21: // Leu '='
            if (c == '=') { token.cat = OP_RELAC; token.codigo = IGUALDADE; return token; }
            else { ungetc(c, fd); token.cat = OP_ARIT; token.codigo = ATRIBUICAO; return token; }
            break;
        case 24: // Leu '!'
            if (c == '=') { token.cat = OP_RELAC; token.codigo = DIFERENTE; return token; }
            else { ungetc(c, fd); token.cat = OP_LOGIC; token.codigo = NOT_LOGIC; return token; }
            break;
        case 27: // Leu '<'
            if (c == '=') { token.cat = OP_RELAC; token.codigo = MENOR_IGUAL; return token; }
            else { ungetc(c, fd); token.cat = OP_RELAC; token.codigo = MENOR_QUE; return token; }
            break;
        case 30: // Leu '>'
            if (c == '=') { token.cat = OP_RELAC; token.codigo = MAIOR_IGUAL; return token; }
            else { ungetc(c, fd); token.cat = OP_RELAC; token.codigo = MAIOR_QUE; return token; }
            break;
        case 39: // Leu '&'
            if (c == '&') { token.cat = OP_LOGIC; token.codigo = AND_LOGIC; return token; }
            else { ungetc(c, fd); token.cat = SINAL; token.codigo = REFERENCIA; return token; }
            break;
        case 42: // Leu '|'
            if (c == '|') { token.cat = OP_LOGIC; token.codigo = OR_LOGIC; return token; }
            else { errorLex(contLinha, c); }
            break;

        } // Fim do switch
    } // Fim do while
}
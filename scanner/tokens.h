
#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>

/* Declaração externa das variáveis globais (definidas no lex.yy.c) */
extern int yynum;
extern float yyreal;
extern char *yytext;

/* Declaração da função do lexer */
int yylex(void);

typedef enum
{
    ID,
    NUM,
    REAL,
    SEMICOLON,
    COMMA,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    IF,
    INT,
    FLOAT,
    WHILE,
    FOR,
    END_OF_FILE
} TokenType;

#endif
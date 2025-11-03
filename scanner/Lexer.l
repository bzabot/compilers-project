%{
#include "tokens.h"
#include <stdlib.h>

/* Definição das variáveis globais */
int yynum;
float yyreal;
%}
%option noyywrap
alpha   [_a-zA-Z]
digit [0-9]
%%

[ \t\n\r]+ 
if { return IF; }
{alpha}({alpha}|{digit})*   { return ID; }
{digit}+    { yynum = atoi(yytext); return NUM; }
{digit}+"."{digit}+     { yyreal = atof(yytext); return REAL; }
<<EOF>> { return END_OF_FILE; }

%%
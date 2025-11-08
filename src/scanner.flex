%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap
%option caseless

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

"procedure" { return PROCEDURE; }
"begin" { return BEGIN_BLOCK; }
"is" { return IS; }
"while" { return WHILE; }
"if" { return IF; }
"then" { return THEN; }
"else" { return ELSE; }
"do" { return DO; }
"mod" { return MODULE; }
"and" { return AND; }
"or" { return OR; }
"xor" { return XOR; }
"not" { return NOT; }
"Put_Line" { return PUT_LINE; }
"Get_Line" { return GET_LINE; }
"end" { return END; }
"loop" { return LOOP; }


[a-zA-Z_][a-zA-Z0-9_]* { 
   yylval.strValue = strdup(yytext);  // NEW: Allocate string for ID
   return ID; 
}
\-?[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return TIMES; }
"/" { return DIVIDES; }
"**" { return EXPONENTIAL; }
":=" { return ASSIGN; }
"=" { return EQUALS; }
"/=" { return NOT_EQUAL; }
"<" { return LESS; }
">" { return GREATER; }
"<=" { return LESS_OR_EQUAL; }
">=" { return GREATER_OR_EQUAL; }
";" { return SEMICOLON; }
"(" { return LPAREN; }
")" { return RPAREN; }


.  { yyerror("unexpected character"); }
%%


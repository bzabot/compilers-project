// Tokens
%token 
  INT  
  PLUS
  MINUS
  TIMES
  DIVIDES
  MODULE
  EXPONENTIAL
  EQUALS
  NOT_EQUAL
  LESS
  GREATER
  LESS_OR_EQUAL
  GREATER_OR_EQUAL
  ASSIGN
  IF
  THEN
  ELSE
  WHILE
  DO
  SEMICOLON
  ID
  AND
  OR
  XOR
  NOT
  PUT_LINE
  GET_LINE
  LPAREN
  RPAREN
  END
  LOOP
  PROCEDURE
  BEGIN_BLOCK
  IS

// Operator associativity & precedence
%left SEMICOLON
%left ASSIGN 
%left IF THEN ELSE 
%left WHILE DO
%left AND OR XOR
%left NOT
%left EQUALS NOT_EQUAL LESS GREATER LESS_OR_EQUAL GREATER_OR_EQUAL 
%left PLUS MINUS
%left TIMES DIVIDES MODULE 
%left EXPONENTIAL


// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  char* strValue;
  Expr* exprValue; 
  BoolExpr* boolExprValue;
  Cmd* cmdValue;
}

%type <intValue> INT
%type <strValue> ID
%type <exprValue> expr
%type <boolExprValue> boolExpr
%type <cmdValue> Cmd
%type <cmdValue> Cmds

%union {
  Program* programValue;
}
%type <programValue> AdaProgram


// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
extern Expr* root;
extern BoolExpr* root2;
extern Cmd* cmd_root;
extern Program* program_root;
}

%%
program: 
  expr { root = $1; }
  |
  boolExpr { root2 = $1; }
  |
  Cmds { cmd_root = $1; }
  |
  AdaProgram { program_root = $1; }
  ;

AdaProgram:
  PROCEDURE ID IS BEGIN_BLOCK Cmds END ID SEMICOLON {
    $$ = ast_program($2, $5);
  }
  |
  PROCEDURE ID IS BEGIN_BLOCK Cmds END SEMICOLON {
    $$ = ast_program($2, $5);
  }
  ;

expr: 
  INT { 
    $$ = ast_integer($1); 
  }
  |
  ID {
    $$ = ast_variable($1);
  }
  | 
  expr PLUS expr { 
    $$ = ast_operation(PLUS, $1, $3); 
  } 
  |
  expr MINUS expr { 
    $$ = ast_operation(MINUS, $1, $3); 
  } 
  |
  expr TIMES expr {
    $$ = ast_operation(TIMES, $1, $3);
  }
  |
  expr DIVIDES expr {
    $$ = ast_operation(DIVIDES, $1, $3);
  }
  |
  expr MODULE expr {
    $$ = ast_operation(MODULE, $1, $3);
  }
  |
  expr EXPONENTIAL expr {
    $$ = ast_operation(EXPONENTIAL, $1, $3);
  }
  |
  LPAREN expr RPAREN {
    $$ = $2;
  }
  ;

boolExpr:
  expr EQUALS expr {
    $$ = ast_bool_relop(EQUALS, $1, $3);
  }
  |
  expr NOT_EQUAL expr {
    $$ = ast_bool_relop(NOT_EQUAL, $1, $3);
  }
  |
  expr LESS expr {
    $$ = ast_bool_relop(LESS, $1, $3);
  }
  |
  expr GREATER expr {
    $$ = ast_bool_relop(GREATER, $1, $3);
  } 
  |
  expr LESS_OR_EQUAL expr {
    $$ = ast_bool_relop(LESS_OR_EQUAL, $1, $3);
  }
  |
  expr GREATER_OR_EQUAL expr {
    $$ = ast_bool_relop(GREATER_OR_EQUAL, $1, $3);
  }
  |
  boolExpr AND boolExpr {
    $$ = ast_bool_binary(AND, $1, $3);
  }
  |
  boolExpr OR boolExpr {
    $$ = ast_bool_binary(OR, $1, $3);
  }
  |
  boolExpr XOR boolExpr {
    $$ = ast_bool_binary(XOR, $1, $3);
  }
  |
  NOT boolExpr {
    $$ = ast_bool_unary(NOT, $2);
  }
  |
  LPAREN boolExpr RPAREN {
    $$ = $2;
  }
  ;

Cmds:
  Cmd { $$ = $1; }
  |
  Cmd SEMICOLON { $$ = $1; }
  |
  Cmd SEMICOLON Cmds {
    $$ = ast_sequence($1, $3);
  }
  ;

Cmd:
  ID ASSIGN expr {
    $$ = ast_assignment($1, $3);
  } 
  |
  IF boolExpr THEN Cmds ELSE Cmds END IF{
    $$ = ast_if_then_else($2, $4, $6);
  }
  |
  WHILE boolExpr LOOP Cmds END LOOP {
    $$ = ast_while_loop($2, $4);
  }
  |
  PUT_LINE LPAREN expr RPAREN {
    $$ = ast_put_line_expr($3);
  }
  |
  PUT_LINE LPAREN boolExpr RPAREN {
    $$ = ast_put_line_bool($3);
  }
  |
  PUT_LINE LPAREN ID RPAREN {
    $$ = ast_put_line_id($3);
  }
  |
  GET_LINE LPAREN ID RPAREN {
    $$ = ast_get_line($3);
  }
  ;
%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

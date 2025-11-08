/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 78 "parser.bison"

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

#line 65 "parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    PLUS = 259,                    /* PLUS  */
    MINUS = 260,                   /* MINUS  */
    TIMES = 261,                   /* TIMES  */
    DIVIDES = 262,                 /* DIVIDES  */
    MODULE = 263,                  /* MODULE  */
    EXPONENTIAL = 264,             /* EXPONENTIAL  */
    EQUALS = 265,                  /* EQUALS  */
    NOT_EQUAL = 266,               /* NOT_EQUAL  */
    LESS = 267,                    /* LESS  */
    GREATER = 268,                 /* GREATER  */
    LESS_OR_EQUAL = 269,           /* LESS_OR_EQUAL  */
    GREATER_OR_EQUAL = 270,        /* GREATER_OR_EQUAL  */
    ASSIGN = 271,                  /* ASSIGN  */
    IF = 272,                      /* IF  */
    THEN = 273,                    /* THEN  */
    ELSE = 274,                    /* ELSE  */
    WHILE = 275,                   /* WHILE  */
    DO = 276,                      /* DO  */
    SEMICOLON = 277,               /* SEMICOLON  */
    ID = 278,                      /* ID  */
    AND = 279,                     /* AND  */
    OR = 280,                      /* OR  */
    XOR = 281,                     /* XOR  */
    NOT = 282,                     /* NOT  */
    PUT_LINE = 283,                /* PUT_LINE  */
    GET_LINE = 284,                /* GET_LINE  */
    LPAREN = 285,                  /* LPAREN  */
    RPAREN = 286,                  /* RPAREN  */
    END = 287,                     /* END  */
    LOOP = 288,                    /* LOOP  */
    PROCEDURE = 289,               /* PROCEDURE  */
    BEGIN_BLOCK = 290,             /* BEGIN_BLOCK  */
    IS = 291                       /* IS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 55 "parser.bison"

  int intValue;
  char* strValue;
  Expr* exprValue; 
  BoolExpr* boolExprValue;
  Cmd* cmdValue;
#line 70 "parser.bison"

  Program* programValue;

#line 129 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */

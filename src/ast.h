
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr
{
  enum
  {
    E_INTEGER,
    E_OPERATION,
    E_VARIABLE
  } kind;
  union
  {
    int value; // for integer values
    char *id;  // for variable identifiers
    struct
    {
      int operator; // PLUS, MINUS, etc
      struct _Expr *left;
      struct _Expr *right;
    } op; // for binary expressions
  } attr;
};

typedef struct _Expr Expr; // Convenience typedef

// Constructor functions (see implementation in ast.c)
Expr *ast_integer(int v);
Expr *ast_variable(char *id);
Expr *ast_operation(int operator, Expr * left, Expr *right);

// AST for boolean expressions
struct _BoolExpr
{
  enum
  {
    E_BOOL_INTEGER,
    E_BOOL_RELOP,
    E_BOOL_BINARY,
    E_BOOL_UNARY
  } kind;
  union
  {
    int value; // for boolean integer values (0 for false, 1 for true)
    struct
    {
      int relop; // ==, !=
      struct _Expr *left;
      struct _Expr *right;
    } op;
    struct
    {
      int logic_op; // AND, OR
      struct _BoolExpr *left;
      struct _BoolExpr *right;
    } bin; // logical binary expressions

    struct
    {
      int logic_op; // NOT (or other unary boolean ops)
      struct _BoolExpr *expr;
    } un; // logical unary expression
  } attr;
};

typedef struct _BoolExpr BoolExpr; // Convenience typedef

// Constructor functions (see implementation in ast.c)
BoolExpr *ast_bool_integer(int v);
BoolExpr *ast_bool_relop(int rel_operator, Expr *left, Expr *right);
BoolExpr *ast_bool_binary(int rel_operator, BoolExpr *left, BoolExpr *right);
BoolExpr *ast_bool_unary(int rel_operator, BoolExpr *expr);

struct _Cmd
{
  enum
  {
    E_ASSIGNMENT,
    E_IF_THEN_ELSE,
    E_WHILE,
    E_SEQUENCE,
    E_PUT_LINE,
    E_GET_LINE
  } kind;
  union
  {
    struct
    {
      char *id;
      Expr *expr;
    } assignment;

    struct
    {
      BoolExpr *condition;
      struct _Cmd *then_cmd;
      struct _Cmd *else_cmd;
    } if_then_else;
    struct
    {
      BoolExpr *condition;
      struct _Cmd *body;
    } while_loop;
    struct
    {
      struct _Cmd *first;
      struct _Cmd *second;
    } sequence;

    struct
    {
      enum
      {
        PRINT_EXPR,
        PRINT_BOOL,
        PRINT_ID
      } print_type;
      union
      {
        Expr *expr;
        BoolExpr *bool_expr;
        char *id;
      } value;
    } put_line;

    struct
    {
      char *id;
    } get_line;

  } attr;
};

typedef struct _Cmd Cmd; // Convenience typedef

// Constructor functions (see implementation in ast.c)
Cmd *ast_assignment(char *id, Expr *expr);
Cmd *ast_if_then_else(BoolExpr *condition, Cmd *then_cmd, Cmd *else_cmd);
Cmd *ast_while_loop(BoolExpr *condition, Cmd *body);
Cmd *ast_sequence(Cmd *first, Cmd *second);

// AST for Ada Program (Procedure)
struct _Program
{
  char *procedure_name;
  Cmd *body; // The commands in the begin..end block
};

typedef struct _Program Program;

// Constructor function
Program *ast_program(char *name, Cmd *body);

/*
INPUT
PutLine
GetLine
*/

struct _IO
{
  enum
  {
    E_IO,
    E_OP
  } kind;
  union
  {
    struct
    {
      char *id;
      Expr *expr;
    } assignment;

    struct
    {
      BoolExpr *condition;
      struct _Cmd *then_cmd;
      struct _Cmd *else_cmd;
    } if_then_else;
    struct
    {
      BoolExpr *condition;
      struct _Cmd *body;
    } while_loop;
    struct
    {
      struct _Cmd *first;
      struct _Cmd *second;
    } sequence;

  } attr;
};

typedef struct _IO IO; // Convenience typedef

// Constructor functions (see implementation in ast.c)
Cmd *ast_assignment(char *id, Expr *expr);
Cmd *ast_if_then_else(BoolExpr *condition, Cmd *then_cmd, Cmd *else_cmd);
Cmd *ast_while_loop(BoolExpr *condition, Cmd *body);
Cmd *ast_sequence(Cmd *first, Cmd *second);
Cmd *ast_put_line_expr(Expr *expr);
Cmd *ast_put_line_bool(BoolExpr *bool_expr);
Cmd *ast_put_line_id(char *id);
Cmd *ast_get_line(char *id);

#endif

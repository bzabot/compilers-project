// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h"    // AST header

Expr *root;            // Global root of the AST
BoolExpr *root2;       // Global root for boolean expressions
Cmd *cmd_root;         // Global root for commands
Program *program_root; // Global root for full programs

Expr *ast_integer(int v)
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr *ast_float(float v)
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_FLOAT;
  node->attr.fvalue = v;
  return node;
}

Expr *ast_variable(char *id)
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_VARIABLE;
  node->attr.id = id;
  return node;
}

Expr *ast_operation(int operator, Expr *left, Expr *right)
{
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

/*
Bool and Logic expressions
 ==, /=, >, >=, <, <=, and, or, xor, not
*/
BoolExpr *ast_bool_integer(int v)
{
  BoolExpr *node = (BoolExpr *)malloc(sizeof(BoolExpr));
  node->kind = E_BOOL_INTEGER;
  node->attr.value = v;
  return node;
}

BoolExpr *ast_bool_relop(int rel_operator, Expr *left, Expr *right)
{
  BoolExpr *node = (BoolExpr *)malloc(sizeof(BoolExpr));
  node->kind = E_BOOL_RELOP;
  node->attr.op.relop = rel_operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

BoolExpr *ast_bool_binary(int rel_operator, BoolExpr *left, BoolExpr *right)
{
  BoolExpr *node = (BoolExpr *)malloc(sizeof(BoolExpr));
  node->kind = E_BOOL_BINARY;
  node->attr.bin.logic_op = rel_operator;
  node->attr.bin.left = left;
  node->attr.bin.right = right;
  return node;
}

BoolExpr *ast_bool_unary(int rel_operator, BoolExpr *expr)
{
  BoolExpr *node = (BoolExpr *)malloc(sizeof(BoolExpr));
  node->kind = E_BOOL_UNARY;
  node->attr.un.logic_op = rel_operator;
  node->attr.un.expr = expr;
  return node;
}

Cmd *ast_assignment(char *id, Expr *expr)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_ASSIGNMENT;
  node->attr.assignment.id = id;
  node->attr.assignment.expr = expr;
  return node;
}

Cmd *ast_bool_assignment(char *id, BoolExpr *expr)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_BOOL_ASSIGNMENT;
  node->attr.bool_assignment.id = id;
  node->attr.bool_assignment.expr = expr;
  return node;
}

Cmd *ast_if_then_else(BoolExpr *condition, Cmd *then_cmd, Cmd *else_cmd)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_IF_THEN_ELSE;
  node->attr.if_then_else.condition = condition;
  node->attr.if_then_else.then_cmd = then_cmd;
  node->attr.if_then_else.else_cmd = else_cmd;
  return node;
}

Cmd *ast_while_loop(BoolExpr *condition, Cmd *body)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_WHILE;
  node->attr.while_loop.condition = condition;
  node->attr.while_loop.body = body;
  return node;
}

Cmd *ast_sequence(Cmd *first, Cmd *second)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_SEQUENCE;
  node->attr.sequence.first = first;
  node->attr.sequence.second = second;
  return node;
}

Cmd *ast_put_line_expr(Expr *expr)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_PUT_LINE;
  node->attr.put_line.print_type = PRINT_EXPR;
  node->attr.put_line.value.expr = expr;
  return node;
}

Cmd *ast_put_line_bool(BoolExpr *bool_expr)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_PUT_LINE;
  node->attr.put_line.print_type = PRINT_BOOL;
  node->attr.put_line.value.bool_expr = bool_expr;
  return node;
}

Cmd *ast_put_line_id(char *id)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_PUT_LINE;
  node->attr.put_line.print_type = PRINT_ID;
  node->attr.put_line.value.id = id;
  return node;
}

Cmd *ast_get_line(char *id)
{
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = E_GET_LINE;
  node->attr.get_line.id = id;
  return node;
}

Program *ast_program(char *name, Cmd *body)
{
  Program *node = (Program *)malloc(sizeof(Program));
  node->procedure_name = name;
  node->body = body;
  return node;
}

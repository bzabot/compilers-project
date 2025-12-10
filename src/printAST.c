#include <stdio.h>
#include <ctype.h>
#include "parser.h"
#include "tac.h"

void printAbsTree(Expr *expr, int depth)
{
    if (expr == 0)
    {
        yyerror("Null expression!!");
    }

    for (int i = 0; i < depth; i++)
    {
        printf("   ");
    }

    if (expr->kind == E_INTEGER)
    {
        printf("INT: %d", expr->attr.value);
    }
    else if (expr->kind == E_FLOAT)
    {
        printf("FLOAT: %f", expr->attr.fvalue);
    }
    else if (expr->kind == E_VARIABLE)
    {
        printf("VAR: %s", expr->attr.id);
    }
    else if (expr->kind == E_OPERATION)
    {
        printf("OP: ");
        switch (expr->attr.op.operator)
        {
        case PLUS:
            printf("+");
            break;
        case MINUS:
            printf("-");
            break;
        case TIMES:
            printf("*");
            break;
        case DIVIDES:
            printf("/");
            break;
        case MODULE:
            printf("mod");
            break;
        default:
            yyerror("Unknown operator!");
        }
        printf("\n");
        printAbsTree(expr->attr.op.left, depth + 1);
        printf("\n");
        printAbsTree(expr->attr.op.right, depth + 1);
    }
}

void printBoolAbsTree(BoolExpr *expr, int depth)
{
    if (expr == 0)
    {
        yyerror("Null boolean expression!!");
    }

    for (int i = 0; i < depth; i++)
    {
        printf("   ");
    }

    if (expr->kind == E_BOOL_INTEGER)
    {
        printf("BOOL_INT: %d", expr->attr.value);
    }
    else if (expr->kind == E_BOOL_RELOP)
    {
        printf("RELOP: ");
        switch (expr->attr.op.relop)
        {
        case EQUALS:
            printf("=");
            break;
        case NOT_EQUAL:
            printf("/=");
            break;
        case GREATER:
            printf(">");
            break;
        case GREATER_OR_EQUAL:
            printf(">=");
            break;
        case LESS:
            printf("<");
            break;
        case LESS_OR_EQUAL:
            printf("<=");
            break;
        default:
            yyerror("Unknown relational operator!");
        }
        printf("\n");
        printAbsTree(expr->attr.op.left, depth + 1);
        printf("\n");
        printAbsTree(expr->attr.op.right, depth + 1);
    }
    else if (expr->kind == E_BOOL_BINARY)
    {
        printf("LOGIC_BIN: ");
        switch (expr->attr.bin.logic_op)
        {
        case AND:
            printf("and");
            break;
        case OR:
            printf("or");
            break;
        case XOR:
            printf("xor");
            break;
        default:
            yyerror("Unknown logical binary operator!");
        }
        printf("\n");
        printBoolAbsTree(expr->attr.bin.left, depth + 1);
        printf("\n");
        printBoolAbsTree(expr->attr.bin.right, depth + 1);
    }
    else if (expr->kind == E_BOOL_UNARY)
    {
        printf("LOGIC_UN: not\n");
        printBoolAbsTree(expr->attr.un.expr, depth + 1);
    }
}

void printCmd(Cmd *cmd, int depth)
{
    if (cmd == 0)
    {
        yyerror("Null command!!");
    }

    for (int i = 0; i < depth; i++)
    {
        printf("   ");
    }

    if (cmd->kind == E_ASSIGNMENT)
    {
        printf("ASSIGNMENT: %s", cmd->attr.assignment.id);
        printf("\n");
        printAbsTree(cmd->attr.assignment.expr, depth + 1);
        printf("\n");
        // transExp(cmd->attr.assignment.expr, cmd->attr.assignment.id); // Removed: Code generation is now handled by codegen.c

    }
    else if (cmd->kind == E_BOOL_ASSIGNMENT)
    {
        printf("BOOL_ASSIGNMENT: %s", cmd->attr.bool_assignment.id);
        printf("\n");
        printBoolAbsTree(cmd->attr.bool_assignment.expr, depth + 1);
    }
    else if (cmd->kind == E_IF_THEN_ELSE)
    {
        printf("IF\n");
        printBoolAbsTree(cmd->attr.if_then_else.condition, depth + 1);
        printf("\n");
        for (int i = 0; i < depth; i++)
            printf("   ");
        printf("THEN\n");
        printCmd(cmd->attr.if_then_else.then_cmd, depth + 1);
        printf("\n");
        for (int i = 0; i < depth; i++)
            printf("   ");
        printf("ELSE\n");
        printCmd(cmd->attr.if_then_else.else_cmd, depth + 1);
    }
    else if (cmd->kind == E_WHILE)
    {
        printf("WHILE\n");
        printBoolAbsTree(cmd->attr.while_loop.condition, depth + 1);
        printf("\n");
        for (int i = 0; i < depth; i++)
            printf("   ");
        printf("DO\n");
        printCmd(cmd->attr.while_loop.body, depth + 1);
    }
    else if (cmd->kind == E_SEQUENCE)
    {
        printf("SEQUENCE\n");
        printCmd(cmd->attr.sequence.first, depth + 1);
        printf("\n");
        printCmd(cmd->attr.sequence.second, depth + 1);
    }
    else if (cmd->kind == E_PUT_LINE)
    {
        printf("PUT_LINE\n");
        if (cmd->attr.put_line.print_type == PRINT_EXPR)
        {
            printAbsTree(cmd->attr.put_line.value.expr, depth + 1);
        }
        else if (cmd->attr.put_line.print_type == PRINT_BOOL)
        {
            printBoolAbsTree(cmd->attr.put_line.value.bool_expr, depth + 1);
        }
        else if (cmd->attr.put_line.print_type == PRINT_ID)
        {
            for (int i = 0; i < depth + 1; i++)
                printf("   ");
            printf("ID: %s", cmd->attr.put_line.value.id);
        }
    }
    else if (cmd->kind == E_GET_LINE)
    {
        printf("GET_LINE\n");
        for (int i = 0; i < depth + 1; i++)
            printf("   ");
        printf("ID: %s", cmd->attr.get_line.id);
    }
    else
    {
        yyerror("Unknown command!");
    }
}
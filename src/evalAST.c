#include <stdio.h>
#include "parser.h"

int eval(Expr *expr)
{
    int result = 0;
    if (expr == 0)
    {
        yyerror("Null expression!!");
    }
    else if (expr->kind == E_INTEGER)
    {
        result = expr->attr.value;
    }
    else if (expr->kind == E_VARIABLE)
    {
        // Variable evaluation - would need symbol table for actual values
        result = 0; // placeholder
    }
    else if (expr->kind == E_OPERATION)
    {
        int vLeft = eval(expr->attr.op.left);
        int vRight = eval(expr->attr.op.right);
        switch (expr->attr.op.operator)
        {
        case PLUS:
            result = vLeft + vRight;
            break;
        case MINUS:
            result = vLeft - vRight;
            break;
        case TIMES:
            result = vLeft * vRight;
            break;
        case DIVIDES:
            result = vLeft / vRight;
            break;
        case MODULE:
            result = vLeft % vRight;
            break;

        default:
            yyerror("Unknown operator!");
        }
    }
    return result;
}

int evalBool(BoolExpr *expr)
{
    int result = 0;
    if (expr == 0)
    {
        yyerror("Null expression!!");
    }
    else if (expr->kind == E_BOOL_INTEGER)
    {
        result = 1;
    }
    else if (expr->kind == E_BOOL_RELOP)
    {
        int vLeft = eval(expr->attr.op.left);
        int vRight = eval(expr->attr.op.right);
        switch (expr->attr.op.relop)
        {
        case EQUALS:
            result = vLeft == vRight;
            break;
        case NOT_EQUAL:
            result = vLeft != vRight;
            break;
        case GREATER:
            result = vLeft > vRight;
            break;
        case GREATER_OR_EQUAL:
            result = vLeft >= vRight;
            break;
        case LESS:
            result = vLeft < vRight;
            break;
        case LESS_OR_EQUAL:
            result = vLeft <= vRight;
            break;
        default:
            yyerror("Unknown operator!");
        }
    }
    return result;
}
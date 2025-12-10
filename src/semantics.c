#include <stdio.h>
#include "semantics.h"

static SymbolTable *table;
static int semantic_errors = 0;
static int offset_counter = 0; // Global offset counter

void sem_error(const char *msg) {
    fprintf(stderr, "Semantic Error: %s\n", msg);
    semantic_errors++;
}

void check_cmd(Cmd *cmd);
SymbolType check_expr(Expr *expr);
void check_bool_expr(BoolExpr *expr);

// --- Expression Checking ---
SymbolType check_expr(Expr *expr) {
    if (!expr) return TYPE_UNKNOWN;

    switch (expr->kind) {
        case E_INTEGER:
            return TYPE_INT;
        
        case E_FLOAT:
            return TYPE_FLOAT;

        case E_VARIABLE: {
            Entry *entry = lookup_symbol(table, expr->attr.id);
            if (!entry) {
                char buf[256];
                sprintf(buf, "Variable '%s' used before assignment", expr->attr.id);
                sem_error(buf);
                return TYPE_UNKNOWN;
            }
            return entry->type;
        }

        case E_OPERATION: {
            SymbolType left = check_expr(expr->attr.op.left);
            SymbolType right = check_expr(expr->attr.op.right);

            if (left == TYPE_UNKNOWN || right == TYPE_UNKNOWN) return TYPE_UNKNOWN;

            if (left == TYPE_INT && right == TYPE_INT) return TYPE_INT;
            
            if (left == TYPE_FLOAT && right == TYPE_FLOAT) return TYPE_FLOAT;

            // Permite int + float + float, por exemplo
            if ((left == TYPE_INT && right == TYPE_FLOAT) || 
                (left == TYPE_FLOAT && right == TYPE_INT)) {
                return TYPE_FLOAT;
            }

            sem_error("Type mismatch in arithmetic operation");
            return TYPE_UNKNOWN;
        }
    }
    return TYPE_UNKNOWN;
}

void check_bool_expr(BoolExpr *expr) {
    if (!expr) return;

    switch (expr->kind) {
        case E_BOOL_INTEGER:
            // 0 or 1
            break;

        case E_BOOL_RELOP: {
            SymbolType left = check_expr(expr->attr.op.left);
            SymbolType right = check_expr(expr->attr.op.right);
            
            if (left == TYPE_UNKNOWN || right == TYPE_UNKNOWN) return;

            if (!((left == TYPE_INT || left == TYPE_FLOAT) && 
                  (right == TYPE_INT || right == TYPE_FLOAT))) {
                sem_error("Relational operator requires numeric operands");
            }
            break;
        }

        case E_BOOL_BINARY:
            check_bool_expr(expr->attr.bin.left);
            check_bool_expr(expr->attr.bin.right);
            break;

        case E_BOOL_UNARY:
            check_bool_expr(expr->attr.un.expr);
            break;
    }
}


void check_cmd(Cmd *cmd) {
    if (!cmd) return;

    switch (cmd->kind) {
        case E_ASSIGNMENT: {
            SymbolType exprType = check_expr(cmd->attr.assignment.expr);
            if (exprType == TYPE_UNKNOWN) return;

            Entry *entry = lookup_symbol(table, cmd->attr.assignment.id);
            if (entry) {
                if (entry->type != exprType) {
                    if (entry->type == TYPE_FLOAT && exprType == TYPE_INT) {
       
                    } else {
                        char buf[256];
                        sprintf(buf, "Type mismatch in assignment to '%s'", cmd->attr.assignment.id);
                        sem_error(buf);
                    }
                }
            } else {
                insert_symbol(table, cmd->attr.assignment.id, exprType, offset_counter);
                offset_counter += 4; 
            }
            break;
        }

        case E_BOOL_ASSIGNMENT: {
            check_bool_expr(cmd->attr.bool_assignment.expr);
            
            Entry *entry = lookup_symbol(table, cmd->attr.bool_assignment.id);
            if (entry) {
                if (entry->type != TYPE_BOOL) {
                    sem_error("Assigning boolean to non-boolean variable");
                }
            } else {
                insert_symbol(table, cmd->attr.bool_assignment.id, TYPE_BOOL, offset_counter);
                offset_counter += 4;
            }
            break;
        }

        case E_IF_THEN_ELSE:
            check_bool_expr(cmd->attr.if_then_else.condition);
            check_cmd(cmd->attr.if_then_else.then_cmd);
            if (cmd->attr.if_then_else.else_cmd) {
                check_cmd(cmd->attr.if_then_else.else_cmd);
            }
            break;

        case E_WHILE:
            check_bool_expr(cmd->attr.while_loop.condition);
            check_cmd(cmd->attr.while_loop.body);
            break;

        case E_SEQUENCE:
            check_cmd(cmd->attr.sequence.first);
            check_cmd(cmd->attr.sequence.second);
            break;

        case E_PUT_LINE:
            if (cmd->attr.put_line.print_type == PRINT_EXPR) {
                check_expr(cmd->attr.put_line.value.expr);
            } else if (cmd->attr.put_line.print_type == PRINT_BOOL) {
                check_bool_expr(cmd->attr.put_line.value.bool_expr);
            } else if (cmd->attr.put_line.print_type == PRINT_ID) {
                 if (!lookup_symbol(table, cmd->attr.put_line.value.id)) {
                     sem_error("Printing undefined variable");
                 }
            }
            break;

        case E_GET_LINE:

            if (!lookup_symbol(table, cmd->attr.get_line.id)) {
                 insert_symbol(table, cmd->attr.get_line.id, TYPE_INT, offset_counter);
                 offset_counter += 4;
            }
            break;
    }
}


int check_semantics(Program *program) {
    if (!program) return -1;

    // Initialize symbol table and reset offset counter
    table = create_symbol_table(101);
    semantic_errors = 0;
    offset_counter = 0;

    // Traverse the AST
    check_cmd(program->body);

    // free_symbol_table(table); 
    
    return semantic_errors > 0 ? -1 : 0;
}

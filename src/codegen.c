#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "tac.h"
#include "mips.h"
#include "parser.h" 

static TacList *tac_list;

static void transExp(Expr *expr, char* dest) {
    if (!expr) return;

    switch(expr->kind) {
        case E_INTEGER:
            append_tac(tac_list, tac_assign_int(dest, expr->attr.value));
            break;
        
        case E_FLOAT:
            append_tac(tac_list, tac_assign_int(dest, (int)expr->attr.fvalue));
            break;

        case E_VARIABLE:
            append_tac(tac_list, tac_assign_var(dest, expr->attr.id));
            break;

        case E_OPERATION: {
            char t1[16], t2[16];
            sprintf(t1, "t%d", newTemp());
            sprintf(t2, "t%d", newTemp());
            
            transExp(expr->attr.op.left, t1);
            transExp(expr->attr.op.right, t2);
            
            append_tac(tac_list, tac_bin_op(dest, t1, expr->attr.op.operator, t2));
            break;
        }
    }
}

static void transCond(BoolExpr *expr, char* trueLabel, char* falseLabel) {
    if (!expr) return;

    switch(expr->kind) {
        case E_BOOL_INTEGER:
            if (expr->attr.value) 
                append_tac(tac_list, tac_jump(trueLabel));
            else 
                append_tac(tac_list, tac_jump(falseLabel));
            break;

        case E_BOOL_RELOP: {
            char t1[16], t2[16];
            sprintf(t1, "t%d", newTemp());
            sprintf(t2, "t%d", newTemp());
            
            transExp(expr->attr.op.left, t1);
            transExp(expr->attr.op.right, t2);
            
            append_tac(tac_list, tac_cond_jump(t1, expr->attr.op.relop, t2, trueLabel));
            append_tac(tac_list, tac_jump(falseLabel));
            break;
        }

        case E_BOOL_BINARY: {
            if (expr->attr.bin.logic_op == AND) {
                char l_mid[16];
                sprintf(l_mid, "L%d", newLabel());
                transCond(expr->attr.bin.left, l_mid, falseLabel);
                append_tac(tac_list, tac_label(l_mid));
                transCond(expr->attr.bin.right, trueLabel, falseLabel);
            } else if (expr->attr.bin.logic_op == OR) {
                char l_mid[16];
                sprintf(l_mid, "L%d", newLabel());
                transCond(expr->attr.bin.left, trueLabel, l_mid);
                append_tac(tac_list, tac_label(l_mid));
                transCond(expr->attr.bin.right, trueLabel, falseLabel);
            }
            break;
        }

        case E_BOOL_UNARY: {
            // NOT: swap labels
            transCond(expr->attr.un.expr, falseLabel, trueLabel);
            break;
        }
    }
}

static void transCmd(Cmd *cmd) {
    if (!cmd) return;

    switch(cmd->kind) {
        case E_ASSIGNMENT: {
            char t1[16];
            sprintf(t1, "t%d", newTemp());
            transExp(cmd->attr.assignment.expr, t1);
            append_tac(tac_list, tac_assign_var(cmd->attr.assignment.id, t1));
            break;
        }
        
        case E_BOOL_ASSIGNMENT: {
            char l_true[16], l_false[16], l_end[16];
            sprintf(l_true, "L%d", newLabel());
            sprintf(l_false, "L%d", newLabel());
            sprintf(l_end, "L%d", newLabel());
            
            transCond(cmd->attr.bool_assignment.expr, l_true, l_false);
            
            append_tac(tac_list, tac_label(l_true));
            append_tac(tac_list, tac_assign_int(cmd->attr.bool_assignment.id, 1));
            append_tac(tac_list, tac_jump(l_end));
            
            append_tac(tac_list, tac_label(l_false));
            append_tac(tac_list, tac_assign_int(cmd->attr.bool_assignment.id, 0));
            
            append_tac(tac_list, tac_label(l_end));
            break;
        }
        
        case E_IF_THEN_ELSE: {
            char l_true[16], l_false[16], l_end[16];
            sprintf(l_true, "L%d", newLabel());
            sprintf(l_false, "L%d", newLabel());
            sprintf(l_end, "L%d", newLabel());

            transCond(cmd->attr.if_then_else.condition, l_true, l_false);
            
            append_tac(tac_list, tac_label(l_true));
            transCmd(cmd->attr.if_then_else.then_cmd);
            append_tac(tac_list, tac_jump(l_end));
            
            append_tac(tac_list, tac_label(l_false));
            if (cmd->attr.if_then_else.else_cmd) {
                transCmd(cmd->attr.if_then_else.else_cmd);
            }
            
            append_tac(tac_list, tac_label(l_end));
            break;
        }

        case E_WHILE: {
            char l_start[16], l_body[16], l_end[16];
            sprintf(l_start, "L%d", newLabel());
            sprintf(l_body, "L%d", newLabel());
            sprintf(l_end, "L%d", newLabel());

            append_tac(tac_list, tac_label(l_start));
            transCond(cmd->attr.while_loop.condition, l_body, l_end);
            
            append_tac(tac_list, tac_label(l_body));
            transCmd(cmd->attr.while_loop.body);
            append_tac(tac_list, tac_jump(l_start));
            
            append_tac(tac_list, tac_label(l_end));
            break;
        }

        case E_SEQUENCE:
            transCmd(cmd->attr.sequence.first);
            transCmd(cmd->attr.sequence.second);
            break;

        case E_PUT_LINE:
            if (cmd->attr.put_line.print_type == PRINT_EXPR) {
                char t1[16];
                sprintf(t1, "t%d", newTemp());
                transExp(cmd->attr.put_line.value.expr, t1);
                append_tac(tac_list, tac_print(t1));
            } else if (cmd->attr.put_line.print_type == PRINT_BOOL) {
                char t1[16];
                char l_true[16], l_false[16], l_end[16];
                sprintf(t1, "t%d", newTemp());
                sprintf(l_true, "L%d", newLabel());
                sprintf(l_false, "L%d", newLabel());
                sprintf(l_end, "L%d", newLabel());
                
                transCond(cmd->attr.put_line.value.bool_expr, l_true, l_false);
                
                append_tac(tac_list, tac_label(l_true));
                append_tac(tac_list, tac_assign_int(t1, 1));
                append_tac(tac_list, tac_jump(l_end));
                
                append_tac(tac_list, tac_label(l_false));
                append_tac(tac_list, tac_assign_int(t1, 0));
                
                append_tac(tac_list, tac_label(l_end));
                append_tac(tac_list, tac_print(t1));
            } else if (cmd->attr.put_line.print_type == PRINT_ID) {
                append_tac(tac_list, tac_print(cmd->attr.put_line.value.id));
            }
            break;
        
        case E_GET_LINE:
            append_tac(tac_list, tac_read(cmd->attr.get_line.id));
            break;
    }
}

void generate_code(Program *program) {
    if (!program) return;
    
    // Generate TAC 
    tac_list = create_tac_list();
    transCmd(program->body);
    print_tac_list(tac_list);
    
    // Generate MIPS from TAC 
    generate_mips_from_tac(tac_list);
    
    free_tac_list(tac_list);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "parser.h"

int temp_count = 0;
int label_count = 0;

int newTemp() {
    return temp_count++;
}

int newLabel() {
    return label_count++;
}

TacList* create_tac_list() {
    TacList *list = (TacList*)malloc(sizeof(TacList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void append_tac(TacList *list, TacInstruction *instr) {
    if (!list || !instr) return;
    
    if (list->tail == NULL) {
        list->head = list->tail = instr;
    } else {
        list->tail->next = instr;
        list->tail = instr;
    }
    instr->next = NULL;
}

void free_tac_list(TacList *list) {
    if (!list) return;
    
    TacInstruction *curr = list->head;
    while (curr) {
        TacInstruction *temp = curr;
        curr = curr->next;
        
        if (temp->dest) free(temp->dest);
        if (temp->src1) free(temp->src1);
        if (temp->src2) free(temp->src2);
        free(temp);
    }
    free(list);
}

TacInstruction* tac_assign_int(char *dest, int val) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_ASSIGN_INT;
    instr->dest = strdup(dest);
    instr->src1 = NULL;
    instr->src2 = NULL;
    instr->int_val = val;
    instr->op = 0;
    instr->next = NULL;
    return instr;
}

TacInstruction* tac_assign_var(char *dest, char *src) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_ASSIGN_VAR;
    instr->dest = strdup(dest);
    instr->src1 = strdup(src);
    instr->src2 = NULL;
    instr->int_val = 0;
    instr->op = 0;
    instr->next = NULL;
    return instr;
}

TacInstruction* tac_bin_op(char *dest, char *src1, int op, char *src2) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_BIN_OP;
    instr->dest = strdup(dest);
    instr->src1 = strdup(src1);
    instr->src2 = strdup(src2);
    instr->int_val = 0;
    instr->op = op;
    instr->next = NULL;
    return instr;
}

TacInstruction* tac_label(char *label) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_LABEL;
    instr->dest = strdup(label);
    instr->src1 = NULL;
    instr->src2 = NULL;
    instr->int_val = 0;
    instr->op = 0;
    instr->next = NULL;
    return instr;
}

TacInstruction* tac_jump(char *label) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_JUMP;
    instr->dest = strdup(label);
    instr->src1 = NULL;
    instr->src2 = NULL;
    instr->int_val = 0;
    instr->op = 0;
    instr->next = NULL;
    return instr;
}

TacInstruction* tac_cond_jump(char *src1, int relop, char *src2, char *label) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_COND_JUMP;
    instr->dest = strdup(label);
    instr->src1 = strdup(src1);
    instr->src2 = strdup(src2);
    instr->int_val = 0;
    instr->op = relop;
    instr->next = NULL;
    return instr;
}

TacInstruction* tac_print(char *src) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_PRINT;
    instr->dest = NULL;
    instr->src1 = strdup(src);
    instr->src2 = NULL;
    instr->int_val = 0;
    instr->op = 0;
    instr->next = NULL;
    return instr;
}

TacInstruction* tac_read(char *dest) {
    TacInstruction *instr = (TacInstruction*)malloc(sizeof(TacInstruction));
    instr->opcode = TAC_READ;
    instr->dest = strdup(dest);
    instr->src1 = NULL;
    instr->src2 = NULL;
    instr->int_val = 0;
    instr->op = 0;
    instr->next = NULL;
    return instr;
}
static const char* op_to_string(int op) {
    switch(op) {
        case PLUS: return "+";
        case MINUS: return "-";
        case TIMES: return "*";
        case DIVIDES: return "/";
        case MODULE: return "%";
        case EQUALS: return "==";
        case NOT_EQUAL: return "!=";
        case LESS: return "<";
        case GREATER: return ">";
        case LESS_OR_EQUAL: return "<=";
        case GREATER_OR_EQUAL: return ">=";
        default: return "?";
    }
}

void print_tac_list(TacList *list) {
    if (!list) return;
    
    fprintf(stderr, "\n========== THREE-ADDRESS CODE ==========\n");
    
    TacInstruction *curr = list->head;
    while (curr) {
        switch(curr->opcode) {
            case TAC_ASSIGN_INT:
                fprintf(stderr, "  %s = %d\n", curr->dest, curr->int_val);
                break;
            
            case TAC_ASSIGN_VAR:
                fprintf(stderr, "  %s = %s\n", curr->dest, curr->src1);
                break;
            
            case TAC_BIN_OP:
                fprintf(stderr, "  %s = %s %s %s\n", 
                       curr->dest, curr->src1, op_to_string(curr->op), curr->src2);
                break;
            
            case TAC_LABEL:
                fprintf(stderr, "%s:\n", curr->dest);
                break;
            
            case TAC_JUMP:
                fprintf(stderr, "  goto %s\n", curr->dest);
                break;
            
            case TAC_COND_JUMP:
                fprintf(stderr, "  if %s %s %s goto %s\n",
                       curr->src1, op_to_string(curr->op), curr->src2, curr->dest);
                break;
            
            case TAC_PRINT:
                fprintf(stderr, "  print %s\n", curr->src1);
                break;
            
            case TAC_READ:
                fprintf(stderr, "  read %s\n", curr->dest);
                break;
        }
        curr = curr->next;
    }
    
    fprintf(stderr, "========================================\n\n");
}

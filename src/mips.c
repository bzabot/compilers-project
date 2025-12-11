#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "mips.h"
#include "parser.h"

// MIPS
typedef struct VarNode {
    char *name;
    int offset;
    struct VarNode *next;
} VarNode;

static VarNode *var_list = NULL;
static int current_offset = 0;

static int get_offset(char *name) {
    VarNode *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr->offset;
        }
        curr = curr->next;
    }
    
    VarNode *node = malloc(sizeof(VarNode));
    node->name = strdup(name);
    current_offset -= 4;
    node->offset = current_offset;
    node->next = var_list;
    var_list = node;
    return node->offset;
}

static void load_mips(char *reg, char *var) {
    int off = get_offset(var);
    printf("    lw %s, %d($fp)\n", reg, off);
}

static void store_mips(char *reg, char *var) {
    int off = get_offset(var);
    printf("    sw %s, %d($fp)\n", reg, off);
}

void generate_mips_from_tac(TacList *list) {
    if (!list) return;
    
    // Reset variable list and offset for MIPS generation
    var_list = NULL;
    current_offset = 0;
    
    // MIPS Prologue
    printf(".data\n");
    printf("newline: .asciiz \"\\n\"\n");
    printf(".text\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("    move $fp, $sp\n");
    printf("    subu $sp, $sp, 1000\n");
    
    // Process each TAC instruction
    TacInstruction *curr = list->head;
    while (curr) {
        switch(curr->opcode) {
            case TAC_ASSIGN_INT:
                printf("    li $t0, %d\n", curr->int_val);
                store_mips("$t0", curr->dest);
                break;
            
            case TAC_ASSIGN_VAR:
                load_mips("$t0", curr->src1);
                store_mips("$t0", curr->dest);
                break;
            
            case TAC_BIN_OP:
                load_mips("$t0", curr->src1);
                load_mips("$t1", curr->src2);
                
                switch(curr->op) {
                    case PLUS:
                        printf("    add $t0, $t0, $t1\n");
                        break;
                    case MINUS:
                        printf("    sub $t0, $t0, $t1\n");
                        break;
                    case TIMES:
                        printf("    mul $t0, $t0, $t1\n");
                        break;
                    case DIVIDES:
                        printf("    div $t0, $t0, $t1\n");
                        printf("    mflo $t0\n");
                        break;
                    case MODULE:
                        printf("    div $t0, $t0, $t1\n");
                        printf("    mfhi $t0\n");
                        break;
                }
                
                store_mips("$t0", curr->dest);
                break;
            
            case TAC_LABEL:
                printf("%s:\n", curr->dest);
                break;
            
            case TAC_JUMP:
                printf("    j %s\n", curr->dest);
                break;
            
            case TAC_COND_JUMP:
                load_mips("$t0", curr->src1);
                load_mips("$t1", curr->src2);
                
                switch(curr->op) {
                    case EQUALS:
                        printf("    beq $t0, $t1, %s\n", curr->dest);
                        break;
                    case NOT_EQUAL:
                        printf("    bne $t0, $t1, %s\n", curr->dest);
                        break;
                    case LESS:
                        printf("    blt $t0, $t1, %s\n", curr->dest);
                        break;
                    case GREATER:
                        printf("    bgt $t0, $t1, %s\n", curr->dest);
                        break;
                    case LESS_OR_EQUAL:
                        printf("    ble $t0, $t1, %s\n", curr->dest);
                        break;
                    case GREATER_OR_EQUAL:
                        printf("    bge $t0, $t1, %s\n", curr->dest);
                        break;
                }
                break;
            
            case TAC_PRINT:
                load_mips("$a0", curr->src1);
                printf("    li $v0, 1\n");
                printf("    syscall\n");
                printf("    li $v0, 4\n");
                printf("    la $a0, newline\n");
                printf("    syscall\n");
                break;
            
            case TAC_READ:
                printf("    li $v0, 5\n");
                printf("    syscall\n");
                store_mips("$v0", curr->dest);
                break;
        }
        
        curr = curr->next;
    }
    
    // MIPS Epilogue
    printf("    li $v0, 10\n");
    printf("    syscall\n");
    
    // Cleanup variable list
    VarNode *node = var_list;
    while (node) {
        VarNode *temp = node;
        node = node->next;
        free(temp->name);
        free(temp);
    }
    var_list = NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "parser.h"


typedef struct VarNode {
    char *name;
    int offset;
    struct VarNode *next;
} VarNode;

static VarNode *var_list = NULL;
static int current_offset = 0;

// Helper
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

static void load(char *reg, char *var) {
    int off = get_offset(var);
    printf("    lw %s, %d($fp)\n", reg, off);
}

static void store(char *reg, char *var) {
    int off = get_offset(var);
    printf("    sw %s, %d($fp)\n", reg, off);
}

int temp_count = 0;
int label_count = 0;

int newTemp() {
    return temp_count++;
}

int newLabel() {
    return label_count++;
}

void emit_init() {
    printf(".data\n");
    printf("newline: .asciiz \"\\n\"\n");
    printf(".text\n");
    printf(".globl main\n");
    printf("main:\n");
    // Prologue
    printf("    move $fp, $sp\n");
    printf("    subu $sp, $sp, 1000\n");
}

void emit_end() {
    // Epilogue
    printf("    li $v0, 10\n"); // Exit syscall
    printf("    syscall\n");
}

void emit_assign_int(char *dest, int val) {
    printf("    li $t0, %d\n", val);
    store("$t0", dest);
}

void emit_assign_float(char *dest, float val) {
    emit_assign_int(dest, (int)val);
}

void emit_assign_var(char *dest, char *src) {
    load("$t0", src);
    store("$t0", dest);
}

void emit_bin_op(int op, char *dest, char *s1, char *s2) {
    load("$t0", s1);
    load("$t1", s2);
    
    switch(op) {
        case PLUS:  printf("    add $t0, $t0, $t1\n"); break;
        case MINUS: printf("    sub $t0, $t0, $t1\n"); break;
        case TIMES: printf("    mul $t0, $t0, $t1\n"); break;
        case DIVIDES: printf("    div $t0, $t0, $t1\n"); printf("    mflo $t0\n"); break;
        case MODULE: printf("    div $t0, $t0, $t1\n"); printf("    mfhi $t0\n"); break;
    }
    
    store("$t0", dest);
}

void emit_label(char *label) {
    printf("%s:\n", label);
}

void emit_jump(char *label) {
    printf("    j %s\n", label);
}

void emit_cond_jump(char *s1, int relop, char *s2, char *label) {
    load("$t0", s1);
    load("$t1", s2);
    
    switch(relop) {
        case EQUALS: printf("    beq $t0, $t1, %s\n", label); break;
        case NOT_EQUAL: printf("    bne $t0, $t1, %s\n", label); break;
        case LESS: printf("    blt $t0, $t1, %s\n", label); break;
        case GREATER: printf("    bgt $t0, $t1, %s\n", label); break;
        case LESS_OR_EQUAL: printf("    ble $t0, $t1, %s\n", label); break;
        case GREATER_OR_EQUAL: printf("    bge $t0, $t1, %s\n", label); break;
    }
}

void emit_print(char *src) {
    load("$a0", src);
    printf("    li $v0, 1\n"); // Print Integer
    printf("    syscall\n");
    
    // Print newline
    printf("    li $v0, 4\n");
    printf("    la $a0, newline\n");
    printf("    syscall\n");
}

void emit_read(char *dest) {
    printf("    li $v0, 5\n"); // Read Integer
    printf("    syscall\n");
    store("$v0", dest);
}

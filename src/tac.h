#ifndef TAC_H
#define TAC_H

#include "ast.h"

typedef enum {
    TAC_ASSIGN_INT,      // dest = constant
    TAC_ASSIGN_VAR,      // dest = src
    TAC_BIN_OP,          // dest = src1 op src2
    TAC_LABEL,           // label:
    TAC_JUMP,            // goto label
    TAC_COND_JUMP,       // if src1 relop src2 goto label
    TAC_PRINT,           // print src
    TAC_READ             // read dest
} TacOpcode;

typedef struct TacInstruction {
    TacOpcode opcode;
    char *dest;          // Destination operand
    char *src1;          // Source operand 1
    char *src2;          // Source operand 2
    int int_val;         // Integer constant value
    int op;              // Operator (for binary operations and conditionals)
    struct TacInstruction *next;
} TacInstruction;

typedef struct {
    TacInstruction *head;
    TacInstruction *tail;
} TacList;

TacList* create_tac_list();
void append_tac(TacList *list, TacInstruction *instr);
void print_tac_list(TacList *list);  // Print TAC in human-readable format
void free_tac_list(TacList *list);

TacInstruction* tac_assign_int(char *dest, int val);
TacInstruction* tac_assign_var(char *dest, char *src);
TacInstruction* tac_bin_op(char *dest, char *src1, int op, char *src2);
TacInstruction* tac_label(char *label);
TacInstruction* tac_jump(char *label);
TacInstruction* tac_cond_jump(char *src1, int relop, char *src2, char *label);
TacInstruction* tac_print(char *src);
TacInstruction* tac_read(char *dest);


int newTemp();
int newLabel();



#endif

#ifndef TAC_H
#define TAC_H

#include "ast.h" 

// Generates a new temporary variable name
int newTemp();
// Generates a new label name
int newLabel();

// Initialization and Finalization
void emit_init();
void emit_end();

// Operations
void emit_assign_int(char *dest, int val);
void emit_assign_float(char *dest, float val);
void emit_assign_var(char *dest, char *src);
void emit_bin_op(int op, char *dest, char *s1, char *s2);

// Control Flow
void emit_label(char *label);
void emit_jump(char *label);
void emit_cond_jump(char *s1, int relop, char *s2, char *label);

// IO
void emit_print(char *src);
void emit_read(char *dest);

#endif

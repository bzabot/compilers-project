#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_UNKNOWN
} SymbolType;

typedef struct Entry {
    char *name;         // Variable name
    SymbolType type;    // Variable type
    int offset;         // Memory offset (for code generation)
    struct Entry *next; // Pointer to next entry in case of collision
} Entry;

// Symbol Table Structure (HashMap)
typedef struct {
    Entry **buckets;    // Array of pointers to entries
    int size;           // Size of the bucket array
    int count;          // Total number of symbols
} SymbolTable;


SymbolTable* create_symbol_table(int size);

void insert_symbol(SymbolTable *table, char *name, SymbolType type, int offset);
Entry* lookup_symbol(SymbolTable *table, char *name);
void free_symbol_table(SymbolTable *table);

#endif

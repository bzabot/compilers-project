#include "symbol_table.h"
#include <string.h>
#include <stdio.h>

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

SymbolTable* create_symbol_table(int size) {
    SymbolTable *table = (SymbolTable*) malloc(sizeof(SymbolTable));
    if (!table) return NULL;
    
    table->size = size;
    table->count = 0;
    table->buckets = (Entry**) calloc(size, sizeof(Entry*));
    return table;
}

void insert_symbol(SymbolTable *table, char *name, SymbolType type, int offset) {
    unsigned long index = hash(name) % table->size;
    Entry *current = table->buckets[index];

    // Check if symbol already exists in the bucket (collision chain)
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // Update existing symbol
            current->type = type;
            current->offset = offset;
            return;
        }
        current = current->next;
    }

    // Symbol not found, create new entry
    Entry *new_entry = (Entry*) malloc(sizeof(Entry));
    if (!new_entry) return;

    new_entry->name = strdup(name); // Duplicate string to ensure ownership
    new_entry->type = type;
    new_entry->offset = offset;
    
    // Insert at the head of the linked list (Collision handling: Chaining)
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->count++;
}

Entry* lookup_symbol(SymbolTable *table, char *name) {
    unsigned long index = hash(name) % table->size;
    Entry *current = table->buckets[index];

    // Traverse the linked list at this bucket
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current; // Found
        }
        current = current->next;
    }
    return NULL; // Not found
}

void free_symbol_table(SymbolTable *table) {
    if (!table) return;

    for (int i = 0; i < table->size; i++) {
        Entry *current = table->buckets[i];
        while (current != NULL) {
            Entry *temp = current;
            current = current->next;
            free(temp->name);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

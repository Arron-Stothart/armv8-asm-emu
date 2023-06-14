#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "defs.h"

// Creates new symbol table.
symbol_table newSymbolTable() {
    // Address can be set to 0 since it is never accessed unless given a label.
    symbol empty = (symbol) {.label = '\0', .address = 0};
    return (symbol_table) {.labels = {empty}, .size = 0};
}

// Checks whether a given label is already in the table.
bool hasLabel(symbol_table* table, char* label) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(label, table->labels[i].label)) {
            return true;
        }
    }

    return false;
}

// Returns the address associated with a label or -1 if it is not present.
int32_t getAddress(symbol_table* table, char* label) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(label, table->labels[i].label)) {
            return table->labels[i].address;
        }
    }

    // If label not found.
    return -1;
}

// Adds a label to the table if not already present.
void addLabel(symbol_table* table, char* label, uint32_t address) {
    assert(table->size < MAX_LABELS);
    assert(!(hasLabel(table, label)));

    table->labels[table->size] = (symbol) {.label = label, .address = address};
    table->size++;
}

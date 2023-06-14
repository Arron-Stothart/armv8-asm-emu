#include <stdint.h>
#include <stdbool.h>
#include "defs.h"

// Creates new symbol table.
symbol_table newSymbolTable();

// Checks whether a given label is already in the table.
bool hasLabel(symbol_table* table, char* label);

// Returns the address associated with a label or -1 if it is not present. (Note this int is signed)
int32_t getAddress(symbol_table* table, char* label);

// Adds a label to the table if not already present.
void addLabel(symbol_table* table, char* label, uint32_t address);


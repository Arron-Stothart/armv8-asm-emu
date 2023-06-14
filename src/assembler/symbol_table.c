#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "defs.h"

// Creates new symbol table.
symbol_table newSymbolTable() {
    return (symbol_table) {.first = NULL, .size = 0};
}

// Checks whether a given label is already in the table.
bool hasLabel(symbol_table* st, char* label) {
    symbol* sym = st->first;

	while (sym != NULL) {
		if (strcmp(sym->label, label) == 0) {
			return true;
		}
		sym = sym->next;
	}

    return false;
}


// Add symbol to start of symbol table
void addSymbol(symbol_table* st, uint64_t address, char* label) {
    assert(st->size < MAX_LABELS);

	symbol* sym = malloc(sizeof(symbol));
	sym->label = label;
	sym->address = address;
	sym->next = st->first;
	st->first = sym;
}

// Get adddress in symbol table from label, returns -1 otherwise. Note return int is signed.
int32_t getAddress(symbol_table* st, char* label) {
	symbol* sym = st->first;

	while (sym != NULL) {
		if (strcmp(sym->label, label) == 0) {
			return sym->address;
		}
		sym = sym->next;
	}

	fprintf(stderr, "Label not in symbol table.\n");
    exit(EXIT_FAILURE);
}

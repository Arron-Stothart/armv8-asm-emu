#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <regex.h>
#include <ctypes.h>
#include "defs.h"

// Creates new symbol table.
symbol_table* newSymbolTable() {
    symbol_table* st = (symbol_table*) malloc (sizeof(symbol_table));
    st->first = NULL;
    st->size = 0;
    return st;
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

// Check if token is a label
static bool isLabel(char* token) {
	if (!isalpha(*token)) {
		return false;
	}
	if (token[strlen(token)] != ':') {
		return false;
	}

	regex_t rx;
	int val1;
	int val2;

	// Compile regex (create pattern to process comparisons)
	val1 = regcomp(&rx, "[a-zA-Z_\.]([a-zA-Z0-9$_\.])*.", 0);
	// If returned value is not 0, compilation of regex has failed
	if (val1 != 0) {
		printf("Error in generating pattern.\n");
        exit(EXIT_FAILURE);
	}

	// Match pattern of regex with token
	val2 = regexec(&rx, token, 0, NULL, 0);
	// If returned value is 0, match has been found
	if (val2 == 0) {
		return true;
	}
	return false;
}

// Add symbol to start of symbol table
void addSymbol(symbol_table* st, uint64_t address, char* label) {
    assert(st->size < MAX_LABELS);

	symbol* sym = malloc(sizeof(symbol));
    assert(sym != NULL);
	sym->label = strdup(label);
	sym->address = address;
	sym->next = st->first;
	st->first = sym;
	st->size++;
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

// Takes read files and adds any labels and corresponding addresses to symbol table.
uint8_t populateSymbolTable(char* buffer[MAX_LINES], symbol_table* st, uint8_t numRead) {
	int numInstr = 0;

	for (int i = 0; i < numRead; i++) {
		if (isLabel(buffer[i])) {
			char* label = strdup(buffer[i]);
			// Remove colon since when labels are called don't have colon.
			label[strlen(label) - 1] = '\0';
			addSymbol(st, numInstr * INSTRUCTION_SIZE, label); //! The address might not be correct
		} else {
			// Labels are not instructions (but take up an extra space in assembly code);
			numInstr++;
		}
	}

	return numInstr;
}
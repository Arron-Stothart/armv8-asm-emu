#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>
#include "symbol_table.h"

// Writes n instructions from array into binary file
void writeBinary(char* path, uint32_t* instructions, int n) {
	// Creating the output file
	FILE* output = fopen(path, "w");

	// Check if file has been loaded into memory
	if(output == NULL) {
		fprintf(stderr, "Error opening file at %s\n", path);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		fwrite(&(instructions[i]), sizeof(uint32_t), 1, output);
	}

	fclose(output);
}

// Reads each line of file into buffer with maximum of MAX_LINES lines. Returns number of lines read.
uint8_t readFile(char* buffer[MAX_LINES], char* path) {

	// Input file
	FILE* input = fopen(path, "r");

	// Verifying the input
	if (input == NULL) {
		printf("ERROR: Cannot open file: %s\n", path);
		exit(EXIT_FAILURE);
	}

	// Read content of input file
	uint8_t i = 0;
	while(fgets(buffer[i], MAX_CHARS_IN_LINE, input)) {
		i++;
	}

	fclose(input);
	return i;
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

// Takes read files and adds any labels and corresponding addresses to symbol table.
uint8_t populateSymbolTable(char* buffer[MAX_LINES], symbol_table* st, uint8_t numRead) {
	int numInstr = 0;

	for (int i = 0; i < numRead; i++) {
		if (isLabel(buffer[i])) {
			char* label = buffer[i];
			// Remove colon since when labels are called they are done without colon.
			label[strlen(label) - 1] = '\0';
			addSymbol(st, numInstr * INSTRUCTION_SIZE, label); //! The address might not be correct
		} else {
			// Labels are not instructions (but take up an extra space in assembly code);
			numInstr++;
		}
	}

	return numInstr;
}

// Check if token is a directive (starts with .)
static bool isDirective(char* token) {
	return strncmp(token, ".", 1) == 0;
}

// Use first token to identify type of line (Instruction, Directive, Label)
LINE_TYPE getLineType(char* line) {
	char* saveptr;
    char* token;

	// Get first token from line
    token = strtok_r(line, " ", &saveptr);
	// If first token is null then return with error
    if (token == NULL || strlen(token) == 0) {
        printf("Error in parsing content from line.\n");
        exit(EXIT_FAILURE);
    }

	// Check if token is a label, return accordingly
	if (isLabel(token)) {
		return LABEL;
	}
	// Check if token is a directive, return accordingly
	if (isDirective(token)) {
		return DIRECTIVE;
	}
	// Assumption: All lines are either Instruction, Directive or Label
	return INSTRUCTION;
}

// Returns the hash used to find the instruction function given the opcode.
uint8_t hash(char* t) {

	// Manually check for 'b' and 'br' since they are not long enough to has using the hash function.
	// This avoid accessing memory which is inconsitent (and would break the hash function).
    if (strlen(t) == 1) {
        return 1;
    } else if (strlen(t) == 2) {
        return 3;
    }

	return (110773 * (t[0] ^ t[1]) ^ 63 * (t[2] ^ t[3])) % 59;
}

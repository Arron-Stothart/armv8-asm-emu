#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "defs.h"
#include <regex.h>

// Writes n instructions from array into binary file
void writeBinary(char* path, uint32_t* instructions, int n) {
	// Creating the output file
	FILE* output = fopen(path, "wb");

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

// Check if token is a directive (starts with .)
static bool isDirective(char* token) {
	assert(token != NULL);
	return strncmp(token, ".", 1) == 0;
}

// Checks if operand is a 64bit register
bool is64BitReg(char* operand) {
	assert(operand != NULL);
	return strncmp(islower(operand), "x", 1) == 0;
}

// Returns string for correct zero register
char* giveZeroReg(char* operand) {
	assert(operand != NULL);
	return is64BitReg(operand) ? OPERAND_XZR : OPERAND_WZR;
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

	// Check if token is a directive, return accordingly
	if (isDirective(token)) {
		return DIRECTIVE;
	}
	// Assumption: All lines are either Instruction or directives
	// All labels were remvoed during first pass
	return INSTRUCTION;
}

// Returns the hash used to find the instruction function given the opcode.
uint8_t hash(char* t) {

	// Manually check for 'b' and 'br' since they are not long enough to has using the hash function.
	// This avoid accessing memory which is inconsitent (and would break the hash function).
    if (strlen(t) == 1) {
        return 11;
    } else if (strlen(t) == 2) {
        return 13;
    }

	return (710 * (t[0] ^ t[1]) ^ 21 * (t[2] ^ t[3])) % 59;
}

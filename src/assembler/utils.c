#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <regex.h>
#include "utils.h"

// Writes n instructions from array into binary file
void writebinary(char* path, uint32_t* instructions, int n) {
	// Creating the output file
	FILE* output = fopen(path, "wb");

	// Verifying the output
	if(output == NULL) {
		fprintf(stderr, "Error opening file at %s\n", path);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		fwrite(&(instructions[i]) , sizeof(uint32_t), 1, output);
	}

	fclose(output);
}

// Get opcode type from instruction menumonic
// TODO: remove magic strings, replace with map style structure!, make local to tokenize
OPCODE getopcode(char* mnemonic) {
	if (strcmp(mnemonic, "add") == 0) {
		return ADD;
	}
	if (strcmp(mnemonic, "adds") == 0) {
		return ADDS;
	}
	if (strcmp(mnemonic, "sub") == 0) {
		return SUB;
	}
	if (strcmp(mnemonic, "subs") == 0) {
		return SUBS;
	}
	if (strcmp(mnemonic, "cmp") == 0) {
		return CMP;
	}
	if (strcmp(mnemonic, "cmn") == 0) {
		return CMN;
	}
	if (strcmp(mnemonic, "neg") == 0) {
		return NEG;
	}
	if (strcmp(mnemonic, "negs") == 0) {
		return NEGS;
	}
	if (strcmp(mnemonic, "and") == 0) {
		return AND;
	}
	if (strcmp(mnemonic, "ands") == 0) {
		return ANDS;
	}
	if (strcmp(mnemonic, "bic") == 0) {
		return BIC;
	}
	if (strcmp(mnemonic, "bics") == 0) {
		return BICS;
	}
	if (strcmp(mnemonic, "eor") == 0) {
		return EOR;
	}
	if (strcmp(mnemonic, "eon") == 0) {
		return EON;
	}
	if (strcmp(mnemonic, "orr") == 0) {
		return ORR;
	}
	if (strcmp(mnemonic, "orn") == 0) {
		return ORN;
	}
	if (strcmp(mnemonic, "tst") == 0) {
		return TST;
	}
	if (strcmp(mnemonic, "mvn") == 0) {
		return MVN;
	}
	if (strcmp(mnemonic, "mov") == 0) {
		return MOV;
	}
	if (strcmp(mnemonic, "movn") == 0) {
		return MOV;
	}
	if (strcmp(mnemonic, "movk") == 0) {
		return MOVK;
	}
	if (strcmp(mnemonic, "movz") == 0) {
		return MOVZ;
	}
	if (strcmp(mnemonic, "madd") == 0) {
		return MADD;
	}
	if (strcmp(mnemonic, "msub") == 0) {
		return MSUB;
	}
	if (strcmp(mnemonic, "mul") == 0) {
		return MUL;
	}
	if (strcmp(mnemonic, "mneg") == 0) {
		return MNEG;
	}
	if (strcmp(mnemonic, "b") == 0) {
		return B;
	}
	if (strcmp(mnemonic, "br") == 0) {
		return BR;
	}
	if (strcmp(mnemonic, "b.cond") == 0) {
		return BCOND;
	}
	if (strcmp(mnemonic, "ldr") == 0) {
		return LDR;
	}
	if (strcmp(mnemonic, "str") == 0) {
		return STR;
	}
	if (strcmp(mnemonic, "nop") == 0) {
		return NOP;
	}
	//! and x0 x0 x0 for halt presents an isssue
	//! .int directive may bennefit from being dealt with here?
}

// Check if token is a label 
static bool islabel(char* token) {
	// Return false if first character is not an alphabet (a-z or A-Z)
	if (!isalpha(*token)) {
		return false;
	}
	// Return false if last character is not a colon
	if (token[strlen(token) - 1] != ':') {
		return false;
	}

	// Return true if token matches regex
	regex_t rx;
	int val1;
	int val2;

	// Compile regex (create pattern to process comparisons)
	val1 = regcomp(&rx, "[a-zA-Z_\.]([a-zA-Z0-9$_\.])*.", 0); // Not super sure about this, red '\.'??!!
	// If returned value is not 0, compilation of regex has failed
	if (val1 != 0) {
		printf("Error in generating pattern.\n");
        exit(EXIT_FAILURE);
	}

	// Match pattern of regex with token
	val2 = regexec(&rx, token, 0, NULL, 0); // May be able to use pmatch parameter instead of the above separate checks
	// If returned value is 0, match has been found
	if (val2 == 0) {
		return true;
	}
	return false;
}

// Check if token is a directive
static bool isdirective(char* token) {
	// Return true if first character is '.'
	return (*token == '.');
}

// Use first token to identify type of line (Instruction, Directive, Label)
LINE_TYPE identifyline(char* line) {
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
	if (islabel(token)) {
		return LABEL;
	}
	// Check if token is a directive, return accordingly
	if (isdirective(token)) {
		return DIRECTIVE;
	}
	// Assumption: All lines are either Instruction, Directive or Label
	return INSTRUCTION;
} 
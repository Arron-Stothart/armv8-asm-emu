#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "defs.h"

// Convert a line known to be an instruction into an instruction type
instruction tokenizeInstruction(char* line) {
    char* saveptr;
    char* token;

    // Get first token from line known to be instruction
    token = strtok_r(line, " ", &saveptr); //! Likely an issue with operands of the form [xn, offset]
    // If first token is null then return with error
    if (token == NULL) {
        printf("Error in parsing any token.\n");
        exit(EXIT_FAILURE);
    }

    // Obtain opcode from first token
    char* opcode = token;

    // Get second token (Required by loop structure) and initialise operand array builder
    token = strtok_r(line, " ", &saveptr);
    char* operands[MAX_OPERANDS] = {""}; // initalized as array of empty strings

    // Add operand tokens to operand array builder until end of line
    int i = 0;
    while (token != NULL) {
        // TODO: replace strings with operand type, create function determining operand_type of each
        operands[i] = token;
        token = strtok_r(line, " ", &saveptr);
        i++;
    }

    // Add opcode and operands to inst
    return (instruction) {.opcode = opcode, .operands = operands};
}

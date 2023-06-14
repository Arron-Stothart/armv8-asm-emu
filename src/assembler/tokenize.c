#include <string.h>
#include <stdlib.h>
#include "tokenize.h"

// Convert a line known to be an instruction into an instruction type 
instruction tokenizeinstruction(char* line) {
    char* saveptr;
    char* token;
    instruction inst;

    // Get first token from line known to be instruction
    token = strtok_r(line, " ", &saveptr);
    // If first token is null then return with error
    if (token == NULL) {
        printf("Error in parsing any token.\n");
        exit(EXIT_FAILURE);
    }

    // Obtain opcode from first mneumonic
    OPCODE opcode = getopcode(token);

    // Get second token (Required by loop structure) and initialise operand array builder
    token = strtok_r(line, " ", &saveptr);
    operand_arr operands;
    int i = 0;

    // Add operand tokens to operand array builder until end of line
    while (token != NULL) {
        // TODO: replace strings with operand type, create function determining operand_type of each
        operands[i] = token;
        token = strtok_r(line, " ", &saveptr);
        i++;
    }

    // Add opcode and operands to inst
    instruction inst = { .opcode = opcode, .operands = operands};
    
    // return instruction
    return inst;
}

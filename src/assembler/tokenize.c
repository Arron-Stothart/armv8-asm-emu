#include <string.h>
#include <stdio.h>
#include "tokenize.h"

// Break a line into tokens (label, opcode and operand field(s)) writing to a target array length MAX_WORDS_IN_LINE
// TODO: Fix header, finish, Deal with allocation
/*
void tokenize(char* line) {
    char* saveptr;
    char* token;

    // Get first token from line
    token = strtok_r(line, " ", &saveptr);

    // Get successive tokens until end of line
    int i = 0;
    while (token != NULL) {
        // Add token to target array
        token = strtok_r(line, " ", &saveptr);
        i++;
    }
}
*/

// Convert a line known to be an instruction into an instruction type 
instruction tokenizeinstruction(char* line) {
    char* saveptr;
    char* token;
    instruction inst;

    // Get first token from instruction line
    token = strtok_r(line, " ", &saveptr);
    // If first token is null then return with error
    if (token == NULL) {
        printf("Error in parsing any token.\n");
        exit(EXIT_FAILURE);
    }

    // Add opcode to inst 
    inst.opcode = getopcode(token);

    // Get second token (Required by loop structure) and initialise operand array builder
    token = strtok_r(line, " ", &saveptr);
    operand_arr operands;
    int i = 0;

    // Add operand tokens to operand array builder until end of line
    while (token != NULL) {
        // TODO: replace strings with operand type
        operands[i] = token;
        token = strtok_r(line, " ", &saveptr);
        i++;
    }

    // Add operands to inst
    inst.operands = operands;
    
    // return instruction;
}
#include <string.h>
#include "tokenize.h"

// Break a line into tokens (label, opcode and operand field(s)) writing to a target array length MAX_WORDS_IN_LINE
// TODO: Fix header, finish, Deal with allocation
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
#include <string.h>
#include "tokenize.h"

// Breaks a line into its label, opcode and operand field(s)
// Header types are temporary
void tokenize(char* line) {
    char* saveptr;
    char* token;

    // Get first token from line
    token = strtok_r(line, " ", &saveptr);
    
    // TODO: deal with this token

    // Get successive tokens until end of line
    while (token != NULL) {
        token = strtok_r(line, " ", &saveptr);
         // TODO: deal with this token
    }
}
#include <string.h>
#include "tokenize.h"

// Break a line into tokens (label, opcode and operand field(s)) writing to a target array length MAX_WORDS_IN_LINE
// TODO: Finish
void tokenize(char** target_array, char* line) {
    char* saveptr;
    char* token;

    // Get first token from line
    token = strtok_r(line, " ", &saveptr);

    // Get successive tokens until end of line
    int i = 0;
    while (token != NULL) {
        // Add token to target array
        target_array[i] = token;
        token = strtok_r(line, " ", &saveptr);
        i++;
    }
}
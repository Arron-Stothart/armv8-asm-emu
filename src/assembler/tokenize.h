#include "defs.h"
#include "utils.h"

// Break a line into tokens (label, opcode and operand field(s)) writing to a target array length MAX_WORDS_IN_LINE
void tokenize(char** target_array, char* line);
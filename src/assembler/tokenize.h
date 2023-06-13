#include "defs.h"
#include "utils.h"

// Break a line into its label, opcode and operand field(s) then write to target array
void tokenize(char** target_array, char* line);
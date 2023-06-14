#include <stdint.h>
#include "defs.h"

// Writes n instructions from array into binary file
void writebinary(char* path, uint32_t* instructions, int n);

// Get opcode type from instruction menumonic
OPCODE getOpcode(char* mnemonic);

// Use first token to identify type of line (Instruction, Directive, Label)
LINE_TYPE getlinetype(char* line);

#include <stdint.h>
#include "defs.h"

// Writes n instructions from array into binary file
void writeBinary(char* path, uint32_t* instructions, int n);

// Reads each line of file into buffer with maximum of MAX_LINES lines. Returns number of lines read.
uint8_t readFile(char* buffer[MAX_LINES], char* path);

// Get opcode type from instruction menumonic
OPCODE getOpcode(char* mnemonic);

// Use first token to identify type of line (Instruction, Directive, Label)
LINE_TYPE getLineType(char* line);

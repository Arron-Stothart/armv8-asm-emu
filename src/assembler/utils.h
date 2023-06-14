#include <stdint.h>
#include "defs.h"

// Writes n instructions from array into binary file
void writebinary(char* path, uint32_t* instructions, int n);

// Get opcode type from instruction menumonic
OPCODE getopcode(char* mnemonic);

// Use first token to identify type of line (Instruction, Directive, Label)
LINE_TYPE getlinetype(char* line);

// Add symbol to symbol table
void addsymbol(uint64_t address, char* label, symbol_table* st);

// Get adddress in symbol table from label
uint64_t getaddress(char* label, symbol_table* st);
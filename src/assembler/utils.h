#include <stdint.h>
#include "defs.h"

// Writes n instructions from array into binary file
void writeBinary(char* path, uint32_t* instructions, int n);

// Reads each line of file into buffer with maximum of MAX_LINES lines. Returns number of lines read.
uint8_t readFile(char* buffer[MAX_LINES], char* path);

// Takes read files and adds any labels and corresponding addresses to symbol table.
uint8_t populateSymbolTable(char* buffer[MAX_LINES], symbol_table* st, uint8_t numRead);

// Use first token to identify type of line (Instruction, Directive, Label)
LINE_TYPE getLineType(char* line);

// Returns the hash used to find the instruction function given the opcode.
uint8_t hash(char* t);

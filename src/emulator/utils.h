#include "defs.h"

// Converts hexadecimals in 32bits between endians.
int convert(int value);

// Gets instruction type given instruction in little-endian.
INSTRUCTION_TYPE getInstructionType(int word);

// Outputs state of ARM processor into .out file.
void outputstate(ARM* arm);

// Given a array of memory and a binary file, data from file will be loaded into array.
void loadbinary(char* memory, char* path);

// Returns word from byte addressable memory
int getword(char* memory);
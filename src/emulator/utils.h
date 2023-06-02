#include "defs.h"

// Converts hexadecimals in 32bits between endians.
int convert(int value);

// Gets instruction type given instruction in little-endian.
INSTRUCTION_TYPE getInstructionType(int word);

// Outputs state of ARM processor into .out file.
void outputState(ARM* arm);

// Given a array of memory and a binary file, data from file will be loaded into array.
void loadBinary(char* memory, char* path);

// Returns word from byte addressable memory
int getWord(char* memory);

// Rotate right operations
int rotateRight32(long long int value, int shift);
int rotateRight64(long long int value, int shift);

// Gets bit at kth position from n.
int getBitAt(int n, int k);
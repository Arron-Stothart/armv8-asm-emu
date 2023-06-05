#include "defs.h"

// Converts hexadecimals in 32bits between endians.
int convert(int32_t value);

// Gets instruction type given instruction in little-endian.
INSTRUCTION_TYPE getInstructionType(int32_t word);

// Outputs state of ARM processor into .out file.
void outputState(ARM* arm);

// Given a array of memory and a binary file, data from file will be loaded into array.
void loadBinary(char* memory, char* path);

// Returns word from byte addressable memory
int getWord(char* memory);

// Rotate right operations
int rotateRight64(uint64_t value, int shift);
int rotateRight32(uint64_t value, int shift);

// Arithmetic shift operations
int arithmeticShiftRight64(uint64_t value, int shift);
int arithmeticShiftRight32(uint64_t value, int shift);

// Gets bit at kth position from n.
int getBitAt(int n, int k);

// Gets l bits upwards starting from kth positon of n.
int getBitsAt(int n, int k, int l);

// Sets clearsize bits starting from kth position of n to new to size copysize.
int setBitsTo(int n, int k, int new, int clearsize, int copysize);

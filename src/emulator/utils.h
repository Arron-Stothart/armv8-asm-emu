#include "defs.h"

// Gets instruction type given instruction in little-endian.
INSTRUCTION_TYPE getInstructionType(int32_t word);

// Outputs state of ARM processor into .out file.
void outputState(ARM* arm, char *file);

// Given a array of memory and a binary file, data from file will be loaded into array.
void loadBinary(char* memory, char* path);

// Returns word from byte addressable memory
int getWord(char* memory);

// Returns double word from byte addressable memory
int getDoubleWord(char* memory);

// Rotate right
uint64_t ror(uint64_t value, uint32_t shift, bool is64bit);

// Arithemtic shift right
uint64_t asr(uint64_t value, uint32_t shift, bool is64bit);

// Logical shift left
uint64_t lsl(uint64_t value, uint32_t shift, bool is64bit);
// Logical shift right
uint64_t lsr(uint64_t value, uint32_t shift, bool is64bit);

// Gets bit at kth position from n.
int getBitAt(uint64_t n, int k);

// Gets l bits upwards starting from kth positon of n.
int getBitsAt(uint64_t n, int k, int l);

// Sets clearsize bits starting from kth position of n to new to size copysize.
int setBitsTo(uint64_t n, int k, int new, int clearsize, int copysize);

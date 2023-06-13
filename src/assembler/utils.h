#include <stdint.h>

// Writes n instructions from array into binary file
void writebinary(char* path, uint32_t* instructions, int n);

// Read and send each line to the parser
void readassemblyfile(char* path);
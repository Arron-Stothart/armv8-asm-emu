#include <stdlib.h>
#include <stdbool.h>
#include "defs.h"

// PSTATE tracks flags triggered by last result.
// N - Negative value; Z - Zero value; C - Carry; V - Overflow.
typedef struct {
    bool N;
    bool Z;
    bool C;
    bool V;
} PSTATE;


typedef struct {
    int registers[NUM_OF_REGISTERS];
    int memory[MAX_MEMORY_SIZE];
    PSTATE pstate;
    unsigned int pc;
    unsigned int cir;
} ARM;

// Declared in loadbinary.c
void loadbinary(int* memory, char* path);

// Declared in outputstate.c
void outputstate(ARM* arm);

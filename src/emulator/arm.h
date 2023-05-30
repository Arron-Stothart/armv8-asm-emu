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
    long long int registers[NUM_OF_REGISTERS];
    char memory[MAX_MEMORY_SIZE];
    PSTATE pstate;
    unsigned long long int pc;
    unsigned long long int cir;
} ARM;

// Declared in loadbinary.c
extern void loadbinary(char* memory, char* array);

// Declared in outputstate.c
extern void outputstate(ARM* arm);

// Declared in utlis.c
extern int convert(int value);

#include <stdlib.h>
#include <stdbool.h>
#include "defs.h"

// PSTATE tracks flags triggered by last result
// N - Negative value; Z - Zero value; C - Carry; V - Overflow
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
    int pc;
} ARM;

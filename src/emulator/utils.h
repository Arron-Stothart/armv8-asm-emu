#include "defs.h"

// Converts hexadecimals in 32bits between endians.
int convert(int value);

// Gets instruction type given instruction in little-endian.
INSTRUCTION_TYPE getInstructionType(int word);

    
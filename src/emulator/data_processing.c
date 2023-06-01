#include "defs.h"

// WIP
void dataProcessingImmediate(ARM* arm, int instruction) {
    int sf = (instruction >> 31) & 1;
    int opc = (instruction >> 29) & 3;
    int opi = (instruction >> 23) & 7;
    int operand = (instruction >> 5) & 0xFFFF1;
}
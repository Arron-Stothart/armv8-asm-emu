#include "defs.h"

// WIP
void dataProcessingImmediate(ARM* arm, int instruction) {
    int sf = (instruction >> 31) & 1;
    int opc = (instruction >> 29) & 3;

    bool setFlags = opc & 1;
    // true if add-ing, false if sub-ing
    bool addOrSub = (opc & 2) >> 1;

    int opi = (instruction >> 23) & 7;
    int operand = (instruction >> 5) & ((1 << 17) - 1);
    int rd = instruction & 15;

    if (sf) {
        // accessed as 64 bits
        
    } else {
        // accessed as 32 bits

    }

    int rd = 0;

    if (addOrSub) {
        // add
        rd = rn + op2;
    } else {
        // sub
        rd = rn - op2;
    }

    if (setFlags) {
        // set flags

    } else {
        // do not set flags

    }

}
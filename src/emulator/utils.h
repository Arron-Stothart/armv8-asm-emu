#include "defs.h"

// Converts hexadecimals in 32bits between endians.
int convert(int value) {
    int converted = 0;

    // Bitshifts for bytes.
    converted |= ((0x000000ff & value) << 24);
    converted |= ((0x0000ff00 & value) << 8);
    converted |= ((0x00ff0000 & value) >> 8);
    converted |= ((0xff000000 & value) >> 24);

    return converted;
}

// Gets instruction type given instruction in little-endian.
INSTRUCTION_TYPE getInstructionType(int word) {
    unsigned int instruction = convert(word);
    unsigned int op0 = (instruction >> 25) & 0x1111; // TODO: test this

    if (instruction == HALT_CODE) {
        return HALT;
    } else if (instruction == NOP_CODE) {
        return NOP;
    } else if ((op0 & 0x1110) == 0x1000) {
        return DATA_PROCESSING_IMMEDIATE;
    } else if ((op0 & 0x0111) == 0x0101) {
        return DATA_PROCESSING_REGISTER;
    } else if ((op0 & 0x0101) == 0x0100) {
        return SINGLE_DATA_TRANSFER;
    } else if ((op0 & 0x1110) == 0x1010) {
        return BRANCH;
    }
}

    
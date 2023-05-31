#include "defs.h"

// Gets branch type from instruction 
BRANCH_TYPE getBranchType(int instruction) {
    // TODO: Implement
    return -1;
}

// Execute branch instruction
void executeBranch(int offset, ARM* arm, int instruction) { // Perhaps take PC rather than entire ARM
    BRANCH_TYPE type = getBranchType(instruction);
    switch (type) {
        case UNCONDITIONAL:
            unsigned int simm26 = instruction & 0x03ffffff // TODO: Test
            break;
        case REGISTER:
            unsigned int xn = instruction & 0x000003e0 // TODO: Test
            break;
        case CONDITIONAL:
            unsigned int simm19 = instruction & 0x00ffffe0 // TODO: Test
            unsigned int cond = instruction & 0x0000000f // TODO: Test
            break;
    }
} 
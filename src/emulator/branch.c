#include <assert.h>
#include "defs.h"

// Gets branch type from instruction 
static BRANCH_TYPE getBranchType(int instruction) {
    // TODO: Implement
    return -1;
}

// Determine if ARM PSTATE satisfies cond
static bool conditionCheck(int cond, ARM* arm) {
    int n = (*arm).pstate.N;
    int z = (*arm).pstate.Z; 
    // int c = (*arm).pstate.C; not needed in branch
    int v = (*arm).pstate.V;  

    switch (cond) {
        // EQ (Equal)
        case BR_EQ: 
            return (z == 1);
        // NE (Not Equal)
        case BR_NE:
            return (z == 0);
        // GE (Signed greater or equal)
        case BR_GE:
            return (n == 1);
        // LT (Signed less than)
        case BR_LT:
            return (n != 1);
        // GT (Signed greater than)
        case BR_GT:
            return (z == 0 && n == v);
        // LE (Signed less than or equal)
        case BR_LE:
            return (!(z == 0 && n == v));
        // AL (always)
        case BR_AL:
            return 1;
        // Condition not of permitted type
        default:
            return -1;
    }
}

// Execute branch instruction TODO: split up into case specific 
void executeBranch(ARM* arm, int instruction) { 

    // Get type of branch instruction
    BRANCH_TYPE type = getBranchType(instruction);

    switch (type) {
        case UNCONDITIONAL:
            int simm26 = instruction & 0x03ffffff; // TODO: Test
            int offset = simm26 * BYTES_IN_WORD;
            // Branch to address encoded by literal
            (*arm).pc += (offset - BYTES_IN_WORD);
            break;
        case REGISTER:
            // Determining encoding of register Xn
            int xn = (instruction & 0x000003e0 >> 5);  // TODO: Test
            // Check if xn refers to an exisiting register
            assert(xn >= 0 && xn < NUM_OF_REGISTERS);
            // Branch to address stored in Xn
            (*arm).pc = (*arm).registers[xn];
            break;
        case CONDITIONAL:
            int simm19 = (instruction & 0x00ffffe0 >> 5); // TODO: Test
            int cond = instruction & 0x0000000f; // TODO: Test
            // TODO: Check if condition is of some allowed type
            if (conditionCheck(cond, arm)) {
                int offset = simm19 * BYTES_IN_WORD;
                // Branch to address encoded by literal
                (*arm).pc += (offset - BYTES_IN_WORD);
            }
            break;
    }
} 
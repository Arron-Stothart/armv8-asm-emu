#include "defs.h"

// Gets branch type from instruction 
BRANCH_TYPE getBranchType(int instruction);

// Execute branch instruction
void executeBranch(int offset, ARM* arm, int instruction); //Perhaps take PC rather than entire ARM
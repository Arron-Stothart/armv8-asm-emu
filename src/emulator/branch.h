#include "defs.h"

// Gets branch type from instruction 
BRANCH_TYPE getBranchType(int instruction);

// Determine if ARM PSTATE satisfies cond
bool conditionCheck(int cond, ARM* arm);

// Execute branch instruction
void executeBranch(int offset, ARM* arm, int instruction); 
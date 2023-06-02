#include "defs.h"
#include "utils.h"

// Execute single data transfer.
void singleDataTransfer(ARM* arm, int instruction) {
    int lbit = getBitAt(instruction, SDT_LBIT);
    int ubit = getBitAt(instruction, SDT_UBIT);
    int sfbit = getBitAt(instruction, SDT_SFBIT);

    int targetreg = getBitsAt(instruction, SDT_RT_START, REG_INDEX_SIZE);
    int basereg = getBitsAt(instruction, SDT_XN_START, REG_INDEX_SIZE);
}
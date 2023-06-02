#include "defs.h"
#include "utils.h"

// WIP
void dataProcessingImmediate(ARM* arm, int instruction) {
    int sf = getBitAt(instruction, DPI_SFBIT);
    int opc = getBitsAt(instruction, DPI_OPC_START, DPI_OPC_LEN);

    bool setFlags = opc & 1;
    // true if add-ing, false if sub-ing
    bool addOrSub = (opc & 2) >> 1;

    int opi = getBitsAt(instruction, DPI_OPI_START, DPI_OPI_LEN);
    int operand = getBitsAt(instruction, DPI_OPERAND_START, DPI_OPERARND_LEN);
    int rd = getBitsAt(instruction, DPI_RD_START, REG_INDEX_SIZE);

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
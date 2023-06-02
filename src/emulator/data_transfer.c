#include "defs.h"
#include "utils.h"

static TRANSFER_TYPE getTransferType(int instruction) {
    // TODO 
    return -1;
}

// Execute single data transfer.
void singleDataTransfer(ARM* arm, int instruction) {
    // int lbit = getBitAt(instruction, SDT_LBIT);
    // int ubit = getBitAt(instruction, SDT_UBIT);
    // int sfbit = getBitAt(instruction, SDT_SFBIT);
    // int ibit = getBitAt(instruction, SDT_IBIT);

    // int targetreg = getBitsAt(instruction, SDT_RT_START, REG_INDEX_SIZE);
    // int basereg = getBitsAt(instruction, SDT_XN_START, REG_INDEX_SIZE);

    TRANSFER_TYPE type = getTransferType(instruction);

    switch (type) {
        case REGISTER_OFFSET:
            break;
        case UNSIGNED_OFFSET:
            break;
        case PRE_INDEX:
            break;
        case POST_INDEX:
            break;
        case LITERAL_ADDRESS:
            break;
    }
}
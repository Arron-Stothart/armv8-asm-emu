#include "defs.h"
#include "utils.h"

static TRANSFER_TYPE getTransferType(int instruction) {
    // TODO
    return -1;
}

// Execute single data transfer.
void singleDataTransfer(ARM* arm, int instruction) {
    int lbit = getBitAt(instruction, SDT_LBIT_POS);
    int ubit = getBitAt(instruction, SDT_UBIT_POS);
    int sfbit = getBitAt(instruction, SDT_SFBIT_POS);
    int ibit = getBitAt(instruction, SDT_IBIT_POS);
    int rt = getBitsAt(instruction, SDT_RT_START, REG_INDEX_SIZE);
    int xn = getBitsAt(instruction, SDT_XN_START, REG_INDEX_SIZE);

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

#include "defs.h"
#include "utils.h"

static TRANSFER_TYPE getTransferType(int instruction) {
    int u = getBitAt(instruction, SDT_UBIT_POS);
    int i = getBitAt(instruction, SDT_IBIT_POS);

    // If u is given then unsigned offset.
    if (u) {
        return UNSIGNED_OFFSET;
    }

    if (getBitAt(instruction, SDT_LOADLITERAL_BIT)) {
        return LITERAL_ADDRESS;
     }

    // If the first bit is 0 then register offset, else pre/post index.
    if (getBitAt(instruction, SDT_REGISTEROFFSET_BIT) == 0) {
        return REGISTER_OFFSET;
    }

    // If i is given then pre-index otherwise post-index (default).
    if (i) {
        return PRE_INDEX;
    }

    return POST_INDEX;
}

// Execute single data transfer.
void singleDataTransfer(ARM* arm, int instruction) {
    int l = getBitAt(instruction, SDT_LBIT_POS);
    int sf = getBitAt(instruction, SDT_SFBIT_POS);
    int rt = getBitsAt(instruction, SDT_RT_START, REG_INDEX_SIZE);
    int xn = getBitsAt(instruction, SDT_XN_START, REG_INDEX_SIZE);
    uint64_t address;

    TRANSFER_TYPE type = getTransferType(instruction);

    switch (type) {
        case UNSIGNED_OFFSET: {
            uint64_t imm12 = getBitsAt(instruction, SDT_IMM12_START, IMM12_LEN);
            address = arm->registers[xn] + imm12;
            break;
        }

        case PRE_INDEX: {
            int64_t simm9 = getBitsAt(instruction, SDT_SIMM9_START, SIMM9_LEN);
            arm->memory[xn] += simm9;
            address = arm->registers[xn] + simm9;
            break;
        }
        case POST_INDEX: {
            int64_t simm9 = getBitsAt(instruction, SDT_SIMM9_START, SIMM9_LEN);
            address = arm->registers[xn];
            arm->memory[xn] += simm9;
            break;
        }
        case REGISTER_OFFSET: {
            int xm = getBitsAt(instruction, SDT_XM_START, REG_INDEX_SIZE);
            address = arm->registers[xn] + arm->registers[xm];
            break;
        }
        case LITERAL_ADDRESS:{
            int64_t simm19 = getBitsAt(instruction, SDT_SIMM9_START, SIMM9_LEN);
            address = arm->pc + (simm19 * BYTES_IN_WORD);
            break;
        }

        // If load bit is given then load, else store.
        if (l) {
            // Load
            if (sf) {
                // In 64 bit load double word at address memory into register.
                arm->registers[rt] = getDoubleWord(&arm->memory[address]);
            } else {
                // In 32 bit load word at address memory into register.
                arm->registers[rt] = getWord(&arm->memory[address]);
            }

        } else {
            // Store
            if (sf) {
                // In 64 bit store rt into 8 bytes at address

            } else {
                // In 32 bit store rt into 4 bytes at address

            }
        }
    }
}

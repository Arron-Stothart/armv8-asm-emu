#include <stdbool.h>
#include <stdint.h>
#include "defs.h"
#include "utils.h"

static void movz(ARM* arm, int rd, int op, int hw) {
    arm->memory[rd] = op;
}

static void movn(ARM* arm, int rd, int op, int hw) {
    arm->memory[rd] = ~op;
}

static void movk(ARM* arm, int rd, int op, int hw) {
    setBitsTo(arm->memory[rd], hw * 16, op);
}

static int add(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = arm->memory[rn] + op2;
    arm->registers[rd] = r;
    return r;
}

static int sub(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = arm->memory[rn] - op2;
    arm->registers[rd] = r;
    return r;
}

static int adds(ARM* arm, int rd, int rn, int op2, int sf) {
    // If rd is the zero register then we compute result without changing memory.
    int r = (rd == ZR_INDEX) ? arm->memory[rn] + op2 : add(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? ((int64_t) r < 0) : ((int32_t) r < 0);
    arm->pstate.C = 0;
    arm->pstate.V = 0;
}

static int subs(ARM* arm, int rd, int rn, int op2, int sf) {
    // If rd is the zero register then we compute result without changing memory.
    int r = (rd == ZR_INDEX) ? arm->memory[rn] - op2 : sub(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? ((int64_t) r < 0) : ((int32_t) r < 0);
    arm->pstate.C = 0;
    arm->pstate.V = 0;
}

static void (*logicalImmediate[3])(ARM* arm, int rd, int op, int hw) = {
    &movz, &movn, &movk
};

static int (*arithmeticImmediate[4])(ARM* arm, int rd, int rn, int op2, int sf) = {
    &add, &adds, &sub, &subs
};

void dataProcessingImmediate(ARM* arm, int instruction) {
    int sf = getBitAt(instruction, DPI_SFBIT);
    int opc = getBitsAt(instruction, DPI_OPC_START, DPI_OPC_LEN);
    int opi = getBitsAt(instruction, DPI_OPI_START, DPI_OPI_LEN);
    int rd = getBitsAt(instruction, DPI_RD_START, REG_INDEX_SIZE);

    // Access rd as 32 bit register if neccessary
    // Only if rd is not the zero register, since ZR is the same in 32 and 64 bits.
    if (!sf && rd != ZR_INDEX) {
        arm->memory[rd] &= WREGISTER_MASK;
    }

    switch (opi) {

        // Arithmetic
        case DPI_ARITHMETIC_OPI: {
            int sh = getBitAt(instruction, DPI_SHBIT);
            int imm12 = getBitsAt(instruction, DPI_IMM12_START, IMM12_LEN);
            int rn = getBitsAt(instruction, DPI_RN_START, REG_INDEX_SIZE);

            // Shift imm12 by 12 if shift bit is given.
            if (sh) {
                imm12 <<= 12;
            }

            // Index 32 encodes ZR for arithmetic instructions which change PSTATE.
            // Opc starts with 1 for adds and subs, which change PSTATE flags.
            // Only compute if destination is not ZR or operation changes flags.
            if (rd != ZR_INDEX || getBitAt(opc, 1) == 1) {
                arithmeticImmediate[opc](arm, rd, rn, imm12, sf);
            }

            break;
        }

        // Wide Move
        case DPI_WIDEMOVE_OPI: {
            int hw = getBitsAt(instruction, DPI_HW_START, DPI_HW_SIZE);
            int imm16 = getBitsAt(instruction, DPI_IMM16_START, IMM16_LEN);

            // For movk don't shift imm16.
            if (opc != DPI_MOVK_OPC) {
                imm16 <<= (hw * 16);
            }

            // Last rd index encodes ZR for wide move processing.
            // No need to compute logical instruction when rd = ZR since write
            // is ignored and PSTATE isn't changed.
            if (rd != ZR_INDEX) {
                logicalImmediate[opc](arm, rd, imm16, hw);
            }

            break;
        }
    }
}

#include <stdbool.h>
#include "defs.h"
#include "utils.h"

static void setFlag(bool* flag, bool condition) {
    flag = condition ? 1 : 0;
}

void movz(ARM* arm, int rd, int op, int hw) {
    arm->memory[rd] = op;
}

void movn(ARM* arm, int rd, int op, int hw) {
    arm->memory[rd] = ~op;
}

void movk(ARM* arm, int rd, int op, int hw) {
    setBitsTo(arm->memory[rd], hw * 16, op);
}

int add(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = arm->memory[rn] + op2;
    arm->registers[rd] = r;
    return r;
}

int sub(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = arm->memory[rn] - op2;
    arm->registers[rd] = r;
    return r;
}

int adds(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = add(arm, rd, rn, op2, sf);

    // // Sets flags for PSTATE
    // setFlag(arm->pstate.Z, r == 0);
    // setFlag(arm->pstate.N, getBitAt())
}

int subs(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = sub(arm, rd, rn, op2, sf);

    // // Set flags for PSTATE
    // setFlag(arm->pstate.Z, r == 0);
    // setFlag(arm->pstate.Z, );
}

void (*logicalImmediate[3])(ARM* arm, int rd, int op, int hw) = {
    &movz, &movn, &movk
};

int (*arithmeticImmediate[4])(ARM* arm, int rd, int rn, int op2, int sf) = {
    &add, &adds, &sub, &subs
};

void dataProcessingImmediate(ARM* arm, int instruction) {
    int sf = getBitAt(instruction, DPI_SFBIT);
    int opc = getBitsAt(instruction, DPI_OPC_START, DPI_OPC_LEN);
    int opi = getBitsAt(instruction, DPI_OPI_START, DPI_OPI_LEN);
    int rd = getBitsAt(instruction, DPI_RD_START, REG_INDEX_SIZE);

    // Access rd as 32 bit register
    if (!sf) {
        rd &= WREGISTER_MASK;
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

            // Index 32 encodes ZR for arithmetic instructiosn which change PSTATE.
            // Opc starts with 1 for adds and subs, which change PSTATE flags.
            if (rd == ZR_INDEX && getBitAt(opc, 1) == 1) {
                rd = 0;
            }

            arithmeticImmediate[opc](arm, rd, rn, imm12, sf);
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

            // Index 32 encodes ZR for logical instructions.
            if (rd == ZR_INDEX) {
                rd = 0;
            }

            logicalImmediate[opc](arm, rd, imm16, hw);
            break;
        }
    }
}

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "defs.h"
#include "utils.h"

static void movz(ARM* arm, int rd, int op, int hw) {
    arm->memory[rd] = op;
}

static void movn(ARM* arm, int rd, int op, int hw) {
    arm->memory[rd] = ~op;
}

static void movk(ARM* arm, int rd, int op, int hw) {
    setBitsTo(arm->memory[rd], hw * 16, op, IMM16_LEN, IMM16_LEN);
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
    int rncontent = arm->memory[rn];
    int r = (rd == ZR_INDEX) ? rncontent + op2 : add(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? ((int64_t) r < 0) : ((int32_t) r < 0);
    // Unsigned overflow if carry bit is produced
    arm->pstate.C = sf ?
        ((rncontent > 0 && op2 > ULLONG_MAX - rncontent) ||
        (rncontent < 0 && op2 < (-ULLONG_MAX + 1) - rncontent)) : //! No ULLONG_MIN so test
        ((rncontent > 0 && op2 > INT_MAX - rncontent) ||
        (rncontent < 0 && op2 < INT_MIN - rncontent));;
    // Signed overflow/underflow if signs of operand are diferent from result
    arm->pstate.V = ((rncontent > 0 && op2 > 0 && r < 0) || (rncontent < 0 && op2 < 0 && r > 0));
}

static int subs(ARM* arm, int rd, int rn, int op2, int sf) {
    // If rd is the zero register then we compute result without changing memory.
    int rncontent = arm->memory[rn];
    int r = (rd == ZR_INDEX) ? arm->memory[rn] - op2 : sub(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? ((int64_t) r < 0) : ((int32_t) r < 0);
    // Unsigned overflow if subtraction produced a borrow
    arm->pstate.C = sf ?
        ((rncontent < 0 && op2 > ULLONG_MAX + rncontent) ||
        (rncontent > 0 && op2 < (-ULLONG_MAX + 1) + rncontent)) :
        ((rncontent < 0 && op2 > INT_MAX + rncontent) ||
        (rncontent > 0 && op2 < INT_MIN + rncontent));
    // Signed overflow/underflow if signs of operand are diferent from result
    arm->pstate.V = ((rncontent > 0 && op2 > 0 && r < 0) || (rncontent < 0 && op2 < 0 && r > 0));
}

static void (*wideMoveImmediate[3])(ARM* arm, int rd, int op, int hw) = {
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

            // If sf is not given, read rd and rn as 32 bit; rn to be restored later.
            int rntemp;
            if (!sf) {
                rntemp = arm->memory[rn];
                arm->memory[rd] &= WREGISTER_MASK;
                arm->memory[rn] &= WREGISTER_MASK;
            }

            // Index 32 encodes ZR for arithmetic instructions which change PSTATE.
            // Opc starts with 1 for adds and subs, which change PSTATE flags.
            // Only compute if destination is not ZR or operation changes flags.
            if (rd != ZR_INDEX || getBitAt(opc, 0) == 0b1) {
                arithmeticImmediate[opc](arm, rd, rn, imm12, sf);
            }

            // Restore rn if it was read as 32 bit;
            if (!sf) {
                arm->memory[rn] = rntemp;
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

            // Read rd as 32 bit register if sf is not given.
            if (!sf) {
                arm->memory[rd] &= WREGISTER_MASK;
            }

            // Last rd index encodes ZR for wide move processing.
            // No need to compute logical instruction when rd = ZR since write
            // is ignored and PSTATE isn't changed.
            if (rd != ZR_INDEX) {
                wideMoveImmediate[opc](arm, rd, imm16, hw);
            }

            break;
        }
    }

    // Write to rd as a 32 bit register if sf is not given (fixes overflows).
    if (!sf) {
        arm->memory[rd] &= WREGISTER_MASK;
    }
}

static int and(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = arm->memory[rn] & op2;
    arm->memory[rd] = r;
    return r;
}

static int bic(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = arm->memory[rn] & ~op2;
    arm->memory[rd] = r;
    return r;
}

static int orr(ARM* arm, int rd, int rn, int op2, int sf) {
    arm->memory[rd] = arm->memory[rn] | op2;
}

static int orn(ARM* arm, int rd, int rn, int op2, int sf) {
    arm->memory[rd] = arm->memory[rn] | ~op2;
}

static int eon(ARM* arm, int rd, int rn, int op2, int sf) {
    arm->memory[rd] = arm->memory[rn] ^ ~op2;
}

static int eor(ARM* arm, int rd, int rn, int op2, int sf) {
    arm->memory[rd] = arm->memory[rn] ^ op2;
}

static int ands(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = (rd == ZR_INDEX) ? arm->memory[rn] & op2 : and(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? ((int64_t) r < 0) : ((int32_t) r < 0);
    // C and V are set to 0.
    arm->pstate.C = 0;
    arm->pstate.V = 0;
}

static int bics(ARM* arm, int rd, int rn, int op2, int sf) {
    int r = (rd == ZR_INDEX) ? arm->memory[rn] & ~op2 : bic(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? ((int64_t) r < 0) : ((int32_t) r < 0);
    // C and V are set to 0.
    arm->pstate.C = 0;
    arm->pstate.V = 0;
}

static void madd(ARM* arm, int rd, int rn, int ra, int rm, int sf) {
    arm->memory[rd] = arm->memory[ra] + (arm->memory[rn] * arm->memory[rm]);
}

static void msub(ARM* arm, int rd, int rn, int ra, int rm, int sf) {
    arm->memory[rd] = arm->memory[ra] - (arm->memory[rn] * arm->memory[rm]);
}

static int (*arithmeticLogicalRegister[8])(ARM* arm, int rd, int rn, int op2, int sf) = {
    &and, &bic, &orr, &orn, &eon, &eor, &ands, &bics
};

static void (*mutiplyRegister[2])(ARM* arm, int rd, int rn, int ra, int rm, int sf) = {
    &madd, &msub
};

static void (*shiftRm32[4])(int rm, int imm6) = {

};

static void (*shiftRm64[4])(int rm, int imm6) = {

};

// Execute data processing register instructions.
void dataProcessingRegister(ARM* arm, int instruction) {
    int sf = getBitAt(instruction, DPR_SFBIT_POS);
    int opr = getBitsAt(instruction, DPR_OPR_START, DPR_OPR_LEN);
    int rd = getBitsAt(instruction, DPR_RD_START, REG_INDEX_SIZE);
    int rm = getBitsAt(instruction, DPR_RM_START, REG_INDEX_SIZE);
    int rn = getBitsAt(instruction, DPR_RN_START, REG_INDEX_SIZE);

    switch (opr) {

        // Multiply
        case DPR_MULTIPLY_OPR: {
            int ra = getBitsAt(instruction, DPR_RA_START, REG_INDEX_SIZE);
            int x = getBitAt(instruction, DPR_XBIT_POS);

            if (rd != ZR_INDEX) {
                mutiplyRegister[x](arm, rd, rn, ra, rm, sf);
            }

            break;
        }

        // Arithemtic and Logical
        default: {
            int shift = getBitsAt(instruction, DPR_SHIFT_START, DPR_SHIFT_LEN);
            int n = getBitAt(instruction, DPR_NBIT_POS);
            int opc = getBitsAt(instruction, DPR_OPC_START, DPR_OPC_LEN);

            int op2 = rm; //TODO add shifts

            switch (rm) {
                // Logical shift left by imm6 bits
                case DPR_LSL_SHIFT:
                    break;
                // Logical shift right by imm6 bits
                case DPR_LSR_SHIFT:
                    break;
                // Arithmetic shift right by imm6 bits
                case DPR_ASR_SHIFT:
                    break;
                // Rotate right by imm6 bits
                case DPR_ROR_SHIFT:
                    break;
            }

            // Negative bits if N bit is given (logical operations).
            if (n) {
                op2 = ~op2;
            }

            // Index 32 encodes ZR for arithmetic instructions which change PSTATE.
            // Opc starts with 0b11 for ands and bics, which change PSTATE flags.
            // Only compute if destination is not ZR or operation changes flags
            if (rd != ZR_INDEX || getBitsAt(opc, 0, 2) == 0b11) {
                arithmeticLogicalRegister[opc](arm, rd, rn, rm, sf);
            }

            break;
        }

    }

}

#include "defs.h"
#include "utils.h"

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

static int (*shiftRm32[4])(int rm, int imm6) = {

};

static int (*shiftRm64[4])(int rm, int imm6) = {

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

            // If sf is not given, read registers as 32 bit; all but rd to be restored later.
            int rntemp;
            int rmtemp;
            int ratemp;
            if (!sf) {
                rntemp = arm->memory[rn];
                ratemp = arm->memory[ra];
                rmtemp = arm->memory[rm];
                arm->memory[rd] &= WREGISTER_MASK;
                arm->memory[rn] &= WREGISTER_MASK;
                arm->memory[ra] &= WREGISTER_MASK;
                arm->memory[rm] &= WREGISTER_MASK;
            }

            // ZR is read-only and no flags will be changed.
            if (rd != ZR_INDEX) {
                mutiplyRegister[x](arm, rd, rn, ra, rm, sf);
            }

            // Restore registers read as 32 bit;
            if (!sf) {
                arm->memory[rn] = rntemp;
                arm->memory[ra] = ratemp;
                arm->memory[rm] = rmtemp;
            }

            break;
        }

        // Arithemtic and Logical
        default: {
            int shift = getBitsAt(instruction, DPR_SHIFT_START, DPR_SHIFT_LEN);
            int n = getBitAt(instruction, DPR_NBIT_POS);
            int opc = getBitsAt(instruction, DPR_OPC_START, DPR_OPC_LEN);
            int imm6 = getBitsAt(instruction, DPR_IMM6_START, IMM6_LEN);

            // If sf is not given, read registers as 32 bit; all but rd to be restored later.
            int rntemp;
            int rmtemp;
            if (!sf) {
                rntemp = arm->memory[rn];
                rmtemp = arm->memory[rm];
                arm->memory[rd] &= WREGISTER_MASK;
                arm->memory[rn] &= WREGISTER_MASK;
                arm->memory[rm] &= WREGISTER_MASK;
            }

            // Shift rm by imm6 with type depending on shift bits.
            int op2 = sf ? shiftRm64[shift](rm, imm6) : shiftRm32[shift](rm, imm6);

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

            // Restore registers read as 32 bit;
            if (!sf) {
                arm->memory[rn] = rntemp;
                arm->memory[rm] = rmtemp;
            }

            break;
        }
    }

    // Write to rd as a 32 bit register if sf is not given (fixes overflows).
    if (!sf) {
        arm->memory[rd] &= WREGISTER_MASK;
    }
}

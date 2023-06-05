#include "defs.h"
#include "utils.h"

// WIP
void dataProcessingImmediate(ARM* arm) {

}

static int and(ARM* arm, int rd, int rn, int op2, int sf) {
    arm->memory[rd] = arm->memory[rn] & op2;
}

static int bic(ARM* arm, int rd, int rn, int op2, int sf) {
    arm->memory[rd] = arm->memory[rn] & ~op2;
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
    int result = and(arm, rd, rn, op2, sf);
    // TODO: Make 'update flags' function
    arm->pstate.N = result > 0; 
    arm->pstate.Z = result = 0;
    arm->pstate.C = 0;
    arm->pstate.V = 0;
}

static int bics(ARM* arm, int rd, int rn, int op2, int sf) {
    int result = bic(arm, rd, rn, op2, sf);
    // TODO: Make 'update flags' function
    arm->pstate.N = result > 0;
    arm->pstate.Z = result = 0;
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

            mutiplyRegister[x](arm, rd, rn, ra, rm, sf);
            break;
        }

        // Arithemtic and Logical
        default: {
            int shift = getBitsAt(instruction, DPR_SHIFT_START, DPR_SHIFT_LEN);
            int n = getBitAt(instruction, DPR_NBIT_POS);
            int opc = getBitsAt(instruction, DPR_OPC_START, DPR_OPC_LEN);

            int op2 = rm; //TODO add shifts

            // Negative bits if N bit is given (logical operations).
            if (n) {
                op2 = ~op2;
            }


            arithmeticLogicalRegister[opc](arm, rd, rn, rm, sf);
            break;
        }

    }

}

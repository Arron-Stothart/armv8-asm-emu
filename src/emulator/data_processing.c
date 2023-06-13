#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "defs.h"
#include "utils.h"

/*
Operations
*/

static void movz(ARM* arm, int rd, uint64_t op, int hw) {
    arm->registers[rd] = op;
    fputs("(movz)", stderr);
}

static void movn(ARM* arm, int rd, uint64_t op, int hw) {
    arm->registers[rd] = ~op;
    fputs("(movn)", stderr);
}

static void movk(ARM* arm, int rd, uint64_t op, int hw) {
    arm->registers[rd] = setBitsTo(arm->registers[rd], (hw + 1) * DPI_MOVK_OFFSET, op, IMM16_LEN);
    fputs("(movk)", stderr);
}

static uint64_t add(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    uint64_t r = arm->registers[rn] + op2;
    arm->registers[rd] = r;
    fputs("(add)", stderr);
    return r;
}

static uint64_t sub(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    uint64_t r = arm->registers[rn] - op2;
    arm->registers[rd] = r;
    fputs("(sub)", stderr);
    return r;
}

static uint64_t adds(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    // If rd is the zero register then we compute result without changing memory.
    int64_t rncontent = arm->registers[rn];
    int64_t r = (rd == ZR_INDEX) ? rncontent + op2 : add(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? (r < 0) : ((int32_t) r < 0);
    // Unsigned overflow if carry bit is produced
    arm->pstate.C = 0;
    // Signed overflow/underflow if signs of operand are diferent from result
    arm->pstate.V = 0;
    fputs("(adds)", stderr);
    return EXIT_SUCCESS;
}

static uint64_t subs(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    // If rd is the zero register then we compute result without changing memory.
    int64_t rncontent = arm->registers[rn];
    int64_t r = (rd == ZR_INDEX) ? arm->registers[rn] - op2 : sub(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ?  (r < 0) : ((int32_t) r < 0);
    // Unsigned overflow if subtraction produced a borrow
    arm->pstate.C = 0;
    // Signed overflow/underflow if signs of operand are diferent from result
    arm->pstate.V = 0;
    fputs("(subs)", stderr);
    return EXIT_SUCCESS;
}

static uint64_t and(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    fprintf(stderr, "{rd: %lx, rn: %lx, op2: %lx}", arm->registers[rd], arm->registers[rn], op2);
    uint64_t r = arm->registers[rn] & op2;
    arm->registers[rd] = r;
    fputs("(and)", stderr);
    return r;
}

static uint64_t orr(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    arm->registers[rd] = arm->registers[rn] | op2;
    fputs("(orr)", stderr);
    return EXIT_SUCCESS;
}

static uint64_t eor(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    arm->registers[rd] = arm->registers[rn] ^ op2;
    fputs("(eor)", stderr);
    return EXIT_SUCCESS;
}

static uint64_t ands(ARM* arm, int rd, int rn, uint64_t op2, int sf) {
    int64_t r = (rd == ZR_INDEX) ? arm->registers[rn] & op2 : and(arm, rd, rn, op2, sf);

    // Sets flags for PSTATE
    arm->pstate.Z = (r == 0);
    // Check negative as 32 or 64 bit
    arm->pstate.N = sf ? (r < 0) : ((int32_t) r < 0);
    // C and V are set to 0 after logical operations.
    arm->pstate.C = 0;
    arm->pstate.V = 0;
    fputs("(ands)", stderr);
    return EXIT_SUCCESS;
}

static void madd(ARM* arm, int rd, int rn, int ra, int rm, int sf) {
    arm->registers[rd] = arm->registers[ra] + (arm->registers[rn] * arm->registers[rm]);
    fputs("(madd)", stderr);
}

static void msub(ARM* arm, int rd, int rn, int ra, int rm, int sf) {
    arm->registers[rd] = arm->registers[ra] - (arm->registers[rn] * arm->registers[rm]);
    fputs("(msub)", stderr);
}

/*
Function Pointers
*/

// Used for wide moves in immediate processing.
static void (*wideMove[4])(ARM* arm, int rd, uint64_t op, int hw) = {
    &movn, NULL, &movz, &movk
};

// Used for arithemtic in immediate and register processing.
static uint64_t (*arithmetic[4])(ARM* arm, int rd, int rn, uint64_t op2, int sf) = {
    &add, &adds, &sub, &subs
};

// Used for logical operations in register processing.
static uint64_t (*logical[4])(ARM* arm, int rd, int rn, uint64_t op2, int sf) = {
    &and, &orr, &eor, &ands
};

// Used for multiply in register processing.
static void (*mutiply[2])(ARM* arm, int rd, int rn, int ra, int rm, int sf) = {
    &madd, &msub
};

// Used to shift rm in register processing.
static uint64_t (*shiftRm[4])(uint64_t rm, uint32_t imm6, bool sf) = {
    &lsl, &lsr, &asr, &ror
};

/*
Main functions
*/

void dataProcessingImmediate(ARM* arm, int instruction) {
    bool sf = getBitAt(instruction, DPI_SFBIT);
    int opc = getBitsAt(instruction, DPI_OPC_START, DPI_OPC_LEN);
    int opi = getBitsAt(instruction, DPI_OPI_START, DPI_OPI_LEN);
    int rd = getBitsAt(instruction, DPI_RD_START, REG_INDEX_SIZE);
    fprintf(stderr, "%d", opc);

    switch (opi) {

        // Arithmetic
        case DPI_ARITHMETIC_OPI: {
            bool sh = getBitAt(instruction, DPI_SHBIT);
            uint64_t imm12 = getBitsAt(instruction, DPI_IMM12_START, IMM12_LEN);
            int rn = getBitsAt(instruction, DPI_RN_START, REG_INDEX_SIZE);

            // Shift imm12 by 12 if shift bit is given.
            if (sh) {
                imm12 <<= 12;
            }

            // If sf is not given, read rd and rn as 32 bit; rn to be restored later.
            uint64_t rntemp;
            if (!sf) {
                rntemp = arm->registers[rn];
                arm->registers[rd] &= WREGISTER_MASK;
                arm->registers[rn] &= WREGISTER_MASK;
            }

            // Index 32 encodes ZR for arithmetic instructions which change PSTATE.
            // Opc starts with 1 for adds and subs, which change PSTATE flags.
            // Only compute if destination is not ZR or operation changes flags.
            if (rd != ZR_INDEX || getBitAt(opc, 0) == 0b1) {
                arithmetic[opc](arm, rd, rn, imm12, sf);
            }

            // Restore rn if it was read as 32 bit and is not the destination register;
            if (!sf && rd != rn) {
                arm->registers[rn] = rntemp;
            }

            break;
        }

        // Wide Move
        case DPI_WIDEMOVE_OPI: {
            int hw = getBitsAt(instruction, DPI_HW_START, DPI_HW_SIZE);
            uint64_t imm16 = getBitsAt(instruction, DPI_IMM16_START, IMM16_LEN);

            // For movk don't shift imm16.
            if (opc != DPI_MOVK_OPC) {
                imm16 <<= (hw * DPI_SHIFT_VALUE);
            }

            // Read rd as 32 bit register if sf is not given.
            if (!sf) {
                arm->registers[rd] &= WREGISTER_MASK;
            }

            // Last rd index encodes ZR for wide move processing.
            // No need to compute logical instruction when rd = ZR since write
            // is ignored and PSTATE isn't changed.
            if (rd != ZR_INDEX) {
                wideMove[opc](arm, rd, imm16, hw);
            }

            break;
        }

        default:
            fputs("(NOTHING)", stderr);
    }

    // Write to rd as a 32 bit register if sf is not given (fixes overflows).
    if (!sf) {
        fprintf(stderr, "[%d]", rd);
        fprintf(stderr, "%lx ", arm->registers[rd]);
        arm->registers[rd] &= WREGISTER_MASK;
        fprintf(stderr, "%lx ", arm->registers[rd]);
    }
}

// Execute data processing register instructions.
void dataProcessingRegister(ARM* arm, int instruction) {
    bool sf = getBitAt(instruction, DPR_SFBIT_POS);
    int rd = getBitsAt(instruction, DPR_RD_START, REG_INDEX_SIZE);
    int rm = getBitsAt(instruction, DPR_RM_START, REG_INDEX_SIZE);
    int rn = getBitsAt(instruction, DPR_RN_START, REG_INDEX_SIZE);
    bool m = getBitAt(instruction, DPR_MBIT_POS);

    // Multiply; m determines whether its multiply or arithemtic/logical
    if (m) {
        int ra = getBitsAt(instruction, DPR_RA_START, REG_INDEX_SIZE);
        bool x = getBitAt(instruction, DPR_XBIT_POS);

        // If sf is not given, read registers as 32 bit; all but rd to be restored later.
        uint64_t rntemp;
        uint64_t rmtemp;
        uint64_t ratemp;
        if (!sf) {
            rntemp = arm->registers[rn];
            ratemp = arm->registers[ra];
            rmtemp = arm->registers[rm];
            arm->registers[rd] &= WREGISTER_MASK;
            arm->registers[rn] &= WREGISTER_MASK;
            arm->registers[ra] &= WREGISTER_MASK;
            arm->registers[rm] &= WREGISTER_MASK;
        }

        // ZR is read-only and no flags will be changed.
        if (rd != ZR_INDEX) {
            mutiply[x](arm, rd, rn, ra, rm, sf);
        }

        // Restore registers read as 32 bit;
        if (!sf) {
            if (rn != rd) {
                arm->registers[rn] = rntemp;
            }
            if (rm != rd) {
                arm->registers[rm] = rmtemp;
            }
            if (rm != ra) {
                arm->registers[ra] = ratemp;
            }
        }

        
    }

    // Arithemtic and Logical
    else {
        int shift = getBitsAt(instruction, DPR_SHIFT_START, DPR_SHIFT_LEN);
        bool n = getBitAt(instruction, DPR_NBIT_POS);
        int opc = getBitsAt(instruction, DPR_OPC_START, DPR_OPC_LEN);
        uint64_t imm6 = getBitsAt(instruction, DPR_IMM6_START, IMM6_LEN);
        bool isArithmetic = getBitAt(instruction, DPR_ARITHMETICBIT_POS);

        // If sf is not given, read registers as 32 bit; all but rd to be restored later.
        uint64_t rntemp;
        uint64_t rmtemp;
        if (!sf) {
            rntemp = arm->registers[rn];
            rmtemp = arm->registers[rm];
            arm->registers[rd] &= WREGISTER_MASK;
            arm->registers[rn] &= WREGISTER_MASK;
            arm->registers[rm] &= WREGISTER_MASK;
        }

        // Shift rm by imm6 with type depending on shift bits.
        uint64_t op2 = shiftRm[shift](arm->registers[rm], imm6, sf);

        // Negate opearand for logical instructions if n bit is given.
        // (N bit is de facto 0 for arithmetic instruction so no need to check).
        if (n) {
            op2 = ~op2; 
        }

        // Index 32 encodes ZR for arithmetic instructions which change PSTATE.
        // Opc starts with 0b11 for ands and bics, which change PSTATE flags.
        // Only compute if destination is not ZR or operation changes flags
        if (!isArithmetic && (rd != ZR_INDEX || getBitsAt(opc, 0, 2) == 0b11)) {
            logical[opc](arm, rd, rn, op2, sf); 
        } else if (rd != ZR_INDEX || getBitAt(opc, 0) == 0b1) {
            arithmetic[opc](arm, rd, rn, op2, sf);
        }

        // Restore registers read as 32 bit if they are not the destination register;
        if (!sf) {
            if (rn != rd) {
                fprintf(stderr, "[accessed %d]", rn);
                fprintf(stderr, "[regval before: %lx]", arm->registers[rn]);
                arm->registers[rn] = rntemp;
                fprintf(stderr, "[regval after: %lx]", arm->registers[rn]);
            }
            if (rm != rd) {
                arm->registers[rm] = rmtemp;
            }
        }
    }

    // Write to rd as a 32 bit register if sf is not given (fixes overflows).
    if (!sf) {
        arm->registers[rd] &= WREGISTER_MASK;
    }
}

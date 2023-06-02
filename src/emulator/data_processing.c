#include "defs.h"
#include "utils.h"

void movz(ARM* arm, int rd, int op2) {
    arm->memory[rd] = op2;
}

void movn(ARM* arm, int rd, int op2) {
    arm->memory[rd] = ~op2;
}

void movk(ARM* arm, int rd, int op2) {

}

int add(ARM* arm, int rd, int rn, int op2) {
    int r = arm->memory[rn] + op2;
    arm->registers[rd] = r;
    return r;
}

int sub(ARM* arm, int rd, int rn, int op2) {
    int r = arm->memory[rn] - op2;
    arm->registers[rd] = r;
    return r;
}

int adds(ARM* arm, int rd, int rn, int op2) {

}

int subs(ARM* arm, int rd, int rn, int op2) {

}

void (*logicalImmediate[3])(ARM* arm, int rd, int op) = {
    &movz, &movn, &movk
};

int (*arithmeticImmediate[4])(ARM* arm, int rd, int op) = {
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
            // arithmetic
            int sh = getBitAt(instruction, DPI_SHBIT);
            int imm12 = getBitsAt(instruction, DPI_IMM12_START, IMM12_LEN);
            // Since we don't implement stack pointer, rn must be a general register
            int rn = getBitsAt(instruction, DPI_RN_START, REG_INDEX_SIZE);
            if (sh) {
                imm12 <<= 12;
            }
            //arithmeticImmediate[opc](arm, rd, rn, imm12)
            break;
        }

        // Wide Move
        case DPI_WIDEMOVE_OPI: {
            int hw = getBitsAt(instruction, DPI_HW_START, DPI_HW_SIZE);
            int imm16 = getBitsAt(instruction, DPI_IMM16_START, IMM16_LEN);
            imm16 <<= (hw * 16);
            logicalImmediate[opc](arm, rd, imm16);
            break;
        }
    }
}

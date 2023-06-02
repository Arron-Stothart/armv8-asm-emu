#include "defs.h"
#include "utils.h"

// void movz(ARM* arm, int rd, int op) {

// }

// void movn(ARM* arm, int rd, int op) {

// }

// void movk(ARM* arm, int rd, int op) {

// }

// int add(ARM* arm, int rd, int rn, int op) {

// }

// int sub(ARM* arm, int rd, int rn, int op) {

// }

// void adds(ARM* arm, int rd, int rn, int op) {

// }

// void subs(ARM* arm, int rd, int rn, int op) {

// }

// int (*logicalImmediate[3])(ARM* arm, int rd, int op) = {
//     &movz, &movn, &movk
// };

// int (*arithmeticImmediate[4])(ARM* arm, int rd, int op) = {
//     &add, &adds, &sub, &subs
// };

// WIP
void dataProcessingImmediate(ARM* arm, int instruction) {
    int sf = getBitAt(instruction, DPI_SFBIT);
    int opc = getBitsAt(instruction, DPI_OPC_START, DPI_OPC_LEN);
    int opi = getBitsAt(instruction, DPI_OPI_START, DPI_OPI_LEN);
    int rd = getBitsAt(instruction, DPI_RD_START, REG_INDEX_SIZE);

    if (sf) {
        // accessed as 64 bits

    } else {
        // accessed as 32 bits

    }

    switch (opi) {

        // Arithmetic
        case DPI_ARITHMETIC_OPI: {
            // arithmetic
            int sh = getBitAt(instruction, DPI_SHBIT);
            int imm12 = getBitsAt(instruction, DPI_IMM12_START, IMM12_LEN);
            int rn = getBitsAt(instruction, DPI_RN_START, REG_INDEX_SIZE);
            if (sh) {
                imm12 <<= 12;
            }
            //arithmeticImmediate[opc](arm, rd, rn, op)
            break;
        }

        // Wide Move
        case DPI_WIDEMOVE_OPI: {
            int hw = getBitsAt(instruction, DPI_HW_START, DPI_HW_SIZE);
            int imm16 = getBitsAt(instruction, DPI_IMM16_START, IMM16_LEN);
            imm16 <<= (hw * 16);
            // logicalImmediate[opc](arm, rd, op);
            break;
        }
    }
}

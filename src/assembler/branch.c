#include <stdint.h>
#include "utils.h"
#include <stdio.h>

/*
UNCONDITONAL BRANCH
*/

#define BR_UNCOND_BASE 0x14000000
#define BR_SIMM26_START 0

uint32_t b(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    fprintf(stderr, "unconditional branch\n");
    return BR_UNCOND_BASE | ((calculateOffset(arg1, address, SIMM26_LEN) / INSTRUCTION_SIZE) << BR_SIMM26_START);
}

/*
REGISTER BRANCH
*/

#define BR_REG_BASE 0xD61F0000
#define BR_XN_START 5


uint32_t br(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return BR_REG_BASE | (getRegNum(arg1) << BR_XN_START);
}

/*
CONDITONAL BRANCH
*/

#define BR_COND_BASE 0x54000000
#define BR_COND_START 0
#define BR_SIMM19_START 5
#define BR_EQ 0b0000 // Equal
#define BR_NE 0b0001 // Not equal
#define BR_GE 0b1010 // Signed greater or equal
#define BR_LT 0b1011 // Signed less than
#define BR_GT 0b1100 // Signed greater than
#define BR_LE 0b1101 // Signed less than or equal
#define BR_AL 0b1110 // Always

uint32_t bcond(char* arg1, uint32_t cond, uint32_t address) {
    return BR_COND_BASE | ((calculateOffset(arg1, address, SIMM19_LEN) / INSTRUCTION_SIZE) << BR_SIMM19_START) | (cond << BR_COND_START);
}

uint32_t bal(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return bcond(arg1, BR_AL, address);
}

uint32_t bne(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return bcond(arg1, BR_NE, address);
}

uint32_t beq(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return bcond(arg1, BR_EQ, address);
}

uint32_t bgt(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return bcond(arg1, BR_GT, address);
}

uint32_t blt(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return bcond(arg1, BR_LT, address);
}

uint32_t ble(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return bcond(arg1, BR_LE, address);
}

uint32_t bge(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) {
    return bcond(arg1, BR_GE, address);
}

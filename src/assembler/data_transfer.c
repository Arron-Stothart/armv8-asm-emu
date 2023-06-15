#include <stdint.h>

/*
Data Transfer
*/

uint32_t ldr(char** operands) {

}

uint32_t str(char** operands) {

}

/*
Special instructions and .int directive included here
*/

#define SPECIAL_NOP_CODE 0x1f2003d5

uint32_t nop(char** operands) {
    return SPECIAL_NOP_CODE;
}

uint32_t intdir(char** operands) {

}

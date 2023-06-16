#include <stdint.h>

/*
Data Transfer
*/

uint32_t ldr(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t str(char* arg1, char* arg2, char* arg3, char* arg4) {

}

/*
Special instructions and .int directive included here
*/

#define SPECIAL_NOP_CODE 0x1f2003d5

uint32_t nop(char* arg1, char* arg2, char* arg3, char* arg4) {
    return SPECIAL_NOP_CODE;
}

uint32_t intdir(char* arg1, char* arg2, char* arg3, char* arg4) {

}

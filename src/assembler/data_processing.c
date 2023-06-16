#include <stdint.h>
#include "utils.h"
#include "defs.h"

uint32_t add(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t adds(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t sub(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t subs(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t cmp(char* arg1, char* arg2, char* arg3, char* arg4) {
    return subs(getZeroReg(arg1), arg1, arg2, "");
}

uint32_t cmn(char* arg1, char* arg2, char* arg3, char* arg4) {
    return adds(getZeroReg(arg1), arg1, arg2, "");
}

uint32_t neg(char* arg1, char* arg2, char* arg3, char* arg4) {
    return sub(arg1, getZeroReg(arg1), arg2, "");
}

uint32_t negs(char* arg1, char* arg2, char* arg3, char* arg4) {
    return subs(arg1, getZeroReg(arg1), arg2, "");
}

uint32_t and(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t ands(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t bic(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t bics(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t eor(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t orn(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t eon(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t orr(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t tst(char* arg1, char* arg2, char* arg3, char* arg4) {
    return ands(getZeroReg(arg1), arg1, arg2, "");
}

uint32_t movk(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t movn(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t movz(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t mov(char* arg1, char* arg2, char* arg3, char* arg4) {
    return orr(arg1, getZeroReg(arg1), arg2, "");
}

uint32_t mvn(char* arg1, char* arg2, char* arg3, char* arg4) {
    return orr(arg1, getZeroReg(arg1), arg2, "");
}

uint32_t madd(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t msub(char* arg1, char* arg2, char* arg3, char* arg4) {

}

uint32_t mul(char* arg1, char* arg2, char* arg3, char* arg4) {
    return madd(arg1, arg2, arg3, giveZeroReg(arg1));
}

uint32_t mneg(char* arg1, char* arg2, char* arg3, char* arg4) {
    return msub(arg1, arg2, arg3, giveZero(arg1));
}

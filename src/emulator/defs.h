#include <stdbool.h>

// Register Constants
#ifndef ZR

#define REGISTER_SIZE 8 // in bytes
#define NUM_OF_REGISTERS 31
#define ZR 0 // Zero register

// Memory Constants
#define MAX_MEMORY_SIZE (1 << 21)
#define BYTES_IN_WORD 4
#define SIZE_OF_BYTE 8 // in bits
#define BYTES_IN_64BIT 8
#define BYTES_IN_32BIT 4


// Instruction Constants 
// Codes are in big endian
#define INSTRUCTION_SIZE 4
#define HALT_CODE 0x0000008a
#define NOP_CODE 0x1f2003d5

// Enum for Instruction Type
typedef enum {
    DATA_PROCESSING_IMMEDIATE,
    DATA_PROCESSING_REGISTER, 
    SINGLE_DATA_TRANSFER,
    BRANCH,
    HALT,
    NOP,
    DATA
} INSTRUCTION_TYPE;

// PSTATE tracks flags triggered by last result.
// N - Negative value; Z - Zero value; C - Carry; V - Overflow.
typedef struct {
    bool N;
    bool Z;
    bool C;
    bool V;
} PSTATE;

// ARM Proccesor 
// Registers are 64 bit; Memory is byte addressable (char = 1 byte). 
// Leave CIR as char since it is an index in memory.
typedef struct {
    long long int registers[NUM_OF_REGISTERS];
    char memory[MAX_MEMORY_SIZE];
    PSTATE pstate;
    unsigned long long int pc;
} ARM;

// Enum for Branch Instruction Type
typedef enum {
    UNCONDITIONAL,
    REGISTER,
    CONDITIONAL
} BRANCH_TYPE;

#endif

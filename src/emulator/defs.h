#include <stdbool.h>

// Register Constants
#ifndef ZR

#define REGISTER_SIZE 8 // in bytes
#define NUM_OF_REGISTERS 31
#define ZR_INDEX 31 // Register 32 is zero register (starting from R0)
#define ZR 0 // Zero register
#define WREGISTER_MASK 0xff // sets top 32 bits to 0.
#define REG_INDEX_SIZE 5 // number of bits used in instructions

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
#define SIMM9_LEN 9
#define SIMM12_LEN 12
#define IMM12_LEN 12
#define IMM16_LEN 16

// Single Data Processing Constants
#define SDT_LBIT 22
#define SDT_UBIT 24
#define SDT_SFBIT 30
#define SDT_RT_START 0
#define SDT_XN_START 5
#define SDT_IBIT 11
#define SDT_SIMM9_START 12
#define SDT_XM_START 16

// Immediate Data Processing Constants
#define DPI_ARITHMETIC_OPI 0b010
#define DPI_WIDEMOVE_OPI 0b101
#define DPI_MOVK_OPC 0b11

#define DPI_SFBIT 31
#define DPI_OPC_START 29
#define DPI_OPC_LEN 2
#define DPI_OPI_START 23
#define DPI_OPI_LEN 3
#define DPI_RD_START 0
#define DPI_SHBIT 22
#define DPI_IMM12_START 10 // for arithmetic
#define DPI_RN_START 5 // for arithmetic
#define DPI_IMM16_START 5 // for logical
#define DPI_HW_START 21 // for logical
#define DPI_HW_SIZE 2 // for logical

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

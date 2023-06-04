#include <stdbool.h>
#include <stdint.h>

// Register Constants
#ifndef ZR

#define REGISTER_SIZE 8 // in bytes
#define NUM_OF_REGISTERS 31
#define ZR 0 // Zero register
#define WREGISTER_MASK 0x0000000011111111 // sets top 32 bits to 0.
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
#define SIMM26_LEN 26

// Single Data Processing Constants
#define SDT_LBIT_POS 22
#define SDT_UBIT_POS 24
#define SDT_SFBIT_POS 30
#define SDT_RT_START 0
#define SDT_XN_START 5
#define SDT_IBIT_POS 11
#define SDT_SIMM9_START 12
#define SDT_XM_START 16

// Branch Constants
#define BR_EQ 0b0000 // Equal
#define BR_NE 0b0001 // Not equal
#define BR_GE 0b1010 // Signed greater or equal
#define BR_LT 0b1011 // Signed less than
#define BR_GT 0b1100 // Signed greater than
#define BR_LE 0b1101 // Signed less than or equal
#define BR_AL 0b1110 // Always

// Data Processing Constants
#define DPR_MULTIPLY_OPR 0b1000

#define DPR_RD_START 0
#define DPR_RN_START 5
#define DPR_RM_START 16
#define DPR_OPR_START 21
#define DPR_OPR_LEN 3
#define DPR_OPC_START 29
#define DPR_OPC_LEN 2
#define DPR_SFBIT 31
#define DPR_SHIFT_START 22
#define DPR_SHIFT_LEN 2
#define DPR_NBIT_POS 21
#define DPR_RA_START 10 // for multiply
#define DPR_XBIT_POS 15 // for multiply

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
typedef struct {
    uint64_t registers[NUM_OF_REGISTERS];
    char memory[MAX_MEMORY_SIZE];
    PSTATE pstate;
    uint64_t pc;
} ARM;

// Enum for Branch Instruction Type
typedef enum {
    UNCONDITIONAL,
    REGISTER,
    CONDITIONAL
} BRANCH_TYPE;

// Enum for Data Transfer instruction type
typedef enum {
    UNSIGNED_OFFSET,
    PRE_INDEX,
    POST_INDEX,
    REGISTER_OFFSET,
    LITERAL_ADDRESS
} TRANSFER_TYPE;

#endif

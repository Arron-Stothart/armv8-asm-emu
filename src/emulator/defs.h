#include <stdbool.h>
#include <stdint.h>

#ifndef ZR

// Register Constants
#define REGISTER_SIZE 8 // in bytes
#define NUM_OF_REGISTERS 31
#define ZR_INDEX 32 // Register 32 is zero register
#define ZR 0 // Zero register
#define WREGISTER_MASK 0x0000000011111111 // sets top 32 bits to 0.
#define REG_INDEX_SIZE 5 // number of bits used in instructions (32 = 2^5 reg)

// Memory Constants
#define MAX_MEMORY_SIZE (1 << 21)
#define BYTES_IN_WORD 4
#define SIZE_OF_BYTE 8 // in bits
#define BYTES_IN_64BIT 8
#define BYTES_IN_32BIT 4


// Instruction Constants; codes are in big endian
#define INSTRUCTION_SIZE 4
#define HALT_CODE 0x0000008a
#define NOP_CODE 0x1f2003d5
#define SIMM9_LEN 9
#define SIMM12_LEN 12
#define SIMM26_LEN 26

// Single Data Processing Constants
#define SDT_LBIT 22
#define SDT_UBIT 24
#define SDT_SFBIT 30
#define SDT_RT_START 0
#define SDT_XN_START 5
#define SDT_IBIT 11
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
#define BR_SIMM26_START 25
#define BR_SIMM19_START 23
#define BR_XN_START 9
#define BR_COND_START 3
#define BR_COND_LEN 4
#define BR_DET_BITS_START 31
#define BR_DET_BITS_LEN 3
#define BR_DET_BITS_UNCOND 0b000
#define BR_DET_BITS_REG 0b110
#define BR_DET_BITS_COND 0b010

// Data Processing Constants
// (I for immediate, R for register)
#define DPI_ARITHMETIC_OPI 0b010
#define DPI_WIDEMOVE_OPI 0b101

#define DPI_SFBIT 31
#define DPI_OPC_START 29
#define DPI_OPC_LEN 2
#define DPI_OPI_START 23
#define DPI_OPI_LEN 3
#define DPI_RD_START 0
#define DPI_OPERAND_START 5
#define DPI_OPERARND_LEN 18

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

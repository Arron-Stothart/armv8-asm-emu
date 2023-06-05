#include <stdbool.h>
#include <stdint.h>

#ifndef ZR

// Register Constants
#define REGISTER_SIZE 8 // in bytes
#define NUM_OF_REGISTERS 32
#define ZR_INDEX 31 // Register 32 is zero register (starting from R0); read-only
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
#define INSTRUCTION_SIZE 4 // in bytes
#define HALT_CODE 0x0000008a
#define NOP_CODE 0x1f2003d5
#define SIMM9_LEN 9
#define SIMM12_LEN 12
#define IMM12_LEN 12
#define IMM16_LEN 16
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
#define DPR_RD_START 0
#define DPR_RN_START 5
#define DPR_RM_START 16
#define DPR_OPR_START_21
#define DPR_OPR_SIZE 3
#define DPR_OPC_START 29
#define DPR_OPC_SIZE 2
#define DPR_SFBIT 31
#define DPR_SHIFT_START 22
#define DPR_SHIFT_SIZE 2
#define DPR_NBIT 21

// Data Processing Constants
#define DPR_MULTIPLY_OPR 0b1000
#define DPR_LSL_SHIFT 0b00
#define DPR_LSR_SHIFT 0b01
#define DPR_ASR_SHIFT 0b10
#define DPR_ROR_SHIFT 0b11

#define DPR_RD_START 0
#define DPR_RN_START 5
#define DPR_RM_START 16
#define DPR_OPR_START 21
#define DPR_OPR_LEN 3
#define DPR_OPC_START 29
#define DPR_OPC_LEN 2
#define DPR_SFBIT_POS 31
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
    uint64_t registers[NUM_OF_REGISTERS];
    char memory[MAX_MEMORY_SIZE];
    PSTATE pstate;
    uint64_t pc;
} ARM;

#endif

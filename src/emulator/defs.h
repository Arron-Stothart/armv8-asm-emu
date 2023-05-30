// Register Constants
#define REGISTER_SIZE 8 // in bytes
#define NUM_OF_REGISTERS 31
#define ZR 0 // Zero register

// Memory Constants
#define MAX_MEMORY_SIZE (1 << 21)

// Instruction Constants 
// Codes are in big endian
#define INSTRUCTION_SIZE 4 // in bytes
#define HALT_CODE 0x0000008a
#define NOP_CODE 0x1f2003d5


// Enum for Instruction Type
typedef enum {
    DATA_PROCESSING_IMMEDIATE,
    DATA_PROCESSING_REGISTER, 
    SINGLE_DATA_TRANSFER,
    BRANCH,
    NOP,
    HALT,
} INSTRUCTION_TYPE;

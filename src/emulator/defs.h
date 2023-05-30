// Register Constants
#define REGISTER_SIZE 8 // in bytes
#define NUM_OF_REGISTERS 31
#define ZR 0 // Zero register

// Memory Constants
#define MAX_MEMORY_SIZE (1 << 21)

// Instruction Constants
#define INSTRUCTION_SIZE 4 // in bytes

// Enum for Instruction Type
typedef enum {
    DATA_PROCESSING_IMMEDIATE,
    DATA_PROCESSING_REGISTER, 
    SINGLE_DATA_TRANSFER,
    LOAD_LITERAL,
    BRANCH,
    NOP,
    HALT,
} instruction_type;

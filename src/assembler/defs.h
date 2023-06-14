#include <stdint.h>

#define MAX_WORDS_IN_LINE 5
#define MAX_OPERANDS 4
// #define MAX_SYMBOL_TABLE_SIZE 

// Entries for symbol table: associates a label with memory address
typedef struct {
    char* label;
    uint64_t address;
    symbol* next;
 } symbol;

// Linked symbol table
typdef struct {
    symbol* first;
} symbol_table;

// Enum for type of assembly line; possibly not needed
// INSTRUCTION = an operation mnemonic (e.g. add, ldr, . . . ), and one, two, three or four operand fields
// DIRECTIVE = strings that start with a ‘.’; the only one is ‘.int’
// LABEL = strings that begin with an alphabetical character (a–z or A–Z) and end with a ‘:’ 
typedef enum {
    INSTRUCTION, 
    DIRECTIVE,
    LABEL        
} LINE_TYPE;

// Enum for opcode (instruction mnemonic)
typedef enum {
    ADD, 
    ADDS, 
    SUB,
    SUBS,
    CMP,
    CMN,
    NEG,
    NEGS,
    AND,
    ANDS,
    BIC,
    BICS,
    EOR,
    EON,
    ORR,
    ORN,
    TST,
    MVN,
    MOV,
    MOVN,
    MOVK,
    MOVZ,
    MADD,
    MSUB,
    MUL,
    MNEG,
    B,
    BR,
    BCOND,
    LDR,
    STR,
    NOP,
    HLT // i.e. and x0, x0, x0
} OPCODE;

// Enum for operand type
typedef enum {
    register_operand,
    immediate_value_operand,
    label_operand, // i.e. immediate address
} operand_type;

// Structure for operand
typedef struct {
    operand_type type;
    int value;
} operand;

typedef char* operand_arr[MAX_OPERANDS];

// Structure for assembler file instructions: operation mneumonic and up to four operands
typedef struct {
    OPCODE opcode;
    operand_arr operands;
} instruction;
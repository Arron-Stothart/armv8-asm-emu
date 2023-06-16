#include <stdint.h>

// Parser Constants
#define MAX_WORDS_IN_LINE 5
#define MAX_OPERANDS 4
#define MAX_CHARS_IN_LINE 128 // Arbitrary choice
#define MAX_LABELS 128 // Arbitrary Choice
#define MAX_LINES 128 // Arbitrary Choice

// Instruction Constants
#define INSTRUCTION_SIZE 4 // in bytes
#define OPERAND_WZR "wzr"
#define OPERAND_XZR "xzr"

// Structure for symbol table
// Has to be defined this way since symbol is self-referential
 typedef struct symbol symbol;
struct symbol {
    char* label;
    uint64_t address;
    symbol* next; // linked list structure
 };

typedef struct {
    symbol* first;
    uint8_t size; // max size < MAX_LABELS
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

// Structure for operand
typedef enum {
    register_operand,
    immediate_value_operand,
    label_operand, // i.e. immediate address
} operand_type;

typedef struct {
    operand_type type;
    int value;
} operand;

// Structure for assembler file instructions: operation mneumonic and up to four operands
typedef struct {
    char* opcode;
    char** operands;
} instruction;

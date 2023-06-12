#include <stdint.h>

// Entries for symbol table: associates a label with memory address
typedef struct {
    char* label;
    uint64_t address;
 } symbol;

// Enum for type of assembly line; possibly not needed
// INSTRUCTION = an operation mnemonic (e.g. add, ldr, . . . ), and one, two, three or four operand fields
// DIRECTIVE = strings that start with a ‘.’; the only one is ‘.int’
// LABEL = strings that begin with an alphabetical character (a–z or A–Z) and end with a ‘:’ 
typedef enum {
    INSTRUCTION, 
    DIRECTIVE,
    LABEL        
} LINE_TYPE;
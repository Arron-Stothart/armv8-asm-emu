#include <stdint.h>

// Entries for symbol table: associates a label with memory address
typedef struct symbol {
    char* label;
    uint64_t address;
} symbol_t;

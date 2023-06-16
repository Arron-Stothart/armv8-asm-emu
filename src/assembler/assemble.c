#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "defs.h"
#include "utils.h"
#include "symbol_table.h"
#include "tokenize.h"
#include "branch.h"
#include "data_processing.h"
#include "data_transfer.h"

uint32_t (*instructionFunctions[59])(char* arg1, char* arg2, char* arg3, char* arg4) = {
    &eor, &bge, &subs, &orr, &bics, &sub, &adds, // 6
    &bic, &and, &negs, &ble, &b, &bal, &br, // 13
    NULL, &eon, NULL, NULL, &ldr, &movn, &blt, // 20
    &beq, &mul, &movk, &madd, NULL, &msub, NULL, // 27
    NULL, &cmn, &intdir, NULL, &neg, NULL, &mvn, // 34
    &movz, &ands, &str, &mov, &add, NULL, NULL, // 41
    NULL, &bgt, &mneg, NULL, NULL, &cmp, NULL, // 48
    NULL, &tst, NULL, NULL, &nop, NULL, &orn, // 55
    NULL, NULL, &bne
};

int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "assemble: ./assemble <file_in> <file_out>.\n");
        exit(EXIT_FAILURE);
    }

    // Create empty symbol table
    symbol_table* st = newSymbolTable();
    assert(st != NULL);

    // Read file into buffer. Buffer is initalized as terminating characters.
    char* buffer[MAX_LINES] = {NULL};
    int numRead = readFile(buffer, argv[1]);

    // Remove blank lines
    // TODO: move to procedure (preferably using a function pointer to remove duplication)
    char **read_line = buffer;
    char **write_line = buffer;
    while (*read_line != NULL) {
        if (!isBlankLine(read_line) && getLineType(read_line) != LABEL) {
            *write_line = *read_line;
            write_line++;
        }
        read_line++;
    }   

    // First pass: Create symbol table associating labels with memory addresses
    uint8_t numIns = populateSymbolTable(buffer, st, numRead);

    // Remove labels from buffer:
    // TODO: move to procedure
    char **read_line = buffer;
    char **write_line = buffer;
    while (*read_line != NULL) {
        if (getLineType(read_line) != LABEL) {
            *write_line = *read_line;
            write_line++;
        }
        read_line++;
    }

    // Allocate memory for instructions
    uint32_t* instructions = (uint32_t*) calloc(numIns, sizeof(uint32_t));
    assert(instructions != NULL);

    // Second pass: Read in each instruction and .int directive, write instructions into array
    instruction instr;
    for (int i = 0; i < numIns; i++) {
        // Tokenize instruction into opcode and operands.
        instr = tokenizeInstruction(buffer[i]);
        // Hash opcode to get correct function. This function will return the word to be written to memory given the operands.
        instructions[i] = instructionFunctions[hash(instr.opcode)](instr.operands[0], instr.operands[1], instr.operands[2], instr.operands[3]);
    }

    writeBinary(argv[2], instructions, numRead);
    free(st);
    return EXIT_SUCCESS;
}

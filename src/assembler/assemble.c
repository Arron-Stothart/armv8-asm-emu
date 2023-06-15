#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "defs.h"
#include "utils.h"
#include "symbol_table.h"
#include "tokenize.h"
#include "branch.h"
#include "data_processing.h"
#include "data_transfer.h"

uint32_t (*instructionFunctions[59])(char** operands) = {
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
    char* buffer[MAX_LINES] = {"\0"};
    int numRead = readFile(buffer, argv[1]);

    // First pass: Create symbol table associating labels with memory addresses
    uint8_t numIns = populateSymbolTable(buffer, st, numRead);

    // Allocate memory for instructions
    uint32_t* instructions = (uint32_t*) calloc(numIns, sizeof(uint32_t));
    assert(instructions != NULL);

    // Second pass: Read in each instruction and .int directive, write instructions into array
    instruction instr;
    for (int i = 0; i < numIns; i++) {
        // If line is not blank or empty.
        if (strcmp(buffer[i], "\n") != 0 && strcmp(buffer[i], "\r\n") != 0 && strcmp(buffer[i], "\0") != 0) {
            // Tokenize instruction into opcode and operands.
            instr = tokenizeInstruction(buffer[i]);
            // Hash opcode to get correct function. This function will return the word to be written to memory given the operands.
            instructions[i] = instructionFunctions[hash(instr.opcode)](instr.operands);
        } else {
            // If not instruction, then don't increment i.
            // This keeps track of where to store next instruction in array.
            i--;
        }
    }

    writeBinary(argv[2], instructions, numRead);
    free(st);
    return EXIT_SUCCESS;
}

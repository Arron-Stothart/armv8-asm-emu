#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "defs.h"
#include "utils.h"
#include "symbol_table.h"
#include "tokenize.h"
#include "branch.h"
#include "data_processing.h"
#include "data_transfer.h"

symbol_table* st;

uint32_t (*instructionFunctions[59])(char* arg1, char* arg2, char* arg3, char* arg4, uint32_t address) = {
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
    st = newSymbolTable();
    assert(st != NULL);

    // Read file into buffer. Buffer is initalized as NULL pointers.
    char buffer[MAX_LINES][MAX_CHARS_IN_LINE] = {(char)0};
    int numRead = readFile(buffer, argv[1]);


    // Remove blank lines
    // numRead = skipBlanks(buffer, numRead);

    // First pass: Create symbol table associating labels with memory addresses
    uint8_t numIns = populateSymbolTable(buffer, st, numRead);

    // Remove labels from buffer:
    //skipLines(buffer, &isLabel, numRead); 

    // Allocate memory for instructions
    uint32_t* instructions = (uint32_t*) calloc(numIns, sizeof(uint32_t));
    assert(instructions != NULL);

    // Second pass: Read in each instruction and .int directive, write instructions into array
    instruction instr = {.opcode = "", .operands = {""}};
    instruction* instrptr = &instr;
    uint32_t address = 0; 
    fprintf(stderr, "numRead is %d\n", numRead);
    for (int i = 0; i < numRead; i++) {
        fprintf(stderr, "i is %d\n", i);
        fprintf(stderr, "calling from assembler.c line is %s", buffer[i]);
        trimWhitespace(buffer[i]);
        if (!isLabel(buffer[i]) && !isBlankLine(buffer[i])) {
            

            // Tokenize instruction into opcode and operands.
            tokenizeInstruction(buffer[i], instrptr);
            fprintf(stderr, "instr successfully returned\n");
            // fprintf(stderr, "%s\n", buffer[i]);
            fprintf(stderr, "opcode: %s\n", instrptr->opcode);
            for (int j = 0; j < MAX_OPERANDS; j++) { 
                fprintf(stderr, "%s\n", instrptr->operands[j]);
            }

            // Hash opcode to get correct function. This function will return the word to be written to memory given the operands.
            // Divide address by INSTRUCTION_SIZE so we don't add blank lines.
            instructions[address / INSTRUCTION_SIZE] 
                = instructionFunctions[hash(instrptr->opcode)](instrptr->operands[0], instrptr->operands[1], instrptr->operands[2],
                                                               instrptr->operands[3], address);

            // Increment address (blank lines and labels don't need to increment address)
            fprintf(stderr, "address: %d\n", address);
            address += INSTRUCTION_SIZE; 
        } else {
            fprintf(stderr, "blank or label\n");
        }

        fprintf(stderr, "finished, %d\n", i);    
    }

    fprintf(stderr, "write\n");
    fprintf(stderr, "the size of intructions is %d\n", numIns);
    writeBinary(argv[2], instructions, numIns);
    fprintf(stderr, "wrinting finished \n");
    free(st);
    free(instructions);
    fprintf(stderr, "success");
    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "utils.h"
#include "branch.h"
#include "data_processing.h"
#include "data_transfer.h"

int main(int argc, char **argv) {

    // Check if binary file provided.
    if (argc < 2) {
        fprintf(stderr, "emulate: no binary file provided.\n");
        fprintf(stderr, "%i args", argc);
        for (int i = 0; i < argc; i++) {
            fputs(argv[i], stderr);
            fputs(" ", stderr);
        }
        exit(EXIT_FAILURE);
    }

    // Initialise ARM to default state.
    ARM arm = {
        .registers = {0},
        .memory = {0},
        .pstate = (PSTATE) {.N = false, .Z = true, .C = false, .V = false},
        .pc = 0
    };

    // Load instructions into memory.
    loadBinary(arm.memory, argv[1]);

    // Fetch-Decode-Execute Cycle
    for (;;) {
        // Check if address is in memory range.
        if (!(arm.pc >= 0 && arm.pc <= MAX_MEMORY_SIZE)) {
            fprintf(
                stderr,
                "the PC is: %lu which is out of range \n",
                arm.pc);
        }
        assert(arm.pc >= 0 && arm.pc <= MAX_MEMORY_SIZE);
        // Fetch and decode instruction.
        int instruction = getWord(&arm.memory[arm.pc]);
        INSTRUCTION_TYPE type = getInstructionType(instruction);
        // Pre-increment pc to next instruction location.
        // DEBUG: fprintf(stderr, "PC is: %lu \n", arm.pc);
        arm.pc += INSTRUCTION_SIZE;

        const char *names[] = { "DATA_PROCESSING_IMMEDIATE ", "DATA_PROCESSING_REGISTER ", "SINGLE_DATA_TRANSFER ", "BRANCH ", "HALT ", "NOP ", "DATA " };
        fputs(names[type], stderr);

        switch(type) {
            case HALT:
                fputs("HALT \n", stderr);
                goto halt; // Required to break out of both switch and for loop
                break;
            case DATA_PROCESSING_IMMEDIATE:
                fputs("DATA_PROCESSING_IMMEDIATE \n", stderr);
                dataProcessingImmediate(&arm, instruction);
                break;
            case DATA_PROCESSING_REGISTER:
                fputs("DATA_PROCESSING_REGISTER \n", stderr);
                dataProcessingRegister(&arm, instruction);
                break;
            case SINGLE_DATA_TRANSFER:
                fputs("SINGLE_DATA_TRANSFER \n", stderr);
                singleDataTransfer(&arm, instruction);
                break;
            case BRANCH:
                fputs("BRANCH \n", stderr);
                branch(&arm, instruction);
                break;
            default:
                // Non-instruction data or NOP case; ignore.
                fputs("default \n", stderr);
                if (type == HALT) {
                    fputs("AGHG", stderr);
                }
        }
    }

    halt:
        // ensure arm.pc is not too high [off-by-one error]
        arm.pc -= INSTRUCTION_SIZE;
        if (argc >= 3) {
            outputState(&arm, argv[2]);
        } else {
            outputState(&arm, "output.out");
        }
        return EXIT_SUCCESS;
}

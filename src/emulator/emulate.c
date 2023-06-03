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
    if (argc != 2) {
        fprintf(stderr, "emulate: no binary file provided.\n");
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
        assert(arm.pc >= 0 && arm.pc <= MAX_MEMORY_SIZE);
        // Fetch and decode instruction.
        int instruction = getWord(&arm.memory[arm.pc]);
        INSTRUCTION_TYPE type = getInstructionType(instruction);
        arm.pc += INSTRUCTION_SIZE;

        switch(type) {
            case HALT:
                goto halt; // Required to break out of two loops.
                break;
            case DATA_PROCESSING_IMMEDIATE:
                break;
            case DATA_PROCESSING_REGISTER:
                dataProcessingRegister(&arm, instruction);
                break;
            case SINGLE_DATA_TRANSFER:
                break;
            case BRANCH:
                executeBranch(&arm, instruction);
                break;
            default:
                // Non-instruction data or NOP case; ignore.
                break;
        }
    }

    halt:
        outputState(&arm);
        return EXIT_SUCCESS;
}

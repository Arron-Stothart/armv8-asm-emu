#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "utils.h"

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
        .pstate = (PSTATE) {.N = false, .Z = false, .C = false, .V = false},
        .pc = 0,
        .cir = 0
    };

    // Load instructions into memory.
    loadBinary(arm.memory, argv[1]);

    // Fetch-Decode-Execute Cycle
    for (;;) {
        // Check if address is in memory range.
        assert(arm.pc >= 0 && arm.pc <= MAX_MEMORY_SIZE);
        // Fetch and decode instruction.
        arm.cir = arm.memory[arm.pc];
        arm.pc += INSTRUCTION_SIZE;
        INSTRUCTION_TYPE type = getInstructionType(arm.cir);

        switch(type) {
            case HALT:
                goto halt; // Required to break out of two loops. 
                break;
            case NOP:
                break;
            case DATA_PROCESSING_IMMEDIATE:
                break;
            case DATA_PROCESSING_REGISTER:
                break;
            case SINGLE_DATA_TRANSFER:
                break;
            case BRANCH:
                break;
            case DATA:
                // Memory is 1 byte so overshot by 3 bytes.
                arm.pc -= (INSTRUCTION_SIZE - 1);
                break;
        }
    }

    halt:
        outputState(&arm);
        return EXIT_SUCCESS;
}

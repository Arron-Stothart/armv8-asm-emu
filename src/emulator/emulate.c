#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "arm.h"

int main(int argc, char **argv) {

    // Check if binary file provided.
    if (argc != 2) {
    printf("emulate: no binary file provided.\n");
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
    loadbinary(arm.memory, argv[1]);

    // Fetch-Decode-Execute Cycle
    for (;;) {
        // Fetch and decode instruction.
        arm.cir = arm.memory[arm.pc];
        INSTRUCTION_TYPE type = getInstructionType(arm.cir);

        switch(type) {
            case HALT:
                goto halt;
                break;
            case NOP:
                arm.pc++;
                break;
            case DATA_PROCESSING_IMMEDIATE:
                break;
            case DATA_PROCESSING_REGISTER:
                break;
            case SINGLE_DATA_TRANSFER:
                break;
            case BRANCH:
                break;
        }
    }

    halt:
        outputstate(&arm);
        return EXIT_SUCCESS;
}

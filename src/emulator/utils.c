#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

// Converts hexadecimals in 32bits between endians.
int convert(int value) {
    int converted = 0;

    // Bitshifts for bytes.
    converted |= ((0x000000ff & value) << 24);
    converted |= ((0x0000ff00 & value) << 8);
    converted |= ((0x00ff0000 & value) >> 8);
    converted |= ((0xff000000 & value) >> 24);

    return converted;
}

// Returns word from byte addressable memory
int getWord(char* memory) {
    int value = 0;
    for (int i = 0; i < BYTES_IN_WORD; i++) {
        value += *memory >> (8 * i);
        memory++;
    }
    return value;
}

// Gets instruction type given instruction in little-endian.
INSTRUCTION_TYPE getInstructionType(int word) {
    unsigned int instruction = convert(word);
    unsigned int op0 = (instruction >> 25) & 0x1111; // TODO: test this

    if (instruction == HALT_CODE) {
        return HALT;
    } else if (instruction == NOP_CODE) {
        return NOP;
    } else if ((op0 & 0x1110) == 0x1000) {
        return DATA_PROCESSING_IMMEDIATE;
    } else if ((op0 & 0x0111) == 0x0101) {
        return DATA_PROCESSING_REGISTER;
    } else if ((op0 & 0x0101) == 0x0100) {
        return SINGLE_DATA_TRANSFER;
    } else if ((op0 & 0x1110) == 0x1010) {
        return BRANCH;
    } else {
        // Default case; not instruction.
        return DATA;
    }
}

// Outputs state of ARM processor into .out file.
void outputState(ARM* arm) {
    FILE* output = fopen("output.out", "w");

    // Output registers.
    fprintf(output, "Registers: \n");

	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		fprintf(output, "$X%02d = %16llx\n",
			   i, arm->registers[i]);
	}

    fprintf(output, "PC  = %16llx\n", arm->pc);

    // Output PSTATE //! find better way to do this
    fprintf(output, "PSTATE: ");
    fprintf(output, (arm->pstate.N) ? "N" : "-");
    fprintf(output, (arm->pstate.Z) ? "Z" : "-");
    fprintf(output, (arm->pstate.C) ? "C" : "-");
    fprintf(output, (arm->pstate.V) ? "V\n" : "-\n");

    // Output Memory
    fprintf(output, "Non-zero memory:\n");

    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
		if (arm->memory[i] > 0) {
            // Bytes are loaded in little endian so have to convert.
            fprintf(output, "0x%08x: 0x%08x\n", i * 4, convert(getword(&arm->memory[i])));
		}
	}

    fclose(output);
}

// Given a array of memory and a binary file, data from file will be loaded into array.
void loadBinary(char* memory, char* path) {

    FILE* binary = fopen(path, "r");

    // If file pointer is null then return with error.
    if (binary == NULL) {
        printf("Error in opening file.\n");
        exit(EXIT_FAILURE);
    }

    // Read data from binary until end of file. Only 1 piece of data read each time.
    int i = 0, read = 1;
    while (read) {
        read = fread(&memory[i], sizeof(char), 1, binary);
        i++;
    }

    fclose(binary);
}

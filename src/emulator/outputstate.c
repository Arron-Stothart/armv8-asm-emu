#include <stdio.h>
#include <string.h>
#include "arm.h"

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

// Outputs state of ARM processor into .out file.
void outputstate(ARM* arm) {
    FILE* output = fopen("state.out", "w");

    // Output registers.
    fprintf(output, "Registers: \n");

	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		fprintf(output, "$X%02d = %16d\n",
			   i, arm->registers[i]);
	}

    fprintf(output, "PC  = %16d\n", arm->pc);

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
            fprintf(output, "0x%.8x: 0x%.8x\n", i, convert(arm->memory[i]));
		}
	}

    fclose(output);
}

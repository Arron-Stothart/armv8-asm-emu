#include <stdio.h>
#include <string.h>
#include "arm.h"


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
            //TODO fprintf memory addresses
            //! we recieve bytes in little endian so we have to reverse int
            // fprintf(output, "0x%.8x: 0x%.8x\n", i, arm->memory[i]);
		}
	}

    fclose(output);
}

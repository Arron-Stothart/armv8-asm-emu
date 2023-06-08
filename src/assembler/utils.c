#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Writes n instructions from array into binary file
void writebinary(char* path, uint32_t* instructions, int n) {
	// Creating the output file
	FILE* output = fopen(path, "wb");

	// Verifying the output
	if(output == NULL) {
		fprintf(stderr, "Error opening file at %s\n", path);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		fwrite(&(instructions[i]) , sizeof(uint32_t), 1, output);
	}

	fclose(output);
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
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
        value += *memory >> (SIZE_OF_BYTE * i);
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
            fprintf(output, "0x%08x: 0x%08x\n", i * 4, convert(getWord(&arm->memory[i])));
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

/*
Bitwise Operations
*/

// Bits are shifted right; rotated back into the left-hand end if carried right.
static int rotateRight(long long int value, int shift, int bits) {
    assert(shift >= 0);
    return (value >> shift) | (value << (bits - shift));
}

// Rotates lower 32 bits. Sets top 32 bits to 0.
int rotateRight32(long long int value, int shift) {
    int masked = value & WREGISTER_MASK;
    return rotateRight(masked, shift, 32);
}

// Rotates 64 bit integers.
int rotateRight64(long long int value, int shift) {
    return rotateRight(value, shift, 64);
}

// Shift bits right filling vacated bits with sign bit.
int arithmeticShiftRight64(long long int value, int shift)
{
    assert(shift >= 0);
    return value < 0 ? ~(~value >> shift) : value >> shift;
}

// Shifts lower 32 bits right filling vacated bits with sign bit. Sets top 32 bits to 0.
int arithmeticShiftRight32(long long int value, int shift)
{
    int masked = value & WREGISTER_MASK;
    return arithmeticShiftRight64(masked, shift);
}

// generates binary mask of n ones.
static int generateMask(int n) {
    return (int) (pow(2, n) - 1);
}

// Gets l bits upwards starting from kth positon of n.
int getBitsAt(int n, int k, int l) {
    // In kth position we can get up to k + 1 bits
    assert(k >= 0 && l > 0);
    // Can be converted into getting bits downards by using:
    // return (n >> (k - l + 1)) & generateMask(l)
    return (n >> k) & generateMask(l);
}

// Gets bit at kth position from n.
int getBitAt(int n, int k) {
    assert(k >= 0);
    return (n >> k) & 1;
}

// Clears l bits downwards starting from kth position of n to 0.
static int bitClear(int n, int k, int l) {
    assert(k >= 0 && l > 0);
    return n & (~(generateMask(l) << (k - l + 1)));
}

// Sets clearsize bits starting from kth position of n to new to size copysize.
int setBitsTo(int n, int k, int new, int clearsize, int copysize) {
    int cleared = bitClear(n, k, clearsize);
    return cleared | (getBitsAt(new, k, copysize) << k);
}


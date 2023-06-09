#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <inttypes.h>
#include "defs.h"

// Returns word from byte addressable memory
uint32_t getWord(char* memory) {
    uint32_t value = 0;
    for (int i = 0; i < BYTES_IN_WORD; i++) {
        value += *memory << (SIZE_OF_BYTE * i);
        memory++;
    }
    return value;
}

// Returns word from byte addressable memory
uint64_t getDoubleWord(char* memory) {
    uint64_t value = 0;
    for (int i = 0; i < BYTES_IN_DOUBLE_WORD; i++) {
        value += *memory << (SIZE_OF_BYTE * i);
        memory++;
    }
    return value;
}

// Outputs state of ARM processor into .out file.
void outputState(ARM *arm, char *file) {
    FILE* output = fopen(file, "w");

    // Output registers.
    fprintf(output, "Registers: \n");

	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		fprintf(output, "X%02d = %016lx\n",
			   i, arm->registers[i]);
	}

    fprintf(output, "PC = %016lx\n", arm->pc);

    // Output PSTATE //! find better way to do this
    fprintf(output, "PSTATE: ");
    fprintf(output, (arm->pstate.N) ? "N" : "-");
    fprintf(output, (arm->pstate.Z) ? "Z" : "-");
    fprintf(output, (arm->pstate.C) ? "C" : "-");
    fprintf(output, (arm->pstate.V) ? "V\n" : "-\n");

    // Output Memory
    fprintf(output, "Non-zero memory:\n");

    for (int i = 0; i < MAX_MEMORY_SIZE; i += 4) {
		if (getWord(&arm->memory[i]) != 0) {
            // Bytes are stored in little endian so have to convert.
            fprintf(output, "0x%08x: %08x\n", i, getWord(&arm->memory[i]));
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

// Masks top 32 bits to 0 if value is 32 bits.
static uint64_t wregisterMask(uint64_t value, bool is64bit) {
    if (!is64bit) {
        value &= WREGISTER_MASK;
    }
    return value;
}

// Rotate right
uint64_t ror(uint64_t value, uint32_t shift, bool is64bit) {
    assert(shift >= 0);
    int bits = is64bit ? 64 : 32;
    // If 32 bit, mask top 32 bits after shift.
    value = wregisterMask(value, is64bit);
    return (value >> shift) | (value << (bits - shift));
}

// Logical shift left
uint64_t lsl(uint64_t value, uint32_t shift, bool is64bit) {
    assert(shift >= 0);
    value >>= shift;
    // If 32 bit, mask top 32 bits after shift.
    return wregisterMask(value, is64bit);
}

uint64_t lsr(uint64_t value, uint32_t shift, bool is64bit) {
    assert(shift >= 0);
    value <<= shift;
    // If 32 bit, mask top 32 bits after shift.
    return wregisterMask(value, is64bit);
}

// Arithemtic shift right
uint64_t asr(uint64_t value, uint32_t shift, bool is64bit) {
    assert(shift >= 0);

    // Cast value to signed 32 bit integer if in 32 bits.
    if (!is64bit) {
        value = (int32_t) value;
    } else {
        value = (int64_t) value;
    }

    value = value < 0 ? ~(~value >> shift) : value >> shift;
    // If 32 bit, mask top 32 bits after shift.
    return wregisterMask(value, is64bit);
}

// generates binary mask of n ones.
static uint32_t generateMask(uint32_t n) {
    return ((1 << n) - 1);
}

// Gets l bits upwards starting from kth positon of n.
int getBitsAt(uint64_t n, int k, int l) {
    // In kth position we can get up to k + 1 bits
    assert(k >= 0 && l > 0);
    // Can be converted into getting bits downards by using:
    // return (n >> (k - l + 1)) & generateMask(l)
    return (n >> k) & generateMask(l);
}

// Gets bit at kth position from n.
int getBitAt(uint64_t n, int k) {
    assert(k >= 0);
    return (n >> k) & 1;
}

// Clears l bits downwards starting from kth position of n to 0.
static int bitClear(uint64_t n, int k, int l) {
    assert(k >= 0 && l > 0);
    return n & (~(generateMask(l) << (k - l + 1)));
}

// Sets clearsize bits starting from kth position of n to new to size copysize.
int setBitsTo(uint64_t n, int k, int new, int clearsize, int copysize) {
    int cleared = bitClear(n, k, clearsize);
    return cleared | (getBitsAt(new, k, copysize) << k);
}

// Gets instruction type given instruction.
INSTRUCTION_TYPE getInstructionType(uint32_t instruction) {
    uint32_t op0 = getBitsAt(instruction, OP0_START, OP0_LEN);

    if (instruction == HALT_CODE) {
        return HALT;
    } else if (instruction == NOP_CODE) {
        return NOP;
    } else if ((op0 & 0b1110) == 0b1000) {
        return DATA_PROCESSING_IMMEDIATE;
    } else if ((op0 & 0b0111) == 0b0101) {
        return DATA_PROCESSING_REGISTER;
    } else if ((op0 & 0b0101) == 0b0100) {
        return SINGLE_DATA_TRANSFER;
    } else if ((op0 & 0b1110) == 0b1010) {
        return BRANCH;
    } else {
        // Default case; not instruction.
        return DATA;
    }
}
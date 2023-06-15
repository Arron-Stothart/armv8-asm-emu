#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "defs.h"
#include "utils.h"
#include "symbol_table.h"
#include "tokenize.c"

uint8_t (*instructions[])(char* operands[MAX_OPERANDS]) = {
  // Add all functions
};

int main(int argc, char **argv) {

  if (argc < 3) {
        fprintf(stderr, "assemble: ./assemble <file_in> <file_out>.\n");
        exit(EXIT_FAILURE);
  }

  // Create empty symbol table
  symbol_table* st = newSymbolTable();
  assert(st != NULL);

  // Read file into buffer. Buffer is initalized as terminating characters.
  char* buffer[MAX_LINES] = {"\0"};
  int numRead = readFile(buffer, argv[1]);

  // First pass: Create symbol table associating labels with memory addresses
  uint8_t numIns = populateSymbolTable(buffer, st, numRead);
  // Allocate memory for instructions
  uint32_t* instructions = (uint32_t*) calloc(numIns, sizeof(uint32_t));

  // Second pass: Read in each instruction and .int directive, write instructions into array
  // TODO: implement
  // instruction instr;
  // for (int i = 0; i < numRead; i++) {
  //   if () {
  //     instr = tokenizeinstruction(buffer[i]);


  //     instructions[i] = instructions[hashedOpc](instr.operands);
  //   }
  // }

  writeBinary(argv[2], instructions, numRead);
  free(st);
  return EXIT_SUCCESS;
}

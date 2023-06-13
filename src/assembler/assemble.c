#include <stdlib.h>
#include <stdio.h>
#include "defs.h"
#include "utils.h"

int main(int argc, char **argv) {

  if (argc < 3) {
        fprintf(stderr, "emulate: no assembler file provided.\n");
        exit(EXIT_FAILURE);
  }

  FILE *input = fopen(argv[1], "r");

  // First pass: Create symbol table associating labels with memory addresses
  symbol sym_table[MAX_SYMBOL_TABLE_SIZE];
  // TODO: implement

  // Second pass: Read in each instruction and .int directive, generate corresponding binary encoding
  // TODO: implement

  return EXIT_SUCCESS;
}

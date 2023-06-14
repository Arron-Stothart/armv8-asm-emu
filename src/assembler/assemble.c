#include <stdlib.h>
#include <stdio.h>
#include "defs.h"
#include "utils.h"

int main(int argc, char **argv) {

  if (argc < 3) {
      fprintf(stderr, "emulate: no assembler file provided.\n");
      exit(EXIT_FAILURE);
  }

  // Read assembly file and pass each line to parser
  FILE* input = fopen(argv[1], "r");
	// If file pointer is null then return with error
	if (input == NULL) {
      printf("Error in opening file.\n");
      exit(EXIT_FAILURE);
  }

  // First pass: Create symbol table associating labels with memory addresses

  // Create empty symbol table
  symbol_table* st = malloc(sizeof(symbol_table));

  // Iterate over each line of file
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), input)) {
      if (getlinetype(line) == LABEL) {
            uint64_t address;
            // TODO: Determine corresponding address
            addsymbol(address, line, st);
      }
  }

  // Second pass: Read in each instruction and .int directive, generate corresponding binary encoding
  // TODO: implement

  fclose(input);

  return EXIT_SUCCESS;
}

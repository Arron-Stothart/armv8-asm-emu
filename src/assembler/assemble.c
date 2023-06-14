#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "defs.h"
#include "utils.h"
#include "symbol_table.h"

int main(int argc, char **argv) {

  if (argc < 3) {
        fprintf(stderr, "assemble: ./assemble <file_in> <file_out>.\n");
        exit(EXIT_FAILURE);
  }

  // Create empty symbol table
  symbol_table* st = newSymbolTable();
  assert(st != NULL);
  // Read file into buffer
  char* buffer[MAX_LINES] = {"\0"};
  int numRead = readFile(buffer, argv[1]);

  // First pass: Create symbol table associating labels with memory addresses



  // Second pass: Read in each instruction and .int directive, generate corresponding binary encoding
  // TODO: implement

  return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

  if (argc < 3) {
        fprintf(stderr, "emulate: no assembler file provided.\n");
        exit(EXIT_FAILURE);
    }

  FILE *input = fopen(argv[1], "r");
  return EXIT_SUCCESS;
}

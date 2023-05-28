#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "arm.h"

int main(int argc, char **argv) {

  // Check if binary file provided.
  if (argc != 2) {
    printf("emulate: no binary file provided.\n");
    exit(EXIT_FAILURE);
  }

  // Initialise ARM to default state.
  ARM arm = {
    .registers = {0},
    .memory = {0},
    .pstate = (PSTATE) {.N = false, .Z = false, .C = false, .V = false},
    .pc = 0,
    .cir = 0
  };

  // Load instructions into memory.
  loadbinary(arm.memory, argv[1]);

  // Fetch-Decode-Execute Cycle
  do {
    // Fetch instruction.
    arm.cir = arm.memory[arm.pc];
    arm.pc++;

    //TODO decode and execute instruction

  } while (arm.cir == true); //TODO add stopping condition here

  outputstate(&arm);
  return EXIT_SUCCESS;
}

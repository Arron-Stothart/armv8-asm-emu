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

    // convert between endians to make life easier
    uint instruction = convert(arm.cir);  // TODO: import this (why on earth is convert in outstate.c??)
    uint op0 = (instruction >> 25) & 0x1111;  // TODO: test this

    if ((op0 & 0x1110) == 0x1000) {
      // data processing immediate
    } if ((op0 & 0x0111) == 0x0101) {
      // data processing register
    } if ((op0 & 0x0101) == 0x0100) {
      // loads and stores
    } if ((op0 & 0x1110) == 0x1010) {
      // branches
    }

  } while (arm.cir != 0x8a000000);  // TODO: test this

  outputstate(&arm);
  return EXIT_SUCCESS;
}

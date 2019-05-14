#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

char cpu_ram_read(struct cpu *cpu, unsigned int index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned int index, unsigned char data) {
  cpu->ram[index] = data;
} 

void cpu_load(struct cpu *cpu, char *file)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded

  char line[128];
  FILE *fp = fopen(file, "r");

  if(fp == NULL) {
    fprintf(stderr, "comp: error opening file \"%s\"\n", file);
    exit(2);
  }

  int address = 0;
  while (fgets(line, sizeof line, fp) != NULL){
    char *endptr;

    unsigned char val = strtoul(line, &endptr, 2);

    if (endptr == line){
      continue;
    }

    cpu->ram[address++] = val;
    }

    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char number_of_operations;
  int running = 1; // True until we get a HLT instruction
  unsigned char operandA;
  unsigned char operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char command = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    number_of_operations = (command >> 6) + 1;  // AND mask &0b11
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->pc +1);
    operandB = cpu_ram_read(cpu, cpu->pc +2);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.

    
    
    

    switch (command) {
      case HLT:
        running = 0;
        break;

      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->pc += number_of_operations;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        cpu->pc += number_of_operations;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->pc += number_of_operations;
        break;
    }

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->ram, 0, 256 * sizeof(cpu->ram[0]));
  memset(cpu->registers, 0, 8 * sizeof(cpu->registers[0]));
}

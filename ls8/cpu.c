#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6
#define E    0b00000001
#define L    0b00000100
#define G    0b00000010

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
    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;

    case ALU_CMP:
      if (cpu->registers[regA] == cpu->registers[regB]) {
        cpu->FL = E;
      } else if (cpu->registers[regA] > cpu->registers[regB]) {
        cpu->FL = G;
      } else {
        cpu->FL = L;
      }
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

  // unsigned char reg_num, val;
  // unsigned int reg_ind = 7;

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
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case PUSH:
        cpu->registers[7]--;
        cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
        break;

      case POP:
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case CALL:
        cpu->registers[7]--;
        cpu_ram_write(cpu, cpu->registers[7], cpu->pc + number_of_operations);
        cpu->pc = cpu->registers[operandA] - number_of_operations;
        break;

      case RET:
        cpu->pc = cpu_ram_read(cpu, cpu->registers[7]) - number_of_operations;
        cpu->registers[7]++;
        break;

      case JMP:
        cpu->pc = cpu->registers[operandA] - number_of_operations;
        break;
      
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;

      case JEQ:
        if (cpu->FL == E) {
          cpu->pc = cpu->registers[operandA] - number_of_operations;
        }
        break;
      
      case JNE:
        if (cpu->FL != E) {
          cpu->pc = cpu->registers[operandA] - number_of_operations;
        }
        break;
        
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
    }
    // make the increment here because some calls don't need to have it done automatically
    cpu->pc += number_of_operations;

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
  cpu->FL = 0;
}

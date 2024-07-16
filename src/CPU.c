#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU_MEM 4096

typedef enum {
    HLT = 0x00,
    ADD = 0x01,
    SUB = 0x02,
    MUL = 0x03,
    DIV = 0x04,
    AND = 0x05,
    OR  = 0x06,
    NOT = 0x07,
    XOR = 0x08,
    JMP = 0x10,
    JE =  0x11,
    JNE = 0x12,
    JG =  0x13,
    JNG = 0x14,
    MOV = 0x15,
    LOAD =0x16,
    STORE=0x17,
} Commands;

typedef struct {
	uint16_t PC;
	uint16_t registers[0x5];
	uint8_t memory[CPU_MEM];
    bool halt;
} CPU;

void debug_mem(CPU *cpu) {
	FILE *debug_file = fopen("mem.txt", "w");
	fwrite(cpu->memory, CPU_MEM, 1, debug_file);
}

void next(CPU *cpu) { cpu->PC++; }

uint16_t *fetch(CPU *cpu) {
	uint16_t *command = (uint16_t*)malloc(4 * sizeof(uint16_t));
    for(int i = 0; i < 4; i++) {
        command[i] = cpu->memory[(cpu->PC * 4) + i];
    }
    return command;
}

void decode_execute(CPU *cpu, uint16_t *instruction) {
  
    
    uint8_t regA = instruction[1] >> 4;
    uint8_t regB = instruction[1] & 0xF;

    uint16_t value = instruction[2] | instruction[3];
    switch(instruction[0]) {
        case HLT: {
            cpu->halt = true;
            printf("[x] HLT\n");
        } break;

        case ADD: {
            // check for overflow?
            cpu->registers[regA] += (regB == 0)? value : cpu->registers[regB];
            printf("[x] ADD\n");
        } break;

        case SUB: {
            // check for underflow?
            cpu->registers[regA] -= (regB == 0)? value : cpu->registers[regB];
            printf("[x] SUB\n");
        } break;

        case MUL: {

            cpu->registers[regA] *= (regB == 0)? value : cpu->registers[regB];
            printf("[x] MUL\n");
        } break;

        case DIV: {

            if(regB == 0) {
                if(value == 0) {
                    printf("[*] ZERO DIVISION EXCEPTION\n");
                    break;
                }
                cpu->registers[regA] /= value;
            } else {
                if(cpu->registers[regB] == 0) {
                    printf("[*] ZERO DIVISION EXCEPTION\n");
                    break;
                }
                cpu->registers[regA] /= cpu->registers[regB];
            }

            printf("[x] DIV\n");
        } break;

        case XOR: {
                   
            cpu->registers[regA] ^= (regB == 0)? value : cpu->registers[regB];
            printf("[x] XOR\n");
        } break;
       
        case JE: {
            if(cpu->registers[regA] != value || cpu->registers[regA] == cpu->registers[regB]) {
                next(cpu);
            }
            printf("[x] JE\n");
        } break;

        case JMP: {
            // Dangerously
            cpu->PC = value;
            printf("[x] JMP\n");
        } break;
        case MOV: {
            cpu->registers[regA] = (regB == 0)? value : cpu->registers[regB];
            printf("[x] MOV\n");
        } break;

        default: {

           printf("TODO\n");
        } break;
        
    }
}

int main(int argc, char *argv[]) {
	printf("Wasix CPU Machine\n");


	CPU cpu = {.PC = 0, .halt = false};

	memset(cpu.memory, 0, CPU_MEM);

	char *program_file = argv[1];
	FILE *program = fopen(program_file, "rb");

	if (program == NULL) {
		fprintf(stderr, "Something was wrong on opening the file\n");
		exit(EXIT_FAILURE);
	}

    // load_program
	fread(cpu.memory, 1, CPU_MEM, program);

    while(!cpu.halt) {
        uint16_t *instruction = fetch(&cpu);
	    decode_execute(&cpu, instruction);
        next(&cpu);
    }

	fclose(program);
	exit(EXIT_SUCCESS);
}
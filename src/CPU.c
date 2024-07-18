#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU_MEM 4096
#define STACK_SIZE 256
#define TOTAL_REGISTERS 0x6

typedef enum {
	HLT = 0x00,
	ADD = 0x01,
	SUB = 0x02,
	MUL = 0x03,
	DIV = 0x04,
	AND = 0x05,
	OR = 0x06,
	NOT = 0x07,
	XOR = 0x08,
	SHR = 0x09,
	SHL = 0xa,
	JMP = 0x10,
	JE = 0x11,
	JNE = 0x12,
	JG = 0x13,
	JNG = 0x14,
	MOV = 0x15,
	LOAD = 0x16,
	STORE = 0x17,
	PUSH = 0x18,
	POP = 0x19,
	PUSHR = 0x1a,
	POPR  = 0x1b,
	CALL  = 0x1c,
	RET   = 0x1d,
} Commands;

typedef struct {
	uint16_t PC;
	uint8_t SP;
	uint8_t BSP;
	uint16_t registers[TOTAL_REGISTERS];
	uint16_t memory[CPU_MEM];
	bool halt;
} CPU;

void debug_mem(CPU *cpu) {
	FILE *debug_file = fopen("mem.txt", "w");
	fwrite(cpu->memory, CPU_MEM, 1, debug_file);
}

void next(CPU *cpu) { cpu->PC++; }

void push(CPU *cpu, uint16_t value) {
	cpu->SP++;
	cpu->memory[cpu->BSP + cpu->SP] = value; 
}

uint16_t pop(CPU *cpu) {
	uint16_t returned = cpu->memory[cpu->BSP + cpu->SP];
	cpu->SP-= cpu->SP == cpu->BSP? 0 : 1;

	return returned;
}

uint16_t *fetch(CPU *cpu) {
	uint16_t *command = (uint16_t *)malloc(4 * sizeof(uint16_t));

	command[0] = cpu->memory[(cpu->PC * 2)] & 0xFF;
	command[1] = cpu->memory[(cpu->PC * 2)] >> 8;
	command[2] = cpu->memory[(cpu->PC * 2) + 1] & 0xFF;
	command[3] = cpu->memory[(cpu->PC * 2) + 1] >> 8;

	return command;
}

void decode_execute(CPU *cpu, uint16_t *instruction) {

	uint8_t regA = instruction[1] >> 4;
	uint8_t regB = instruction[1] & 0xF;

	uint16_t holder = instruction[2] << 8;
	uint16_t value = holder | instruction[3];

	switch (instruction[0]) {
	case HLT: {
		cpu->halt = true;
		printf("[x] HLT\n");
	} break;

	case ADD: {
		// check for overflow?
		cpu->registers[regA] += (regB == 0) ? value : cpu->registers[regB];
		printf("[x] ADD\n");
	} break;

	case SUB: {
		// check for underflow?
		cpu->registers[regA] -= (regB == 0) ? value : cpu->registers[regB];
		printf("[x] SUB\n");
	} break;

	case MUL: {
		cpu->registers[regA] *= (regB == 0) ? value : cpu->registers[regB];
		printf("[x] MUL\n");
	} break;

	case DIV: {

		if (regB == 0) {
			if (value == 0) {
				printf("[*] ZERO DIVISION EXCEPTION\n");
				exit(EXIT_FAILURE);
			}
			cpu->registers[regA] /= value;
		} else {
			if (cpu->registers[regB] == 0) {
				printf("[*] ZERO DIVISION EXCEPTION\n");
				exit(EXIT_FAILURE);
			}
			cpu->registers[regA] /= cpu->registers[regB];
		}

		printf("[x] DIV\n");
	} break;

	case AND: {
		cpu->registers[regA] &= (regB == 0) ? value : cpu->registers[regB];
		printf("[x] AND\n");

	} break;

	case OR: {
		cpu->registers[regA] |= (regB == 0) ? value : cpu->registers[regB];
		printf("[x] OR\n");
	} break;

	case NOT: {
		if (regA == 0) {
			printf("[*] NOT INSTRUCTION SHOULD HAVE A VALUE\n");
			exit(EXIT_FAILURE);
		}
		cpu->registers[regA] = ~cpu->registers[regA];
		printf("[x] NOT\n");

	} break;

	case XOR: {

		cpu->registers[regA] ^= (regB == 0) ? value : cpu->registers[regB];
		printf("[x] XOR\n");
	} break;

	case SHR: {
		// TODO: Check for errors
		cpu->registers[0x5] = cpu->registers[regA] & 1;
		cpu->registers[regA] >>= 1;
		printf("[x] SHR\n");
	} break;

	case SHL: {
		// TODO: Check for errors
		cpu->registers[0x5] = cpu->registers[regA] & 1;
		cpu->registers[regA] <<= 1;
		printf("[x] SHL\n");
	} break;
	case JMP: {
		// Dangerously
		cpu->PC = value - 1;
		printf("[x] JMP\n");
	} break;

	case JE: {
		if (cpu->registers[regA] != value || cpu->registers[regA] == cpu->registers[regB]) {
			next(cpu);
		}
		printf("[x] JE\n");
	} break;

	case JNE: {
		if (cpu->registers[regA] == value || cpu->registers[regA] != cpu->registers[regB])
			next(cpu);

		printf("[x] JNE\n");
	} break;

	case JG: {
		if (regB != 0) {
			if (cpu->registers[regA] > cpu->registers[regB])
				next(cpu);
		} else {
			if (cpu->registers[regA] < value)
				next(cpu);
		}
		printf("[x] JG\n");
	} break;

	case JNG: {
		if (regB != 0) {
			if (cpu->registers[regA] < cpu->registers[regB])
				next(cpu);
		} else {
			if (cpu->registers[regA] > value)
				next(cpu);
		}
		printf("[x] JNG\n");
	} break;
	case MOV: {
		uint8_t key = instruction[2] >> 4;
		uint16_t lower_nibble = (instruction[2] & 0xF) << 8;
		if (key == 0xa) {
			// Get the value on the memory and put it on the regA
			uint16_t memory_index = lower_nibble | instruction[3];
			uint16_t memory_value = cpu->memory[memory_index];
			cpu->registers[regA] = memory_value;
		} else if (key == 0xb) {
			// Put the memory index on the regA
			cpu->registers[regA] = lower_nibble | instruction[3];
		} else {
			// Put the value of regB or the raw value on the regA
			cpu->registers[regA] = (regB == 0) ? value : cpu->registers[regB];
		}
		printf("[x] MOV\n");
	} break;

	case LOAD: {
		// Always load the value on the memory
		if (regB == 0) {
			cpu->registers[regA] = cpu->memory[value];
		} else {
			cpu->registers[regA] = cpu->memory[cpu->registers[regB]];
		}
		printf("[x] LOAD\n");
	} break;

	case STORE: {
		// TODO: Check if you're storing inside the program size
		// store the value on regB or value on the memory_index on regA
		if (regB == 0) {
			cpu->memory[cpu->registers[regA]] = value;
		} else {
			cpu->memory[cpu->registers[regA]] = cpu->registers[regB];
		}
		printf("[x] STORE\n");
	} break;

	case PUSH: {
		if(regA == 0 && value == 0) {
			printf("[*] PUSH COMMAND NEED A VALUE\n");
			exit(EXIT_FAILURE);
		}
		
		push(cpu, regA == 0? value : cpu->registers[regA]);	
		printf("[x] PUSH\n");
	} break;

	case POP: {
		if(regA == 0) { 
			printf("[*] POP COMMAND NEED A REGISTER\n"); 
			exit(EXIT_FAILURE);
		}
		cpu->registers[regA] = pop(cpu);
		printf("[x] POP\n");
	} break;

	case PUSHR: {
		for(int i = 0x1; i < TOTAL_REGISTERS; i++) {
			push(cpu, cpu->registers[i]);
		}
		printf("[x] PUSHR\n");

	} break;
	case POPR: {
		for(int i = TOTAL_REGISTERS - 1; i >= 0x1; i--) {
			cpu->registers[i] = pop(cpu);
		}
		printf("[x] POP\n");

	} break;

	case CALL: {
		if(value == 0) {
			printf("[*] CALL COMMAND NEED A LOCATION");
			exit(EXIT_FAILURE);
		}
		push(cpu, cpu->PC);
		cpu->PC = value;
		printf("[x] CALL\n");
	} break;

	case RET: {
		cpu->PC = pop(cpu);
		printf("[x] RET\n");
	} break;



	default: {

		printf("TODO\n");
	} break;
	}
}

int main(int argc, char *argv[]) {
	printf("Wasix CPU Machine\n");
	bool debug = false;

	if (argv[2] != NULL && strcmp(argv[2], "1") == 0) {
		debug = true;
	}

	CPU cpu = {.PC = 0, .SP = 0, .halt = false};

	memset(cpu.memory, 0, CPU_MEM);

	char *program_file = argv[1];
	FILE *program = fopen(program_file, "rb");

	if (program == NULL) {
		fprintf(stderr, "Something was wrong on opening the file\n");
		exit(EXIT_FAILURE);
	}

	// load_program
	size_t bytes = fread(cpu.memory, 1, CPU_MEM, program);
	
	// TODO: this isn't the best way to do this
	cpu.BSP = bytes;
	cpu.SP = cpu.BSP;


	while (!cpu.halt) {
		uint16_t *instruction = fetch(&cpu);
		decode_execute(&cpu, instruction);
		next(&cpu);
	}

	if (debug) {
		printf("Read %ld elements\n", bytes);
		debug_mem(&cpu);
		printf("[%d %d %d %d %d]\n", cpu.registers[1], cpu.registers[2], cpu.registers[3], cpu.registers[4], cpu.registers[5]);
	}

	fclose(program);
	exit(EXIT_SUCCESS);
}
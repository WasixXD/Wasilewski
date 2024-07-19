#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU_MEM 4096

#define STACK_SIZE 256
#define STACK_START 0x0800 // 2048
#define STACK_END STACK_START + STACK_SIZE


#define BK_SIZE 512
#define BK_START STACK_END
#define BK_END BK_START + BK_SIZE

#define ROM_SIZE 1024
#define ROM_START BK_END
#define ROM_END ROM_START + ROM_SIZE

#define TOTAL_REGISTERS 0x6

#define A 0x1
#define B 0x2
#define C 0x3
#define D 0x4
#define E 0x5

static bool debug = false;
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
	POPR = 0x1b,
	CALL = 0x1c,
	RET = 0x1d,
	SYS = 0x80,
} Commands;

typedef struct {
	// Program Counter
	uint16_t PC;
	// Stack Pointer
	uint8_t SP;
	// Base Stack Pointer
	uint16_t BSP;
	// Free Rom Byte -> next free byte on rom
	uint16_t FRB;
	uint16_t registers[TOTAL_REGISTERS];
	uint16_t memory[CPU_MEM];
	bool halt;
} CPU;

void debug_mem(CPU *cpu) {
	FILE *debug_file = fopen("mem.txt", "w");
	fwrite(cpu->memory, CPU_MEM, 1, debug_file);
}

uint16_t bk_set_addr(CPU *cpu, uint16_t addr) {
	// Set in BK position the address that will be writen in ROM
	cpu->memory[BK_START + addr] = cpu->FRB;
	return cpu->memory[BK_START + addr];
}

uint16_t bk_get_addr(CPU *cpu, uint16_t addr) {
	// Get in BK the address of the start of that v
	return cpu->memory[BK_START + addr];
}

uint16_t bk_get_free(CPU *cpu) {
	for(int i = BK_START; i < BK_END; i++) {
		if(bk_get_addr(cpu, i) == 0) {
			return i;
		}
	}
	return BK_START;
}

void next(CPU *cpu) { cpu->PC++; }

void push(CPU *cpu, uint16_t value) {
	cpu->SP++;
	cpu->memory[cpu->BSP + cpu->SP] = value;
}

uint16_t pop(CPU *cpu) {
	uint16_t returned = cpu->memory[cpu->BSP + cpu->SP];
	cpu->SP -= cpu->SP == cpu->BSP ? 0 : 1;
	return returned;
}

void increase_frb(CPU *cpu, int size) {
	cpu->FRB += size + 1;
}

uint16_t write_str(CPU *cpu, char *c, int size) {
	uint16_t next_free_pointer = bk_get_free(cpu);
	uint16_t str_pointer = bk_set_addr(cpu, next_free_pointer);

	for(int i = 0; c[i] != '\0'; i++) {
		cpu->memory[str_pointer + i] = (uint16_t)c[i];
	}
	increase_frb(cpu, size);

	return str_pointer;
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

	char *msg = "";
	switch (instruction[0]) {
	case HLT: {
		cpu->halt = true;
		msg = "[x] HLT";
	} break;

	case ADD: {
		// check for overflow?
		cpu->registers[regA] += (regB == 0) ? value : cpu->registers[regB];
		msg = "[x] ADD";
	} break;

	case SUB: {
		// check for underflow?
		cpu->registers[regA] -= (regB == 0) ? value : cpu->registers[regB];
		msg = "[x] SUB";
	} break;

	case MUL: {
		cpu->registers[regA] *= (regB == 0) ? value : cpu->registers[regB];
		msg = "[x] MUL";
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

		msg = "[x] DIV";
	} break;

	case AND: {
		cpu->registers[regA] &= (regB == 0) ? value : cpu->registers[regB];
		msg = "[x] AND";

	} break;

	case OR: {
		cpu->registers[regA] |= (regB == 0) ? value : cpu->registers[regB];
		msg = "[x] OR";
	} break;

	case NOT: {
		if (regA == 0) {
			printf("[*] NOT INSTRUCTION SHOULD HAVE A VALUE\n");
			exit(EXIT_FAILURE);
		}
		cpu->registers[regA] = ~cpu->registers[regA];
		msg = "[x] NOT";

	} break;

	case XOR: {

		cpu->registers[regA] ^= (regB == 0) ? value : cpu->registers[regB];
		msg = "[x] XOR";
	} break;

	case SHR: {
		// TODO: Check for errors
		cpu->registers[0x5] = cpu->registers[regA] & 1;
		cpu->registers[regA] >>= 1;
		msg = "[x] SHR";
	} break;

	case SHL: {
		// TODO: Check for errors
		cpu->registers[0x5] = cpu->registers[regA] & 1;
		cpu->registers[regA] <<= 1;
		msg = "[x] SHL";
	} break;
	case JMP: {
		// Dangerously
		cpu->PC = value - 1;
		msg = "[x] JMP";
	} break;

	case JE: {
		if (cpu->registers[regA] != value || cpu->registers[regA] == cpu->registers[regB]) {
			next(cpu);
		}
		msg = "[x] JE";
	} break;

	case JNE: {
		if (cpu->registers[regA] == value || cpu->registers[regA] != cpu->registers[regB])
			next(cpu);

		msg = "[x] JNE";
	} break;

	case JG: {
		if (regB != 0) {
			if (cpu->registers[regA] > cpu->registers[regB])
				next(cpu);
		} else {
			if (cpu->registers[regA] < value)
				next(cpu);
		}
		msg = "[x] JG";
	} break;

	case JNG: {
		if (regB != 0) {
			if (cpu->registers[regA] < cpu->registers[regB])
				next(cpu);
		} else {
			if (cpu->registers[regA] > value)
				next(cpu);
		}
		msg = "[x] JNG";
	} break;
	case MOV: {
		uint8_t key = instruction[2] >> 4;
		uint16_t lower_nibble = (instruction[2] & 0xF) << 8;
		if (key == 0xa) {
			// Get the value on the memory and put it on the regA
			uint16_t memory_index = lower_nibble | instruction[3];
			uint16_t memory_value = cpu->memory[bk_get_addr(cpu, memory_index)];
			cpu->registers[regA] = memory_value;
		} else if (key == 0xb) {
			// Put the memory index on the regA
			cpu->registers[regA] = bk_get_addr(cpu, lower_nibble | instruction[3]);
		} else {
			// Put the value of regB or the raw value on the regA
			cpu->registers[regA] = (regB == 0) ? value : cpu->registers[regB];
		}
		msg = "[x] MOV";
	} break;

	case LOAD: {
		// Always load the value on the memory
		if (regB == 0) {
			uint16_t address = bk_get_addr(cpu, value);
			cpu->registers[regA] = cpu->memory[address];
		} else {
			uint16_t address = bk_get_addr(cpu, value);
			cpu->registers[regA] = cpu->memory[address];
		}
		msg = "[x] LOAD";
	} break;

	case STORE: {
		// store the value on regB or value on the memory_index on regA
		if (regB == 0) {
			uint16_t address = bk_set_addr(cpu, value);
			cpu->memory[address] = value;
		} else {
			uint16_t address = bk_set_addr(cpu, cpu->registers[regB]);
			cpu->memory[address] = cpu->registers[regB];
		}
		msg = "[x] STORE";
	} break;

	case PUSH: {
		if (regA == 0 && value == 0) {
			printf("[*] PUSH COMMAND NEED A VALUE\n");
			exit(EXIT_FAILURE);
		}

		push(cpu, regA == 0 ? value : cpu->registers[regA]);
		msg = "[x] PUSH";
	} break;

	case POP: {
		if (regA == 0) {
			printf("[*] POP COMMAND NEED A REGISTER\n");
			exit(EXIT_FAILURE);
		}
		cpu->registers[regA] = pop(cpu);
		msg = "[x] POP";
	} break;

	case PUSHR: {
		for (int i = 0x1; i < TOTAL_REGISTERS; i++) {
			push(cpu, cpu->registers[i]);
		}
		msg = "[x] PUSHR";

	} break;
	case POPR: {
		for (int i = TOTAL_REGISTERS - 1; i >= 0x1; i--) {
			cpu->registers[i] = pop(cpu);
		}
		msg = "[x] POPR";

	} break;

	case CALL: {
		if (value == 0) {
			printf("[*] CALL COMMAND NEED A LOCATION");
			exit(EXIT_FAILURE);
		}
		push(cpu, cpu->PC);
		cpu->PC = value;
		msg = "[x] CALL";
	} break;

	case RET: {
		cpu->PC = pop(cpu);
		msg = "[x] RET";
	} break;

	case SYS: {
		if (cpu->registers[B] == 0) {
			if(cpu->registers[C] == 1) {
				char c[cpu->registers[D]];
				// fgets always put a \n on the end of the string, we want to let the user choose
				if(fgets(c, sizeof c, stdin) != NULL) {
					size_t len = strlen(c);
					if(len > 0 && c[len - 1] == '\n') {
						c[len - 1] = '\0';
					}
				}	
				cpu->registers[A] = (uint16_t)c[0];

				if(cpu->registers[D] > 1) {
					cpu->registers[A] = write_str(cpu, c, strlen(c));
				}
			} else {
				scanf("%hu", &cpu->registers[A]);
			}
		} else if (cpu->registers[B] == 1) {
			if(cpu->registers[C] == 0) {
				printf("%d", cpu->registers[A]);

			} else {
				for(int i = 0; i < cpu->registers[D]; i++) {
					printf("%c", cpu->memory[cpu->registers[A] + i]);
				}
			}
		}
		msg = "[x] SYS";
	} break;

	default: {

		printf("TODO\n");
	} break;
	}
	if (debug)
		printf("%s\n", msg);
}

int main(int argc, char *argv[]) {

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

	cpu.BSP = STACK_START;
	cpu.SP = cpu.BSP;
	cpu.FRB = ROM_START;

	while (!cpu.halt) {
		uint16_t *instruction = fetch(&cpu);
		decode_execute(&cpu, instruction);
		next(&cpu);
	}

	if(debug) 
		debug_mem(&cpu);

	fclose(program);
	exit(EXIT_SUCCESS);
}
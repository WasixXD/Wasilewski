#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU_MEM 4096

typedef struct {
	uint16_t PC;
	uint16_t registers[0x5];
	uint8_t memory[CPU_MEM];
} CPU;

void debug_mem(CPU *cpu) {
	FILE *debug_file = fopen("debug.txt", "w");
	fwrite(cpu->memory, CPU_MEM, 1, debug_file);
}

void next(CPU *cpu) { cpu->PC++; }

void decode(CPU *cpu) {
	/*char *op_code = cpu->memory[cpu->PC];*/
	int n = sizeof(op_code) * 8;
	printf("%s\n", op_code);

	/*for (int i = 1; i <= n; i++) {*/
	/*	printf("%d", (op_code >> (n - i)) & 1);*/
	/*}*/
}

int main(int argc, char *argv[]) {
	printf("Wasix CPU Machine\n");

	CPU cpu = {.PC = 5};

	memset(cpu.memory, 0, CPU_MEM);

	char *program_file = argv[1];
	FILE *program = fopen(program_file, "rb");

	if (program == NULL) {
		fprintf(stderr, "Something was wrong on opening the file");
		exit(EXIT_FAILURE);
	}

	fgets(cpu.memory, CPU_MEM, program);
	debug_mem(&cpu);
	decode(&cpu);

	fclose(program);
	exit(EXIT_SUCCESS);
}

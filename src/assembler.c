#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { OP_CODE, COMMA, S_BRACKETS, DOLLAR, COMMENTARY, NUMBER, MEMORY_LOCATION, REGISTER, UNKNOWN, PROCEDURE, PROC_NAME } TOKENS_TYPE;

typedef enum {
	A_REGISTER = 0b0001,
	B_REGISTER = 0b0010,
	C_REGISTER = 0b0011,
	D_REGISTER = 0b0100,
	E_REGISTER = 0b0101,
	HLT = 0b00000000,
	ADD = 0b00000001,
	SUB = 0b00000010,
	MUL = 0b00000011,
	DIV = 0b00000100,
	AND = 0b00000101,
	OR = 0b00000110,
	NOT = 0b00000111,
	XOR = 0b00001000,
	SHR = 0b00001001,
	SHL = 0b00001010,
	JMP = 0b00010000,
	JE = 0b00010001,
	JNE = 0b00010010,
	JG = 0b00010011,
	JNG = 0b00010100,
	MOV = 0b00010101,
	LOAD = 0b00010110,
	STORE = 0b00010111,
	PUSH = 0b00011000,
	POP = 0b00011001,
	PUSHR = 0b00011010,
	POPR = 0b00011011,
	CALL = 0b00011100,
	RET = 0b00011101,
	SYS = 0b10000000

} OP_BINARY;

typedef struct {
	char *value;
	TOKENS_TYPE token;
	OP_BINARY bin;
} Token;

typedef struct Line {
	int line_index;
	Token tokens[16];
	int tokens_count;
	bool is_proc;
	struct Line *next_line;
} Line;

unsigned long hash(const char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

OP_BINARY get_bin(char *str) {
	switch (hash(str)) {
	case 177670:
		return A_REGISTER;
	case 177671:
		return B_REGISTER;
	case 177672:
		return C_REGISTER;
	case 177673:
		return D_REGISTER;
	case 177674:
		return E_REGISTER;
	case 193457997:
		return HLT;
	case 193450094:
		return ADD;
	case 193470255:
		return SUB;
	case 193463731:
		return MUL;
	case 193453544:
		return DIV;
	case 193450424:
		return AND;
	case 5862598:
		return OR;
	case 193464630:
		return NOT;
	case 193475518:
		return XOR;
	case 193469842:
		return SHR;
	case 193469836:
		return SHL;
	case 193460204:
		return JMP;
	case 5862420:
		return JE;
	case 193460226:
		return JNE;
	case 5862422:
		return JG;
	case 193460228:
		return JNG;
	case 193463543:
		return MOV;
	case 6384260357:
		return LOAD;
	case 210689088690:
		return STORE;
	case 6384411237:
		return PUSH;
	case 193466804:
		return POP;
	case 210685570903:
		return PUSHR;
	case 6384404614:
		return POPR;
	case 6383922049:
		return CALL;
	case 193468656:
		return RET;
	case 193470404:
		return SYS;
	default: {
		return -1;
	} break;
	}
}

Line *prepare_tokens(char *line) {
	static int line_count = 0;
	Line *items = (Line *)malloc(sizeof(Line));
	items->next_line = NULL;
	items->line_index = line_count;
	items->tokens_count = 0;
	items->is_proc = false;

	int c = 0;

	while (line[c] != '\0') {
		while (isspace(line[c])) {
			c++;
		}
		if (line[c] == '\0' || line[c] == ':')
			break;

		Token new_token;
		// throw this on a switch
		if (line[c] == ',') {
			new_token.token = COMMA;
			new_token.value = &line[c];
			c++;
		} else if (line[c] == '[' || line[c] == ']') {
			new_token.token = S_BRACKETS;
			new_token.value = &line[c];
			c++;

		} else if (line[c] == '$') {
			new_token.token = DOLLAR;
			new_token.value = &line[c];
			c++;
		} else if (line[c] == ';') {
			new_token.token = COMMENTARY;
			new_token.value = &line[c];
			break;

		} else if (isalpha(line[c])) {
			int length = 0;
			new_token.value = (char *)malloc(5 * sizeof(char));
			while (isalnum(line[c])) {
				new_token.value[length++] = line[c++];
			}

			new_token.value[length] = '\0';
			new_token.bin = get_bin(new_token.value);

			if (new_token.bin == -1 && line[c] == ':') {
				new_token.bin = 1;
				new_token.token = PROCEDURE;

				items->tokens[items->tokens_count++] = new_token;
				break;
			}

			if (items->is_proc) {
				new_token.bin = 1;
				new_token.token = PROC_NAME;
				items->is_proc = false;

				items->tokens[items->tokens_count++] = new_token;
				break;
			}

			if (new_token.bin == CALL) {
				items->is_proc = true;
			}

			if (length == 1) {
				new_token.token = REGISTER;
			} else {
				new_token.token = OP_CODE;
			}
		} else if (isdigit(line[c])) {
			int length = 0;
			new_token.value = (char *)malloc(UINT16_MAX * sizeof(char));
			while (isdigit(line[c])) {
				new_token.value[length++] = line[c++];
			}
			new_token.token = NUMBER;
		} else if (!items->is_proc) {
			printf("[*] ERROR on line %d, unexpected token: %s\n", line_count, &line[c]);
			exit(EXIT_FAILURE);
		}

		items->tokens[items->tokens_count++] = new_token;
	}
	line_count++;

	return items;
}

int get_procedure_line(Line *head, const char *str) {
	while (head != NULL) {
		if (head->tokens[0].token == PROCEDURE && strcmp(head->tokens[0].value, str) == 0) {
			return head->line_index;
		}
		head = head->next_line;
	}
	return -1;
}

void compile(Line *head, char *output_file) {
	Line *helper = head;
	FILE *stream = fopen(output_file, "wb");
	while (helper != NULL) {
		uint8_t op_code = 0;
		uint8_t regA = 0;
		uint8_t regB = 0;
		uint16_t value = 0;

		for (int i = 0; i < helper->tokens_count; i++) {
			Token current_token = helper->tokens[i];
			if (current_token.bin == -1) {
				printf("[*] ERROR: Not such command as %s in line %d\n", current_token.value, helper->line_index);
				exit(EXIT_FAILURE);
			}
			if (current_token.token == PROC_NAME) {
				if ((value |= get_procedure_line(helper, current_token.value)) == (uint16_t)-1) {
					printf("[*] ERROR: Could not find the procedure %s\n", current_token.value);
					exit(EXIT_FAILURE);
				}
			}

			if (current_token.token == OP_CODE) {
				op_code = current_token.bin;
			} else if (current_token.token == REGISTER) {
				if (regA == 0) {
					regA = current_token.bin;
				} else {
					regB = current_token.bin;
				}
			} else if (current_token.token == NUMBER) {
				value |= atoi(current_token.value);
			} else if (current_token.token == DOLLAR) {
				value |= 0b1011 << 12;
			}
		}
		uint32_t instruction = htonl(((uint32_t)op_code) << 24 | ((uint32_t)regA) << 20 | ((uint32_t)regB) << 16 | (uint32_t)value);
		fwrite(&instruction, sizeof(instruction), 1, stream);
		printf("\nLINE: %d\n", helper->line_index);
		printf("instruction: 0x%08X\n", instruction);
		printf("op_code: %d\n", op_code);
		printf("regA: %d\n", regA);
		printf("regB: %d\n", regB);
		printf("value: %d\n", value);

		helper = helper->next_line;
	}
}

int main(int argc, char *argv[]) {

	if (argv[1] == NULL) {
		fprintf(stderr, "Please specify a .was file\n");
		exit(EXIT_FAILURE);
	}

	char *output_file = "./a.bin";
	if (argv[2] != NULL) {
		output_file = argv[2];
	}

	FILE *asm_file = fopen(argv[1], "r");
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	if (asm_file == NULL) {
		fprintf(stderr, "Something was wrong on opening the file\n");
		exit(EXIT_FAILURE);
	}

	Line *program = NULL;
	Line *head = NULL;

	while ((read = getline(&line, &len, asm_file)) != -1) {
		if (program == NULL) {

			program = prepare_tokens(line);
			head = program;
		} else {
			program->next_line = prepare_tokens(line);
			program = program->next_line;
		}
	}

	compile(head, output_file);

	fclose(asm_file);
	exit(EXIT_SUCCESS);
}

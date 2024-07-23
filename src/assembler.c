#include <arpa/inet.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { OP_CODE, COMMA, S_BRACKETS, DOLLAR, COMMENTARY, NUMBER, MEMORY_LOCATION, REGISTER, UNKNOWN, PROCEDURE } TOKENS_TYPE;

typedef enum {
	A_REGISTER = 0b0001,
	B_REGISTER = 0b0010,
	C_REGISTER = 0b0011,
	D_REGISTER = 0b0100,
	E_REGISTER = 0b0101,
	HLT = 0b00000000,
	ADD = 0b00000001,
	MOV = 0b00010101,

} OP_BINARY;

typedef struct {
	char *value;
	TOKENS_TYPE token;
} Token;

typedef struct Line {
	int line_index;
	Token tokens[16];
	int tokens_count;
	struct Line *next_line;
} Line;

Line *prepare_tokens(char *line) {
	static int line_count = 0;
	Line *items = (Line *)malloc(sizeof(Line));
	items->next_line = NULL;
	items->line_index = line_count;
	items->tokens_count = 0;

	int c = 0;

	while (line[c] != '\0') {
		while (isspace(line[c])) {
			c++;
		}
		if (line[c] == '\0')
			break;

		Token new_token;
		if (line[c] == ',') {
			new_token.token = COMMA;
			new_token.value = &line[c];
			c++;
		} else if (isalpha(line[c])) {
			int length = 0;
			new_token.value = (char *)malloc(5 * sizeof(char));
			while (isalnum(line[c])) {
				new_token.value[length++] = line[c++];
			}
			new_token.value[length] = '\0';
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
		} else {
			new_token.token = UNKNOWN;
			new_token.value = "\0";
			c++;
		}

		items->tokens[items->tokens_count++] = new_token;
	}
	line_count++;

	return items;
}

void compile(Line *head, char *output_file) {
	Line *helper = head;
	FILE *stream = fopen(output_file, "wb");
	while (helper != NULL) {
		uint8_t op_code = 0;
		uint8_t regS = 0;
		uint16_t value = 0;

		for (int i = 0; i < helper->tokens_count; i++) {
			Token current_token = helper->tokens[i];
			// TODO: REFACTO THIS
			if (current_token.token == OP_CODE) {
				if (strcmp(current_token.value, "MOV") == 0) {
					op_code = MOV;
				} else if (strcmp(current_token.value, "ADD") == 0) {
					op_code = ADD;
				} else if (strcmp(current_token.value, "HLT") == 0) {
					op_code = HLT;
				}
			} else if (current_token.token == REGISTER) {
				if (strcmp(current_token.value, "a") == 0) {
					regS = A_REGISTER << 4;
				}
			} else if (current_token.token == NUMBER) {
				value = atoi(current_token.value);
			}
		}
		uint32_t instruction = htonl(((uint32_t)op_code) << 24 | ((uint32_t)regS) << 16 | (uint32_t)value);
		fwrite(&instruction, sizeof(instruction), 1, stream);
		printf("instruction: 0x%08X\n", instruction);
		printf("op_code: %d\n", op_code);
		printf("regS: %d\n", regS);
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

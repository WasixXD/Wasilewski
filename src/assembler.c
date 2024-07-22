#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { OP_CODE, COMMA, S_BRACKETS, DOLLAR, COMMENTARY, NUMBER, MEMORY_LOCATION, REGISTER, UNKNOWN } TOKENS_TYPE;

typedef struct {
	char *value;
	TOKENS_TYPE token;
} Token;

typedef struct {
	int line_index;
	Token tokens[16];
	int tokens_count;
} Line;

Line prepare_tokens(char *line) {
	static int line_count = 0;
	Line items = {.line_index = line_count, .tokens_count = 0};
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

		items.tokens[items.tokens_count++] = new_token;
	}
	line_count++;

	return items;
}

void parse_line(char *line) {
	Line tokens = prepare_tokens(line);
	printf("--------");
	printf("\nLINE_COUNT: %d\n", tokens.line_index);
	printf("NUMBER_OF_TOKENS: %d\n", tokens.tokens_count);
	for (int i = 0; i < tokens.tokens_count; i++) {
		if (tokens.tokens[i].token == OP_CODE) {
			printf("OP_CODE: %s\n", tokens.tokens[i].value);
		} else if (tokens.tokens[i].token == REGISTER) {
			printf("REGISTER: %s\n", tokens.tokens[i].value);
		} else if (tokens.tokens[i].token == COMMA) {
			printf("COMMA\n");
		} else if (tokens.tokens[i].token == NUMBER) {
			printf("NUMBER: %s\n", tokens.tokens[i].value);
		}
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

	while ((read = getline(&line, &len, asm_file)) != -1) {
		parse_line(line);
	}

	fclose(asm_file);
	exit(EXIT_SUCCESS);
}

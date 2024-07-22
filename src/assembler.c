#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    char *op;
    uint16_t binary; 
} Op_code;

typedef struct {
    Op_code op;
    int line_n;
    Op_code *arg_a;
    Op_code *arg_b;
} Line;

typedef enum {
    OP_CODE,
    COMMA,
    S_BRACKETS,
    DOLLAR,
    COMMENTARY
} TOKENS;

int main(int argc, char *argv[]) {
    if(argv[1] == NULL) {
		fprintf(stderr, "Please specify a .was file\n");
        exit(EXIT_FAILURE);
    }


    char *output_file = "./a.bin";
    if(argv[2] != NULL) {
        output_file = argv[2];
    }

    FILE *asm_file = fopen(argv[1], "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if(asm_file == NULL) {
		fprintf(stderr, "Something was wrong on opening the file\n");
        exit(EXIT_FAILURE);
    }

    while((read = getline(&line, &len, asm_file)) != -1) {
        printf("LINE: %s", line);
    }

    fclose(asm_file);
    exit(EXIT_SUCCESS);
}
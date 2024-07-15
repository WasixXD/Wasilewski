#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint16_t PC;   
} CPU;

int main(int argc, char *argv[]) {
    printf("Wasix CPU Machine\n");

    unsigned char buffer[10];
    char *program_file = argv[1];
    FILE *program = fopen(program_file, "r");

    if(program == NULL) {
        fprintf(stderr, "Something was wrong on opening the file");
    }

}
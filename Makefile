$CC = gcc
$CFLAGS = -Wextra -Wall

all: asm cpu
.PHONY: all asm cpu

asm: comp
cpu: exec

comp: 
	$(CC) -o asm.o src/assembler.c $(CFLAGS)

exec:
	$(CC) -o machine.o src/CPU.c $(CFLAGS)


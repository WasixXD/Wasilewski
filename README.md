
TODO: 
    [x] - Fazer a stack
    [x] - Chamada de Procedures
    [x] - Interrupts
    [x] - Bookkeeper => o Bookkeeper será uma parte do processador que gerencia partes de memoria, então quando alguém tenta dar load em algo, ele pede o endereço daquele valor no bookkeeper e então pega dentro da ROM
    [ ] - Assembler



Fetch -> Decode -> Execute \
program counter = 0; \
registers [
    A, B, C, D, E, F
] \
stack = u8 [ 128 ] 

2⁸ = 256 instruções \
2⁴ = 16 registers de 32 bits \
2¹⁶= 65536 valores possiveis \   

instrução| regA | regB |
0000 0000  0000   0000   0000 0000 0000 0000

HLT  0000 0000 0000 0000 0000 0000 0000 0000

; Arithmethic 
ADD  0000 0001| 0001 0010 0000 0000 0000 0000 => adicionar  | reg1 | reg2 => adicionar o valor do regB no regA
SUB  0000 0010| 0010 0000 0000 0000 0000 0101 => subtrair   | reg1 | 5    => subtrair o valor do regB em 5
MUL  0000 0011| 0001 0010 0000 0000 0000 0000 => multiplicar| reg1 | reg2 => multiplicar o valor do regB no regA
DIV  0000 0100| 0000 0000 0000 0000 0000 0010 => dividir    | reg1 | 2    => dividir o valor de regA em 2

; Logic
AND 0000 0101|
OR  0000 0110|
NOT 0000 0111|
XOR 0000 1000|
SHR 0000 1001
SHL 0000 1010

;Jumps
JMP ; Jump                     0001 0000 
JE  ; Jump if Equal            0001 0001
JNE ; Jump if not Equal        0001 0010
JG  ; Jump if greater than     0001 0011
JNG ; Jump if not greater than 0001 0100

;Memory Op
MOV   0001 0101
LOAD  0001 0110
STORE 0001 0111

MOV a, [0x150]
0001 0101 0001 0000 1010 0001 0101 0000

MOV a, $150
0001 0101 0001 0000 1011 0001 0101 0000

5        = valor real
[0x150]  = valor na mémoria
$150     = valor de mémoria


PUSH 0001 1000; PUSH value or register
POP  0001 1001; POP stack pointer on regA
PUSHR 0001 1010; PUSH all registers
POPR 0001 1011; POP all registers


CALL 0001 1100; CALL a procedure (The CPU only knows how to jump, put the work of discover where the procedure is located on the assembler)

RET 0001 1101; Return to the execution

SYS 1000 0000

{
    a = scanned value or print value
    b = [0 read] [1 write]
    c = [0 int] [1 string]
    d = sizeof reading
}

MOV b, 0 
MOV c, 1
MOV d, 2
SYS
MOV b, 1 
SYS
HLT

0001 0101 0010 0000 0000 0000 0000 0000
0001 0101 0011 0000 0000 0000 0000 0000
1000 0000 0000 0000 0000 0000 0000 0000
0001 0101 0010 0000 0000 0000 0000 0001
1000 0000 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
___
Exemplo em assembly
```asm
0 MOV a, 2
1 CALL double; assembler -> CALL [0x5]
2 ADD a, 1
3 HLT
4 
5 double: 
6 MUL a, 2
7 RET
8 HLT
```
binário
```bin
0001 0101 0001 0000 0000 0000 0000 0010
0001 1100 0000 0000 0000 0000 0000 0101
0000 0001 0001 0000 0000 0000 0000 0001
0000 0000 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0001
0000 0011 0001 0000 0000 0000 0000 0010
0001 1101 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
```
___
Exemplo em Assembly

```asm
MOV a, 5 ;a = 5
ADD a, 2 ;a = 7
HLT
``` 
em binário
```bin
0001 0101 0001 0000 0000 0000 0000 0101
0000 0001 0001 0000 0000 0000 0010 0010
0000 0000 0000 0000 0000 0000 0000 0000
```
___

Fibonnaci
```asm
MOV a, 0 ;a = 0
MOV b, 1 ;b = 1
ADD c, a ;c += a
ADD c, b ;c += b 
MOV a, b ;a = b
MOV b, c ;b = c
JE  c, 34
HLT
XOR c, c ;c = 0
JMP [2]
```
em binário
```
0001 0101 0001 0000 0000 0000 0000 0000
0001 0101 0010 0000 0000 0000 0000 0001
0000 0001 0011 0001 0000 0000 0000 0000
0000 0001 0011 0010 0000 0000 0000 0000
0001 0101 0001 0010 0000 0000 0000 0000
0001 0101 0010 0011 0000 0000 0000 0000
0001 0001 0011 0000 0000 0000 0010 0010
0000 0000 0000 0000 0000 0000 0000 0000
0000 1000 0011 0011 0000 0000 0000 0000
0001 0000 0000 0000 0000 0000 0000 0001
```
___
Contagem de números 1 no número 17

```asm
MOV a, 17 ;0 a = 17
ADD b, 1  ;1 b++
SHR a     ;2 a = a >> 1  
JE  e, 1   ;3 if e == 1
ADD c, 1  ;4 c++
JE  b, 8   ;5 if b == 8
HLT       ;6 htl
JMP [1]   ;7 goto 1
```
em binário
```
0001 0101 0001 0000 0000 0000 0001 0001
0000 0001 0010 0000 0000 0000 0000 0001
0000 1001 0001 0000 0000 0000 0000 0000
0001 0001 0101 0000 0000 0000 0000 0001
0000 0001 0011 0000 0000 0000 0000 0001
0001 0001 0010 0000 0000 0000 0000 1000
0000 0000 0000 0000 0000 0000 0000 0000
0001 0000 0000 0000 0000 0000 0000 0001
```

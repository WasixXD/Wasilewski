
Fetch -> Decode -> Execute
program counter = 0;
registers [
    A, B, C, D, E, F
]
stack = u8 [ 128 ]
ram   = u8 [ 512 ]




2⁸ = 256 instruções
2⁴ = 16 registers de 32 bits
2¹⁶= 65536 valores possiveis  

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
JE c, 34
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
0001 0001 
```asm
MOV a, 17
ADD b, 1
SHR a
JE a, 0 
HLT 
ADD c, e 
JMP [1]
```
em binário
```
0001 0101 0001 0000 0000 0000 0001 0001
0000 0001 0010 0000 0000 0000 0000 0001
0000 1001 0001 0000 0000 0000 0000 0000 
0001 0001 0001 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
0000 0001 0011 0101 0000 0000 0000 0000
0001 0000 0000 0000 0000 0000 0000 0010
```

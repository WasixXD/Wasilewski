# Wasilewski 
## A Custom 16 bits CPU and Assembler
![image](https://github.com/user-attachments/assets/92a52f88-a4fa-4e3c-9ea1-ec39b9b6ce4d)
**Maybe I can solve enigma**

# Brief 📖
This is both a custom CPU and Assembler that reads/compile into binary files and is capable of solving some hard problems

<br/>

# Challenges 🐢
- Come up with the instructions
- Memory Addressing
- Lexical analysis
- Debugging bits
<br>



# Goals 🏆
[ x ] CPU can read binary files<br>
[ x ] CPU can translate binary into instructions <br>
[ x ] CPU has some form of control of it own memory<br>
[ x ] CPU can perform procedures <br>
[ x ] CPU accept 26 custom instructions <br>
[ x ] Assembler can read .was file<br>
[ x ] Assembler can translate Assembly language into Machine code<br>
<br>


# How it works? 💼
## CPU
The CPU performs three basic steps: Fetch -> Decode -> Execute. First, we load our program into main memory, with a maximum size of 2 KB. Next, we set the PC (Program Counter) to zero and begin fetching bytes from that position. The next step is to read the instruction:

    instruc   - regA - regB - value 
    0000 0000 - 0000 - 0000 - 0000 0000 0000 0000

After that, we decode the instruction to determine what the CPU is supposed to do and then execute the corresponding operation. 

# The Stack
The Wasilewski features a 256-byte stack that supports basic stack operations. It's recommended for storing register values using the PUSHR instruction.

# The BookKeeper
The BookKeeper (BK) is a 512-byte address holder responsible for all memory operations, as it is the only component that interfaces with the ROM. It is useful for storing initial variable values.

# The ROM
The CPU includes a 1024-byte Read-Only Memory (ROM) segment. This memory is used solely for storing variables or values set by the LOAD and STORE commands.


| Registers | Binary       |
|-----------|--------------|
| A_REGISTER| 0001         |
| B_REGISTER| 0010         |
| C_REGISTER| 0011         |
| D_REGISTER| 0100         |
| E_REGISTER| 0101         |

| Opcode    | Binary       | Opcode    | Binary       |
|-----------|--------------|-----------|--------------|
| HLT       | 00000000     | MOV       | 00010101     |
| ADD       | 00000001     | LOAD      | 00010110     |
| SUB       | 00000010     | STORE     | 00010111     |
| MUL       | 00000011     | PUSH      | 00011000     |
| DIV       | 00000100     | POP       | 00011001     |
| AND       | 00000101     | PUSHR     | 00011010     |
| OR        | 00000110     | POPR      | 00011011     |
| NOT       | 00000111     |  CALL      | 00011100     |
| XOR       | 00001000     | RET       | 00011101     |
| SHR       | 00001001     | SYS       | 10000000     |
| SHL       | 00001010     |       |           |              |
| JMP       | 00010000     |       |           |              |
| JE        | 00010001     |       |           |              |
| JNE       | 00010010     |       |           |              |
| JG        | 00010011     |       |           |              |
| JNG       | 00010100     |       |           |              |



SYS 1000 0000

{
    a = scanned value or print value
    b = [0 read] [1 write]
    c = [0 int] [1 string]
    d = sizeof reading
}

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

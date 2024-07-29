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
The CPU performs three basic steps: Fetch -> Decode -> Execute. First, we load our program into main memory, with a maximum size of 10 KB. Next, we set the PC (Program Counter) to zero and begin fetching bytes from that position. The next step is to read the instruction:

    instruc   - regA - regB - value 
    0000 0000 - 0000 - 0000 - 0000 0000 0000 0000

After that, we decode the instruction to determine what the CPU is supposed to do and then execute the corresponding operation. 

### The Stack
The Wasilewski features a 256-byte stack that supports basic stack operations. It's recommended for storing register values using the PUSHR instruction.

### The BookKeeper
The BookKeeper (BK) is a 512-byte address holder responsible for all memory operations, as it is the only component that interfaces with the ROM. It is useful for storing initial variable values.

### The ROM
The CPU includes a 1024-byte Read-Only Memory (ROM) segment. This memory is used solely for storing variables or values set by the LOAD and STORE commands.


<br>


# How to install 🚀
Clone it with git

```bash
$ git clone https://github.com/WasixXD/Wasilewski
```

Run the tests with
```bash
chmod +x ./test.sh
./test.sh
```
Create a file ending with `.was` and compile it with:
```
$ ./asm.o your_file.was output.bin
```

After that you can execute the binary with:
```
$ ./machine.o output.bin
```


___
# Basic understading
The CPU primarily performs two tasks: processing raw numeric values up to a maximum of 65536, and working with values stored in registers. Most instructions don't require a second register or any register at all.

### Arithmetic
We have four basic commands for arithmetic operations: `ADD`, `SUB`, `MUL`, and `DIV`. Each command requires you to specify the register that will receive the result, called `regA`, and a second parameter, which can either be another register (regB) or a raw value.

### Bitwise Operations
`AND`, `OR`, `NOT`, `XOR`, `SHR`, `SHL` are all bitwise operations.
- `NOT`: Don't need a second parameter
- `SHR` (Shift Right by 1 bit): Don't need a second parameter
- `SHL` (Shift Left by 1 bit): Don't need a second parameter

### Logic
- `JMP` (JUMP): Sets the PC to the line that we will jump. _Notice that the program starts counting from 0_
- `JE` (Jump if Equal): Skips the next instruction if the `regA` equals to the second parameter
- `JNE` (Jump if Not Equal): Skips the next instruction if `regA` not equals to the second parameter
- `JG` (Jump if Greater than): Skips the next instruction if `regA` is greater than the second parameter
- `JNG` (Jump if Not Greater than): Skips the next instruction if `regA` is not greater than the second parameter

### Memory operations
- `MOV` (Move): Requires both `regA` and a second parameter. The second parameter can be three things:
- - `raw value`: Numeric values that can go up to 65536
  - `$`: With the dollar sign the CPU will put the memory address of that location into `regA`
  - `[]`: Any value surronded by square brackets is referring to the value "inside" that memory address. That will be put in to `regA`
- `LOAD`: Expects both parameters and always load the value "inside" of that memory address. `LOAD` does not require the `[]`
- `STORE`: Store the value of `regA` on a specifyc memory address setted up by the user

### Stack operations
- `PUSH`: Push eithers `regA` or the value specified to the stack
- `POP`: Pop to `regA` the value from the stack
- `PUSHR`: Push all registers to the stack
- `POPR`: Pop to registers the values from the stack

### Procedures
- `CALL`: Push the PC to the stack and then Jumps to specified procedure line
- `RET`: Pop to PC the value to the stack and Jumps back to previously line <br>
When writing the procedure don't forget to put the `name` + a `:`. Ex: `procedure_number_1:`

### SYS CALLS
The CPU can handle only two SYS CALLS: read and write. And both have some differences because reading a number != string
- To read a string
- - First set register `B` to 0
  - Then register `C` to 1 (reading string)
  - Finally put on `D` the `size of string + 1`
  - The value read will be put on register `A`
- To read a number
- - First set register `B` to 0
  - Then register `C` to 0 (reading a number)
  - The value read will be put on register `A`

- To write a string
- - First set register `B` to 1
  - Then register `C` to 1 (writing string)
  - Finally put on `D` the `size of string + 1`
  - The value written will be the register `A`
- To write a number
- - First set register `B` to 1
  - Then register `C` to 0 (writing a number)
  - The value written will be the register `A`



___

# ▶ DEMO

### Basic procedure
```asm
0 MOV a, 2
1 CALL double; assembler -> CALL 5
2 ADD a, 1
3 HLT
4 
5 double: 
6 MUL a, 2
7 RET
8 HLT
```

binary
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

Print the Fibonacci number at the index specified by the user.
```asm
MOV d, 0
MOV b, 0;b = 0
MOV c, 0;c = 0
SYS
STORE a, 10; [10] = a
MOV a, 0; a = 0
MOV b, 1; b = 1
ADD c, a; c += a
ADD c, b; c += b
MOV a, b; a = b
MOV b, c; b = c
PUSH a; [a]
PUSH b; [a b]
MOV a, c; a = c
PUSH c; [a b c]
MOV b, 1; b = 1
MOV c, 0; c = 0
SYS 
POP c; c = [a b *c*]
POP b; b = [a *b*]
POP a; a = [*a*]
PUSH a; [a]
LOAD a, 10; a = [10]
JE a, d; a == d?
HLT
POP a; a = [*a*]
ADD d, 1; d += 1
XOR c, c; c ^ c
JMP 7; goto 7
```
binary
```
0001 0101 0100 0000 0000 0000 0000 0000
0001 0101 0010 0000 0000 0000 0000 0000
0001 0101 0011 0000 0000 0000 0000 0000
1000 0000 0000 0000 0000 0000 0000 0000
0001 0111 0001 0000 0000 0000 0000 1010
0001 0101 0001 0000 0000 0000 0000 0000
0001 0101 0010 0000 0000 0000 0000 0001
0000 0001 0011 0001 0000 0000 0000 0000
0000 0001 0011 0010 0000 0000 0000 0000
0001 0101 0001 0010 0000 0000 0000 0000
0001 0101 0010 0011 0000 0000 0000 0000
0001 1000 0001 0000 0000 0000 0000 0000
0001 1000 0010 0000 0000 0000 0000 0000
0001 0101 0001 0011 0000 0000 0000 0000
0001 1000 0011 0000 0000 0000 0000 0000
0001 0101 0010 0000 0000 0000 0000 0001
0001 0101 0011 0000 0000 0000 0000 0000
1000 0000 0000 0000 0000 0000 0000 0000
0001 1001 0011 0000 0000 0000 0000 0000
0001 1001 0010 0000 0000 0000 0000 0000
0001 1001 0001 0000 0000 0000 0000 0000
0001 1000 0001 0000 0000 0000 0000 0000
0001 0110 0001 0000 0000 0000 0000 1010
0001 0001 0001 0100 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
0001 1001 0001 0000 0000 0000 0000 0000
0000 0001 0100 0000 0000 0000 0000 0001
0000 1000 0011 0011 0000 0000 0000 0000
0001 0000 0000 0000 0000 0000 0000 0111
```
___
Counting the number of 1 in the user input

```asm
MOV b, 0;b = 0
MOV c, 0;c = 0
SYS ;a = sys
ADD b, 1 ;1 b++
SHR a;a = a >> 1  
JE  e, 1 ;if e == 1
ADD c, 1 ;c++
JE  b, 8   ;if b == 8
CALL print
JMP 3   ;goto 3
print:
    MOV a, c; a = c
    MOV b, 1; b = 1
    MOV c, 0; c = 0
    SYS 
    HLT
```
binary
```
0001 0101 0010 0000 0000 0000 0000 0000
0001 0101 0011 0000 0000 0000 0000 0000
1000 0000 0000 0000 0000 0000 0000 0000
0000 0001 0010 0000 0000 0000 0000 0001
0000 1001 0001 0000 0000 0000 0000 0000
0001 0001 0101 0000 0000 0000 0000 0001
0000 0001 0011 0000 0000 0000 0000 0001
0001 0001 0010 0000 0000 0000 0000 1000
0001 1100 0000 0000 0000 0000 0000 1010
0001 0000 0000 0000 0000 0000 0000 0011
0000 0000 0000 0000 0000 0000 0000 0000
0001 0101 0001 0011 0000 0000 0000 0000
0001 0101 0010 0000 0000 0000 0000 0001
0001 0101 0011 0000 0000 0000 0000 0000
1000 0000 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
```
___
Calculate the factorial based on user input
```asm
MOV b, 0
MOV c, 0
SYS
MOV d, a
SUB d, 1
MUL a, d
JE d, 1
CALL print
JMP 4
print: 
    MOV b, 1
    MOV c, 0
    SYS
    HLT
```
binary
```
0001 0101 0010 0000 0000 0000 0000 0000
0001 0101 0011 0000 0000 0000 0000 0000
1000 0000 0000 0000 0000 0000 0000 0000
0001 0101 0100 0001 0000 0000 0000 0000
0000 0010 0100 0000 0000 0000 0000 0001
0000 0011 0001 0100 0000 0000 0000 0000
0001 0001 0100 0000 0000 0000 0000 0001
0001 1100 0000 0000 0000 0000 0000 1001
0001 0000 0000 0000 0000 0000 0000 0100
0000 0000 0000 0000 0000 0000 0000 0000
0001 0101 0010 0000 0000 0000 0000 0001
0001 0101 0011 0000 0000 0000 0000 0000
1000 0000 0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 0000 0000 0000
``` 


# OP CODES Cheatsheet

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

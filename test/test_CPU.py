import unittest
import subprocess
import util

class TestCPU(unittest.TestCase):

    def tearDown(self):
        util.remove_file(self.path)

    def test_add(self):
        self.path = "./test/add.bin"
        program = [
            "00000001000100000000000000100010",
            "00000001000100000000000000100010",
            "00000001000100000000000000100010",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] ADD\n[x] ADD\n[x] ADD\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_sub(self):
        self.path = "./test/sub.bin"
        program = [
            "00000010000000000000000000000000",
            "00000010000000000000000000000000",
            "00000010000000000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] SUB\n[x] SUB\n[x] SUB\n[x] HLT\n"
        self.assertEqual(stdout, expected)
    
    def test_mul(self):
        self.path = "./test/mul.bin"
        program = [
            "00000011000100000000000000000000",
            "00000011000100000000000000000000",
            "00000011000100000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] MUL\n[x] MUL\n[x] MUL\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_div_with_zero(self):
        self.path = "./test/div.bin"
        program = [
            "00000100000100000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[*] ZERO DIVISION EXCEPTION\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_div(self):
        self.path = "./test/div.bin"
        program = [
            "00000100000100000000000000000001",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] DIV\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_and(self):
        self.path = "./test/and.bin"
        program = [
            "00000101000100010000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] AND\n[x] HLT\n"
        self.assertEqual(stdout, expected)


    def test_or(self):
        self.path = "./test/or.bin"
        program = [
            "00000110000100010000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] OR\n[x] HLT\n"
        self.assertEqual(stdout, expected)


    def test_fail_not(self):
        self.path = "./test/not.bin"
        program = [
            "00000111000000000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[*] NOT INSTRUCTION SHOULD HAVE A VALUE\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_not(self):
        self.path = "./test/not.bin"
        program = [
            "00000111000100000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] NOT\n[x] HLT\n"
        self.assertEqual(stdout, expected)
 
    def test_je(self):
        self.path = "./test/je.bin"
        program = [
            "00000001000100000000000000000001",
            "00010001000100000000000000000010",
            "00000000000000000000000000000000",
            "00010000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] ADD\n[x] JE\n[x] JMP\n[x] ADD\n[x] JE\n[x] HLT\n"
        self.assertEqual(stdout, expected)       

    def test_jne(self):
        self.path = "./test/jne.bin"
        program = [
            "00000001000100000000000000000001", # a += 1
            "00010010000100000000000000000010", # if a != 2
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] ADD\n[x] JNE\n[x] HLT\n"
        self.assertEqual(stdout, expected)   

    def test_jg(self):
        self.path = "./test/jg.bin"
        program = [
            "00000001000100000000000000000100", # a += 4
            "00010011000100000000000000000111", # if a > 7
            "00000000000000000000000000000000", # HLT
            "00010000000000000000000000000000", # JMP 0
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] ADD\n[x] JG\n[x] JMP\n[x] ADD\n[x] JG\n[x] HLT\n"
        self.assertEqual(stdout, expected)   

    def test_jng(self):
        self.path = "./test/jg.bin"
        program = [
            "00000001000100000000000000000100", # a += 4
            "00010100000100000000000000000111", # if a < 7
            "00000000000000000000000000000000", # HLT
            "00010000000000000000000000000000", # JMP 0
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] ADD\n[x] JNG\n[x] HLT\n"
        self.assertEqual(stdout, expected)  

    def test_shr(self):
        self.path = "./test/shr.bin"
        program = [
            "00010101000100000000000000000001", # a = 1
            "00001001000100000000000000000000", # a >> 1
            "00010001010100000000000000000001", # JE e == 1
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] MOV\n[x] SHR\n[x] JE\n[x] HLT\n"
        self.assertEqual(stdout, expected)  
    def test_shl(self):
        self.path = "./test/jg.bin"
        program = [
            "00010101000100000000000000000001", # a = 1
            "00001010000100000000000000000000", # a >> 1
            "00010001010100000000000000000000", # JE e == 0
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] MOV\n[x] SHL\n[x] JE\n[x] HLT\n"
        self.assertEqual(stdout, expected)  

if __name__ == "__main__":
    unittest.main()

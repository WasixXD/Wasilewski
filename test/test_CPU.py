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
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] ADD\n[x] HLT\n";
        self.assertEqual(stdout, expected)

    def test_sub(self):
        self.path = "./test/sub.bin"
        program = [
            "00000010000000000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] SUB\n[x] HLT\n"
        self.assertEqual(stdout, expected)
    
    def test_mul(self):
        self.path = "./test/mul.bin"
        program = [
            "00000011000100000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] MUL\n[x] HLT\n"
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
        expected = "[*] ZERO DIVISION EXCEPTION\n"
        self.assertEqual(stdout, expected)

    def test_div(self):
        self.path = "./test/div.bin"
        program = [
            "00000100000100000000000000000001",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] DIV\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_and(self):
        self.path = "./test/and.bin"
        program = [
            "00000101000100010000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] AND\n[x] HLT\n"
        self.assertEqual(stdout, expected)


    def test_or(self):
        self.path = "./test/or.bin"
        program = [
            "00000110000100010000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] OR\n[x] HLT\n"
        self.assertEqual(stdout, expected)


    def test_fail_not(self):
        self.path = "./test/not.bin"
        program = [
            "00000111000000000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[*] NOT INSTRUCTION SHOULD HAVE A VALUE\n"
        self.assertEqual(stdout, expected)

    def test_not(self):
        self.path = "./test/not.bin"
        program = [
            "00000111000100000000000000000000",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] NOT\n[x] HLT\n"
        self.assertEqual(stdout, expected)
 
    def test_je(self):
        self.path = "./test/je.bin"
        program = [
            "00010001000100000000000000000010",
            "00000000000000000000000000000000",
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] JE\n[x] HLT\n"
        self.assertEqual(stdout, expected)       

    def test_jne(self):
        self.path = "./test/jne.bin"
        program = [
            "00010010000100000000000000000010", # if a != 2
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] JNE\n[x] HLT\n"
        self.assertEqual(stdout, expected)   

    def test_jg(self):
        self.path = "./test/jg.bin"
        program = [
            "00010011000100000000000000000111", # if a > 7
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] JG\n[x] HLT\n"
        self.assertEqual(stdout, expected)   

    def test_jng(self):
        self.path = "./test/jg.bin"
        program = [
            "00010100000100000000000000000111", # if a < 7
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] JNG\n[x] HLT\n"
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

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] MOV\n[x] SHR\n[x] JE\n[x] HLT\n"
        self.assertEqual(stdout, expected)  
    def test_shl(self):
        self.path = "./test/jg.bin"
        program = [
            "00010101000100000000000000000001", # a = 1
            "00001010000100000000000000000000", # a << 1
            "00010001010100000000000000000000", # JE e == 0
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] MOV\n[x] SHL\n[x] JE\n[x] HLT\n"
        self.assertEqual(stdout, expected)  
    def test_mov(self):
        self.path = "./test/mov.bin"
        program = [
            "00010101000100000000000000000001", # a = 1
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] MOV\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_load(self):
        self.path = "./test/load.bin"
        program = [
            "00010110000100000000000000000001", # LOAD
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] LOAD\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_store(self):
        self.path = "./test/load.bin"
        program = [
            "00010111000100000000000000000001", # STORE
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] STORE\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_push(self):
        self.path = "./test/push.bin"
        program = [
            "00011000000100000000000000000001", # PUSH
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] PUSH\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_pop(self):
        self.path = "./test/pop.bin"
        program = [
            "00011001000100000000000000000001", # POP
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] POP\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_pushr(self):
        self.path = "./test/pushr.bin"
        program = [
            "00011010000100000000000000000001", # PUSHR
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] PUSHR\n[x] HLT\n"
        self.assertEqual(stdout, expected)


    def test_popr(self):
        self.path = "./test/popr.bin"
        program = [
            "00011011000100000000000000000001", # POPR
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] POPR\n[x] HLT\n"
        self.assertEqual(stdout, expected)


    def test_call(self):
        self.path = "./test/call.bin"
        program = [
            "00011100000100000000000000000001", # CALL
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] CALL\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_ret(self):
        self.path = "./test/call.bin"
        program = [
            "00011101000100000000000000000001", # RET
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] RET\n[x] HLT\n"
        self.assertEqual(stdout, expected)


    def test_sys(self):
        self.path = "./test/call.bin"
        program = [
            "10000000000100000000000000000001", # SYS
            "00000000000000000000000000000000", # HLT
        ]
        util.write_file(self.path, program)

        resultado = subprocess.run(["./machine.o", self.path, "1"], stdout=subprocess.PIPE)
        stdout = resultado.stdout.decode('utf-8')
        expected = "[x] SYS\n[x] HLT\n"
        self.assertEqual(stdout, expected)






if __name__ == "__main__":
    unittest.main()

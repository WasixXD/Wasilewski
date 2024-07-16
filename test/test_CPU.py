import unittest
import subprocess
import util
from time import sleep

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



        
if __name__ == "__main__":
    unittest.main()

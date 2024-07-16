import util
import subprocess
import unittest


counter = 0

programs = [
    {
        "path": "./test/add.bin",
        "program": [
            "00000001000100000000000000100010",
            "00000001000100000000000000100010",
            "00000001000100000000000000100010",
            "00000000000000000000000000000000",
        ]
    },
    {
        "path": "./test/sub.bin",
        "program": [
            "00000010001000000000000000000001",
            "00000010001000000000000000000001",
            "00000010001000000000000000000001",
            "00000000000000000000000000000000",
        ]
    },

]  

class TestCPU(unittest.TestCase):

    def setUp(self):
        global counter, program
        util.write_file(programs[counter]['path'], programs[counter]['program'])

    def tearDown(self):
        global counter, program
        util.remove_file(programs[counter]['path'])
        counter += 1

    def test_add(self):
        global counter, program
        resultado = subprocess.run(["./machine.o", programs[counter]['path']], stdout=subprocess.PIPE)

        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] ADD\n[x] ADD\n[x] ADD\n[x] HLT\n"
        self.assertEqual(stdout, expected)

    def test_sub(self):
        global counter, program
        resultado = subprocess.run(["./machine.o", programs[counter]['path']], stdout=subprocess.PIPE)

        stdout = resultado.stdout.decode('utf-8')
        expected = "Wasix CPU Machine\n[x] SUB\n[x] SUB\n[x] SUB\n[x] HLT\n"
        self.assertEqual(stdout, expected)
        
if __name__ == "__main__":
    unittest.main()

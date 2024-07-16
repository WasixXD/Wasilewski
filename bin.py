binary_data = [
"00000100000100000000000000000000",
"00000100000100000000000000000000",
"00000100000100000000000000000000",
"00000000000000000000000000000000", 
]

with open("div.bin", "wb") as file:
    for binary_string in binary_data:
        file.write(int(binary_string, 2).to_bytes(4, byteorder='big'))

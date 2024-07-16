import pathlib

def write_file(file_name: str, binary_data: [str]):
    with open(file_name, "wb") as file:
        for binary_string in binary_data:
            file.write(int(binary_string, 2).to_bytes(4, byteorder='big'))


def remove_file(file_name: str):
    path_file = pathlib.Path(file_name)
    path_file.unlink()

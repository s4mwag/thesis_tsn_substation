import numpy as np
import os

def generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket):
    random_numbers = np.random.uniform(low=simulationStart, high=simulationEnd, size=noOfPackets)
    sorted_numbers = np.sort(random_numbers)
    formatted_numbers = [f"{number:.6f} {bytesPerPacket};" for number in sorted_numbers]
    output = " ".join(formatted_numbers)
    return output

def write_to_file(filename, content):
    file_exists = os.path.exists(filename)
    file_index = 1
    unique_filename = filename

    while file_exists:
        unique_filename = f"{filename.split('.')[0]}_{file_index}.txt"
        file_exists = os.path.exists(unique_filename)
        file_index += 1

    with open(unique_filename, 'w') as file:
        file.write(content)

noOfPackets = 8333
simulationStart = 0
simulationEnd = 10
bytesPerPacket = 84
output = generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket)

filename = "sendscript.txt"
write_to_file(filename, output)
print(f"Output written to {filename}")
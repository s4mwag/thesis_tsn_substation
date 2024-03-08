import numpy as np
import os

def generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket):
    random_numbers = np.random.uniform(low=simulationStart, high=simulationEnd, size=noOfPackets)
    sorted_numbers = np.sort(random_numbers)
    formatted_numbers = [f"{number:.6f} {bytesPerPacket};" for number in sorted_numbers]
    output = " ".join(formatted_numbers)
    return output

def write_to_file(filename, content):
    base_filename, file_extension = filename.split('.')
    file_index = 1
    unique_filename = f"{base_filename}_{file_index}.{file_extension}"
    file_exists = os.path.exists(unique_filename)

    while file_exists:
        file_index += 1
        unique_filename = f"{base_filename}_{file_index}.{file_extension}"
        file_exists = os.path.exists(unique_filename)

    with open(unique_filename, 'w') as file:
        file.write(content)
    print(f"Output written to {unique_filename}")
    
noOfPackets = 833
simulationStart = 0
simulationEnd = 10
bytesPerPacket = 1434
noOfFiles = 4
filename = "sendscriptIT.txt"

for i in range(noOfFiles):
    output = generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket)
    write_to_file(filename, output)
    
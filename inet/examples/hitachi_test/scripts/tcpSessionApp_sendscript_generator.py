import numpy as np
import os, glob, sys

noOfPackets = 0
simulationStart = 0
simulationEnd = 0
bytesPerPacket = 0
noOfFiles = 0
filename = ""
seed_value = int(sys.argv[1]) * 100

def configuration_mms_traffic():
    global noOfPackets, simulationStart, simulationEnd, bytesPerPacket, noOfFiles, filename
    noOfPackets = 8333
    simulationStart = 0
    simulationEnd = 10
    bytesPerPacket = 84
    noOfFiles = 3
    filename = "sendscript.txt"

def configuration_it_traffic():
    global noOfPackets, simulationStart, simulationEnd, bytesPerPacket, noOfFiles, filename
    noOfPackets = 833
    simulationStart = 0
    simulationEnd = 10
    bytesPerPacket = 1434
    noOfFiles = 4
    filename = "sendscriptIT.txt"

def configuration_log_traffic():
    global noOfPackets, simulationStart, simulationEnd, bytesPerPacket, noOfFiles, filename
    noOfPackets = 50
    simulationStart = 0
    simulationEnd = 10
    bytesPerPacket = 84
    noOfFiles = 12
    filename = "sendscriptLOG.txt"

def generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket):
    global seed_value
    seed_value += 1
    if len(sys.argv) > 1:
        np.random.seed(seed_value) 

            
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
    
def generate_scripts(script):
    if script == "mms":
        configuration_mms_traffic()
        for i in range(noOfFiles):
            output = generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket)
            write_to_file(filename, output)
    elif script == "log":
        configuration_log_traffic()
        for i in range(noOfFiles):
            output = generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket)
            write_to_file(filename, output)
    elif script == "it":
        configuration_it_traffic()
        for i in range(noOfFiles):
            output = generate_sorted_numbers(noOfPackets, simulationStart, simulationEnd, bytesPerPacket)
            write_to_file(filename, output)
            
def remove_text_files():
    files = glob.glob('./*.txt')
    for f in files:
        os.remove(f)
        
def move_text_files():
    files = glob.glob('./*.txt')
    for f in files:
        os.replace(f, '../'+f) 


        
remove_text_files()
generate_scripts("mms")
generate_scripts("log")
generate_scripts("it")
move_text_files()




    
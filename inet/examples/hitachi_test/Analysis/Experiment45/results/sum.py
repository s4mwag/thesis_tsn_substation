import re

# Open the file
with open("GCL_for_omnet_sorted.txt", "r") as file:
    # Iterate over each line in the file
    for line in file:
        # Extract values containing "ns" in the list
        ns_values = re.findall(r'\d+ns', line)
        # Convert each extracted value to integer and sum them up
        row_sum = sum(int(value[:-2]) for value in ns_values)
        # Print the sum for the current row
        if row_sum > 0:
            print("Sum for row:", row_sum, "ns")
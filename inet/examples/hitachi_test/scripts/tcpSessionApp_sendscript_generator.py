import numpy as np

#This function generates an amount of numbers that can be used as a sendscript.txt file for a TcpSessionApp in Omnet INET
def generate_sorted_numbers(amount, lower_bound, upper_bound, constant_value):
    # Generate the specified amount of random numbers within the given range
    random_numbers = np.random.uniform(low=lower_bound, high=upper_bound, size=amount)
    
    # Sort the numbers in ascending order
    sorted_numbers = np.sort(random_numbers)
    
    # Format each number with 6 decimal points followed by the constant value, separated by a semicolon
    formatted_numbers = [f"{number:.6f} {constant_value};" for number in sorted_numbers]
    
    # Join all formatted numbers into a single string
    output = " ".join(formatted_numbers)
    return output

# Example usage
amount = 100
lower_bound = 0
upper_bound = 10
constant_value = 100
output = generate_sorted_numbers(amount, lower_bound, upper_bound, constant_value)
print(output)

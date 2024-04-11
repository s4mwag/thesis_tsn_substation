import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

labels = ["E-L", "E-H", "E-PQ-L", "E-PQ-H", "TSN-L", "TSN-H"]  # Labels for each box plot
eth_h_messages = ['eth_h/GOOSE 1A', 'eth_h/GOOSE 1B', 'eth_h/MMS', 'eth_h/SV']
eth_l_messages = ['eth_l/GOOSE 1A', 'eth_l/GOOSE 1B', 'eth_l/MMS', 'eth_l/SV']
eth_p_l_messages = ['eth_p_l/GOOSE 1A', 'eth_p_l/GOOSE 1B', 'eth_p_l/MMS', 'eth_p_l/SV']
eth_p_h_messages = ['eth_p_h/GOOSE 1A', 'eth_p_h/GOOSE 1B', 'eth_p_h/MMS', 'eth_p_h/SV']
TAS_l_messages = ['TAS_l/GOOSE 1A', 'TAS_l/GOOSE 1B', 'TAS_l/MMS', 'TAS_l/SV']
TAS_h_messages = ['TAS_h/GOOSE 1A', 'TAS_h/GOOSE 1B', 'TAS_h/MMS', 'TAS_h/SV']

for message in eth_p_h_messages:
    title = 'End-to-End Delay for ' + message + ' messages'
    folder_path = message
    # List all CSV files in the folder
    csv_files = sorted([f for f in os.listdir(folder_path) if f.endswith('.csv')])

    # Initialize a list to store data for plotting
    plot_data = []


    for file in csv_files:
        file_path = os.path.join(folder_path, file)
        df = pd.read_csv(file_path)
        
        y_columns = [col for i, col in enumerate(df.columns) if i % 2 != 0]
        
        # Concatenate all y-axis values into a single series and add to the list
        combined_y_values = pd.concat([df[col] for col in y_columns], axis=0) * 1e6
        combined_y_values = combined_y_values.dropna()
        plot_data.append(combined_y_values)
    
    
    min_e2e = np.min(plot_data)
    max_e2e = np.max(plot_data)
    mean_e2e = np.mean(plot_data)
    sd_e2e = np.std(plot_data)

    print(message + " Min E2E Delay: " + str(min_e2e) + " us")
    print(message + " Max E2E Delay: " + str(max_e2e) + " us")
    print(message + " Mean E2E Delay: " + str(mean_e2e) + " us")
    print(message + " SD E2E Delay: " + str(sd_e2e) + " us")
    print("\n")
    """
    flierprops=dict(marker='_', markersize=6)
    medianprops = dict(linestyle='-', linewidth=2.5, color='black')
    meanprops = dict(linestyle='--', linewidth=2.5, color='black')
    plt.rcParams['font.family'] = 'DejaVu Serif'
    plt.rcParams['font.size'] = 12
    # Creating the box plot for all datasets
    plt.figure(figsize=(10, 6))  # Adjust the figure size as needed
    plt.boxplot(plot_data, labels=labels, whis=(0.001,99.999), showfliers=True, showmeans=True, meanline=True, flierprops=flierprops, medianprops=medianprops, meanprops=meanprops)  # Use the labels for each plot
    plt.title(title)
    plt.ylabel('Time (μs)')
    # Adding a subtle grid
    plt.grid(which='major', linestyle='--', linewidth='0.8', color='lightsteelblue')
    plt.minorticks_on()
    plt.grid(which='minor', linestyle=':', linewidth='0.5', color='lightsteelblue')
    plt.savefig(str(message)+'.pdf', format='pdf')
    #plt.show()
    """
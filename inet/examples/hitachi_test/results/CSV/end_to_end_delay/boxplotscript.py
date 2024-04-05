import os
import pandas as pd
import matplotlib.pyplot as plt

folder_path = 'ethernet_low_load_prio'
title = 'End-to-End Delay Standard Ethernet With Priority Queueing and Low Traffic Load'

# List all CSV files in the folder
csv_files = sorted([f for f in os.listdir(folder_path) if f.endswith('.csv')])

# Initialize a list to store data for plotting
plot_data = []
labels = ["GOOSE 1A", "GOOSE 1B", "MMS", "SV"]  # Labels for each box plot

for file in csv_files:
    file_path = os.path.join(folder_path, file)
    df = pd.read_csv(file_path)
    
    y_columns = [col for i, col in enumerate(df.columns) if i % 2 != 0]
    
    # Concatenate all y-axis values into a single series and add to the list
    combined_y_values = pd.concat([df[col] for col in y_columns], axis=0) * 1e6
    combined_y_values = combined_y_values.dropna()
    plot_data.append(combined_y_values)
    
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
plt.savefig(str(title)+'.pdf', format='pdf')
plt.show()

import pandas as pd
import os
import glob

def process_csv(file_path, result_file_path):
    min_value = float('inf')
    max_value = float('-inf')
    total_sum = 0
    count = 0

    df = pd.read_csv(file_path)
    
    for i in range(1, len(df.columns), 2):
        df.iloc[0:, i] = (df.iloc[0:, i] - df.iloc[2:, i - 1]) * 1000000
        current_min_value = df.iloc[0:, i].min()
        current_max_value = df.iloc[0:, i].max()
        min_value = min(current_min_value, min_value)
        max_value = max(current_max_value, max_value)
        total_sum += df.iloc[0:, i].sum()
        count += len(df.iloc[0:, i].dropna())

    average_value = total_sum / count if count != 0 else 0

    stats_df = pd.DataFrame({
        'Statistic': ['Min', 'Max', 'Average'],
        'Value': [min_value, max_value, average_value]
    })

    stats_df.to_csv(result_file_path, index=False)
    print(f"Stats saved to {result_file_path}")
    

# You would call the function with the path to your CSV file
directory_path = "..\\results\\CSV\\"
search_pattern = directory_path + "sync*.csv"

for file_path in glob.glob(search_pattern):
    filename = os.path.basename(file_path)
    result_filename = "stats" + filename
    result_file_path = os.path.join(directory_path, result_filename)
    process_csv(file_path, result_file_path)
    


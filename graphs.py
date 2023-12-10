import os
import csv
import matplotlib.pyplot as plt
import numpy as np

folder_path = 'measures'

csv_files = [file for file in os.listdir(folder_path) if file.endswith('.csv')]

all_programs = {}


for csv_file in csv_files:
    file_path = os.path.join(folder_path, csv_file)


    with open(file_path, newline='') as csvfile:
        data = list(csv.DictReader(csvfile))


    programs = {}
    for row in data:
        program_name = row['program']
        category = program_name.split('-')[0] 

        if category not in programs:
            programs[category] = {}

        if program_name not in programs[category]:
            programs[category][program_name] = {'threads': [], 'time': []}

        programs[category][program_name]['threads'].append(int(row['num_threads']))
        programs[category][program_name]['time'].append(float(row['time']))


    for category, program_data in programs.items():
        if category not in all_programs:
            all_programs[category] = {}

        for program_name, results in program_data.items():
            if program_name not in all_programs[category]:
                all_programs[category][program_name] = {'threads': [], 'time': []}

            all_programs[category][program_name]['threads'].extend(results['threads'])
            all_programs[category][program_name]['time'].extend(results['time'])


fig, axs = plt.subplots(len(all_programs), 1, figsize=(10, 6 * len(all_programs)), sharex=True)

for i, (category, program_data) in enumerate(all_programs.items()):
	ax = axs[i]

	for program_name, results in program_data.items():
		avg_time = np.mean(results['time'])
		std_dev = np.std(results['time'])

		ax.errorbar(results['threads'], results['time'], yerr=std_dev, fmt='o-', label=f'{program_name} (Avg: {avg_time:.2f} s)')

	ax.set_title(f'Category: {category}')
	ax.set_ylabel('Execution Time (s)')
	ax.set_xlabel('Threads')
	ax.legend()
	ax.grid(True)

plt.xlabel('Number of Threads')
plt.suptitle(f'Execution Time vs Number of Threads (Grouped by Category)', y=0.92, fontsize=16)
plt.show()
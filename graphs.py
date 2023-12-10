import csv
import matplotlib.pyplot as plt
import numpy as np

with open('perf_measures.csv', newline='') as csvfile:
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

#Plotting
fig, axs = plt.subplots(len(programs), 1, figsize=(10, 6 * len(programs)), sharex=True)

for i, (category, program_data) in enumerate(programs.items()):
    ax = axs[i]

    for program_name, results in program_data.items():
        avg_time = np.mean(results['time'])
        std_dev = np.std(results['time'])

        ax.errorbar(results['threads'], results['time'], yerr=std_dev, fmt='o-', label=f'{program_name} (Avg: {avg_time:.2f} s)')

    ax.set_title(f'Category: {category}')
    ax.set_ylabel('Execution Time (s)')
    ax.legend()
    ax.grid(True)

plt.xlabel('Number of Threads')
plt.suptitle('Execution Time vs Number of Threads (Grouped by Category)', y=0.92, fontsize=16)
plt.show()
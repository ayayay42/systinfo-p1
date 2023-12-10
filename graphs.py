import pandas as pd
import numpy as np
from matplotlib import pyplot as plt
try:
    import seaborn as sns #pour de plus beaux graph askip 
except ModuleNotFoundError:
    print("Seaborn not found")
else:
    sns.set()

#program_names = ["philosophes", "philosophes-my-functions", "thread_test"] #noms des programmes à évaluer
program_names = ["readerWriter"] #TODO: ajouter tous les programmes à plot 
df = pd.read_csv("perf_measures.csv")
#df["time"] *= 1000 #in ms 

for i in range(len(program_names)):
	current = program_names[i] #nom du programme courant
	plt.figure(figsize=(10, 5))
	plt.xlabel("nb thread")
	#plt.xscale('log', base = 2)
	#plt.xlim(left=1, right=68)
	plt.ylabel("temps en s")

	#peut-être plus faire une boucle sur les programmes et faire un subplot pour chaque programme pour mieux s'y retrouver et que y ai le bon nom
	plt.title("Eval perf pour le programme: " + current) 

	current_df = df[df["program"] == current]
	grouped_data = current_df[['num_threads', 'time']].groupby("num_threads").agg(['mean', 'std']) 
	print(grouped_data)
	print(grouped_data['time']['mean'])
	print(grouped_data['time']['std'])

	plt.errorbar(grouped_data.index, grouped_data['time']['mean'], yerr=grouped_data['time']['std'], fmt='-o')
	#TODO: ajouter l'errorbar de nos implémentations sur le même graph que l'original (donc 3 lignes)
	max_val = max(grouped_data['time']['mean'])
	plt.ylim(bottom=0, top=max_val*1.2)
	plt.savefig("plots/" + current + ".png")

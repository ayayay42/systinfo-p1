#!/bin/bash
make 

#1.4 et 2.2

rm -f perf_measures.csv || true

program_names=("readerWriter-my-functions" "readerWriter") #TODO: ajouter tous les fichiers à exécuter .c


num_threads=(2 4 8 16 32 64)

num_mesures=$(seq 5)

echo "program,num_threads,num_mesures,time" > perf_measures.csv

for program in ${program_names[@]}; do
	echo "Running $program"
	for thread in ${num_threads[@]}; do
		for i in $num_mesures; do
		echo "Running $program with $thread threads, mesure $i"
			temps=$({ \time -p ./bin/"$program" "$thread" "$thread"; } 2>&1 | grep real | awk '{print $2}')
			echo $program,$thread,$i,$temps >> perf_measures.csv
		done
	done
done


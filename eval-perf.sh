#!/bin/bash
make 

rm -f perf_measures.csv || true

program_names=("philosophes") #, "producerConsumer", "readerWriter"}

num_threads=(2 4 8 16 32 64)

num_mesures=$(seq 5)

echo "program,num_threads,num_mesures,time" > perf_measures.csv

for program in $program_names; do
	for thread in ${num_threads[@]}; do
		for i in $num_mesures; do
			temps=$({ \time -p ./"$program" "$thread" "$thread"; } 2>&1 | grep real | awk '{print $2}')
			echo $program,$thread,$i,$temps >> perf_measures.csv
		done
	done
done


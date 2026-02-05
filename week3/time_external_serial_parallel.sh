#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Test for values from 10 to 1860
# Serial case
for i in {10..1861..10}; do
	echo "$i" >> data/serial_external_timing.txt
	# Call the time function, output to a file and append to it. Save the real time. Call the c funtion with $a as its input
	\time -o data/serial_external_timing.txt --append -f "%e" ./bin/vector_serial_edited $i
done

# Parallel case
for j in {2..16}; do
	echo "$j" >> data/parallel_external_timing.txt
	for i in {10..1861..10}; do
		echo "$i" >> data/parallel_external_timing.txt
		\time -o data/parallel_external_timing.txt --append -f "%e" mpirun -np $j ./bin/vector_parallel $i
	done
done

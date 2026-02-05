#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Test for values from 10 to 1860
# Serial case
for i in {10..1861..10}; do
	echo "$i" >> data/serial_external_timing.txt
	# Call the time function, output to a file and append to it. Save the data with the format: Real, usr, sys. Call the c funtion with $a as its input
	\time -o data/serial_external_timing.txt --append -f "%e, %U, %S" ./bin/vector_serial_edited $i
done

# Parallel case
for j in {2..16}; do
	for i in {10..1861..10}; do
		\time -o data/parallel_external_timing.txt --append -f "%e, %U, %S" mpirun -np $j ./bin/vector_parallel $i
	done
done

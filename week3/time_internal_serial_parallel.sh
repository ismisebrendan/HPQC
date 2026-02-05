#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Test for values from 10 to 1860
# Serial case
for i in {10..1861..10}; do
	./bin/vector_serial_internal $i
done

# Parallel case
for j in {2..16}; do
	for i in {10..1861..10}; do
		mpirun -np $j ./bin/vector_parallel_internal $i
	done
done

#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Test for 2 to 16 processors
for j in {2..16}; do
	# Test for number of elements from 10 to 210
	for i in {10..210..20}; do
		# Repeat 10 times to get a mean
		for k in {1..10}; do
			mpirun -np $j ./bin/vector_broadcast $i
			mpirun -np $j ./bin/vector_send_recv $i
			mpirun -np $j ./bin/vector_scatter $i
		done
	done
done

#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Repeat 100 times each
for j in {0..100}; do

	# Use 2 to 16 processors
	for i in {2..16}; do
		mpirun -np $i ./bin/comm_test_Ssend_time
		mpirun -np $i ./bin/comm_test_Bsend_time
		mpirun -np $i ./bin/comm_test_Isend_time
		mpirun -np $i ./bin/comm_test_Rsend_time
	done
done

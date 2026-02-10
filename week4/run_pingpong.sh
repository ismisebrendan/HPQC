#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Test for values from 1 to 1e9
for j in {0..8}; do

	# Run for values (1 to 9)*10**j
	for i in {1..9}; do
		a=$(($i*10**$j))
		mpirun -np 2 ./bin/pingpong $a
	done
done

a=$((10**9))
mpirun -np 2 ./bin/pingpong $a

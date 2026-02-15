#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Test for values from 1 to 524288 (equivalent to a vector 2 MiB long)
for i in {0..524289}; do

	mpirun -np 2 ./bin/pingpong_vector $i
done

#!/bin/bash

# Run the file repeatedly with different numbers input
# Using internal timing for benchmarking

# Test for 20 to 500 points (steps of 10)
for i in {20..500..10}; do
	# Test for 10 to 100 cycles
	for j in {10..100..10}; do
		# Test for 10 to 100 samples
		for k in {10..100..10}; do
			./bin/string_params $i $j $k ./data/serial_out.csv
			# For the parallel case check from 3 to 16 processors
			for l in {3..16}; do
				mpirun -np $l ./bin/string_parallel $i $j $k ./data/parallel_out.csv
			done
		done
	done
done

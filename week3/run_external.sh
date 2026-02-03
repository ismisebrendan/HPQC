#!/bin/bash

# Run the file repeatedly with different numbers input
# Using the time command for benchmarking

# Test for values from 1 to 1e9
for j in {0..8}; do

	# Run for values (1 to 9)*10**j
	for i in {1..9}; do
		a=$(($i*10**$j))
		echo "$a" >> data/external_timing.txt
		# Call the time function, output to a file and append to it. Save the data with the format: Real, usr, sys. Call the c funtion with $a as its input
		\time -o data/external_timing.txt --append -f "%e, %U, %S" ./bin/vector_serial_edited $a
	done
done

a=$((10**9))
echo "$a" >> data/external_timing.txt
\time -o data/external_timing.txt --append -f "%e, %U, %S" ./bin/vector_serial_edited $a

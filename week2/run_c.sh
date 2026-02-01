#!/bin/bash

# Run the C files repeatedly with different numbers input

# Test for values from 1 to 1e9
for j in {0..8}; do

	# Run for values (1 to 9)*10**j
	for i in {1..9}; do
		a=$(($i*10**$j))
		./bin/time_print $a
		./bin/time_save $a
		./bin/time_read
	done
done

a=$((10**9))
./bin/time_print $a
./bin/time_save $a
./bin/time_read

# Remove the file created by time_save
rm ./data/c_time_out.txt

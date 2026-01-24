#!/bin/bash

# Run the python files repeatedly with different numbers input

# Test for values from 1 to 1e9
for j in {0..8}; do

	# Run for values (1 to 9)*10**j
	for i in {1..9}; do
		a=$(($i*10**$j))
		python3 ./HPQC/week2/time_print.py $a
		python3 ./HPQC/week2/time_save.py $a
		python3 ./HPQC/week2/time_read.py
	done
done


a=$((10**9))
python3 ./HPQC/week2/time_print.py $a
python3 ./HPQC/week2/time_save.py $a
python3 ./HPQC/week2/time_read.py

# Remove the file created by time_save.py
rm ./data/python_time_out.txt

# Week 2

The work done in week 2.
---

repeat_adder.py - slightly corrected version of the repeat_adder.py from the main repository for this course.

time_print.py, time_print.c - do the same as the same files as in the main repository. They also append the number of iterations and the time taken to run the file to ./data/python_time_print_results.txt or ./data/c_time_print_results.txt as the case my be.

time_save.py, time_save.c - files to time how long it takes to open a new file, write a sequence of numbers to a file and then close the file. They also append the number of iterations and the time taken to run the file to ./data/python_time_save_results.txt or ./data/c_time_save_results.txt as the case my be.

time_read.py, time_read.c - files to time how long it takes to open a file, read its contents and then close the files. The file that is opened is the file created by the corresponding time_save.* file. They also append the file size and the time taken to run the file to ./data/python_time_read_results.txt or ./data/c_time_read_results.txt as the case my be.

All files also print the time taken to the console (this of course is not counted in the timing).

Also, for all files the time taken to open and append to the *_results.txt file is not included.

The python and C files should be run from a directory which contains a sub-directory named 'data'. The python files don't necessarily have to be in this directory however.

The file run_python.sh repeatedly runs the python files with inputs from 1 to 1 billion, it uses the values 1 to 9, then 10 to 90 in steps of 10, etc. up to 1 billion.

The file run_c.sh does the same for the C files.

The C files should be compiled to a directory called 'bin'.

run_c.sh and run_python.sh should be run in the directory which contains the data directory.

If being run by run_python.sh the python files should be in the file structure ./HPQC/week2/

File structure for running the python or C files:
- Directory in which they are run/
	- data/

File structure for running the bash files:
- Directory in which they are run/
	- data/
	- bin/
	- HPQC/
		- week2/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv) 
{
	// creates and initialises the variables
	int i, input, file_size;
	i = input = 0;
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	FILE *my_file, *data_file;
	
	// gets the time before the loop
	timespec_get(&start_time, TIME_UTC);
	
	// open the file to be read
	my_file = fopen("./data/c_time_out.txt", "r");
	
	// close the file
	fclose(my_file);

	// gets the time after opening and reading the file
        timespec_get(&end_time, TIME_UTC);
	
	// calculates the runtime
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);

	// outputs the runtime
	printf("\n\nRuntime for reading the file: %lf seconds.\n\n", runtime);

	// Find size of the file that was opened
	my_file = fopen("./data/c_time_out.txt", "r");
	
	fseek(my_file, 0, SEEK_END);
	long size = ftell(my_file);

	fclose(my_file);

	// Save the runtime and input number to an output file
	data_file = fopen("./data/c_time_read_results.txt", "a");
	// File format: size of file, run time
	fprintf(data_file, "%ld, %lf \n", size, runtime);
	fclose(data_file);
	
	return 0;
}


double to_second_float(struct timespec in_time)
{
	// creates and initialises the variables
	float out_time = 0.0;
	long int seconds, nanoseconds;
	seconds = nanoseconds = 0;

	// extracts the elements from in_time
	seconds = in_time.tv_sec;
	nanoseconds = in_time.tv_nsec;

	// calculates the time in seconds by adding the seconds and the nanoseconds divided by 1e9
	out_time = seconds + nanoseconds/1e9;

	// returns the time as a double
	return out_time;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
	// creates and initialises the variables
	struct timespec time_diff;
	long int seconds, nanoseconds;                                                                                                       seconds = nanoseconds = 0;
	double runtime = 0.0;

	// extracts the elements from start_time and end_time
	seconds = end_time.tv_sec - start_time.tv_sec;
	nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

	// if the ns part is negative
	if (nanoseconds < 0)
	{
		// "carry the one!"
		seconds = seconds - 1;
		nanoseconds = ((long int) 1e9) - nanoseconds;
	}

	// creates the runtime
	time_diff.tv_sec = seconds;
	time_diff.tv_nsec = nanoseconds;

	return time_diff;
}


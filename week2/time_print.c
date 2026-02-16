#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);


int main(int argc, char **argv) 
{
	// Create and initialise the variables
	int i, input;
	i = input = 0;
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	FILE *data_file;

	// Check if there are the right number of arguments
	if (argc == 2)
	{
		// Convert the first argument to an integer
		input = atoi(argv[1]);
	}
	else
	{
		// Raise an error
		fprintf(stderr, "Incorrect arguments.  Usage: time_print [NUM]\ne.g. \n time_print 3\n");
		// Crash out
		exit(-1);
	}
	
	// Get the time before the loop
	timespec_get(&start_time, TIME_UTC);
	// Iterate over all numbers up the input
	for (i = 0; i < input; i++)
	{
		// Print the index
		printf("%d, ", i);
	}
	// Get the time after the loop
    timespec_get(&end_time, TIME_UTC);

	// Calculate the runtime
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);


	// Output the runtime
	printf("\n\nRuntime for core loop: %lf seconds.\n\n", runtime);

	// Save the runtime and input number to an output file
	data_file = fopen("./data/c_time_print_results.txt", "a");
	// File format: number of iterations, run time
	fprintf(data_file, "%d, %lf \n", input, runtime);
	fclose(data_file);

	return 0;
}


double to_second_float(struct timespec in_time)
{
	// Create and initialise the variables
	float out_time = 0.0;
	long int seconds, nanoseconds;
	seconds = nanoseconds = 0;

	// Extract the elements from in_time
	seconds = in_time.tv_sec;
	nanoseconds = in_time.tv_nsec;

	// Calculate the time in seconds by adding the seconds and the nanoseconds divided by 1e9
	out_time = seconds + nanoseconds/1e9;

	// Return the time as a double
	return out_time;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
	// Create and initialise the variables
	struct timespec time_diff;
	long int seconds, nanoseconds;
	seconds = nanoseconds = 0;
	double runtime = 0.0;

	// Extract the elements from start_time and end_time
	seconds = end_time.tv_sec - start_time.tv_sec;
	nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

	// If the ns part is negative
	if (nanoseconds < 0)
	{
		// "carry the one!"
		seconds = seconds - 1;
		nanoseconds = ((long int) 1e9) - nanoseconds;
	}

	// Create the runtime
	time_diff.tv_sec = seconds;
	time_diff.tv_nsec = nanoseconds;

	return time_diff;
}
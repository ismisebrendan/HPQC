#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Add in the stuff necessary for timing from last week
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

// Declares the functions that will be called within main
// Note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector(int vector[], int size);

int main(int argc, char **argv)
{
	// For timing purposes
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	FILE *data_file;

	// Declare and initialise the numerical argument variable
	int num_arg = check_args(argc, argv);

	// Create a vector variable
	int* my_vector = malloc (num_arg * sizeof(int));
	
	// Get the time before initialising the vector
	timespec_get(&start_time, TIME_UTC);
	
	// And initialise every element
	initialise_vector(my_vector, num_arg, 0);

	// Sum the vector
	int my_sum = sum_vector(my_vector, num_arg);

	// Get the time after summing the vector
    	timespec_get(&end_time, TIME_UTC);
	
	// Print the sum
	printf("Sum: %d\n", my_sum);

	// If we use malloc, must free when done!
	free(my_vector);

	// Calculate the runtime
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);

	// Save the runtime and input number to an output file
	data_file = fopen("./data/serial_timing_internal.txt", "a");
	// File format: number of iterations, run time
	fprintf(data_file, "%d, %lf \n", num_arg, runtime);
	fclose(data_file);

	return 0;
}

// Define a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
	// Create a variable to hold the sum
	int sum = 0;

	// Iterate through the vector
	for (int i = 0; i < size; i++)
	{
		// Set the elements of the vector to the initial value
		sum += vector[i];
	}

	// Return the sum
	return sum;
}

// Define a function to initialise all values in a vector
void initialise_vector(int vector[], int size, int initial)
{
	// Iterate through the vector
	for (int i = 0; i < size; i++)
	{
		// Set the elements of the vector to the (initial value + i)**2
		vector[i] = (initial + i)*(initial + i);
	}
}

// Define a function to print a vector of ints
void print_vector(int vector[], int size)
{
	// Iterate through the vector
	for (int i = 0; i < size; i++)
	{
		// Print the elements of the vector to the screen
		printf("%d\n", vector[i]);
	}
}

// Define a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
	// Declare and initialise the numerical argument
	int num_arg = 0;

	// Check the number of arguments
	if (argc == 2) // Program name and numerical argument
	{
		// Declare and initialise the numerical argument
		num_arg = atoi(argv[1]);
	}
	else // The number of arguments is incorrect
	{
		// Raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

		// And exit COMPLETELY
		exit (-1);
	}
	return num_arg;
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
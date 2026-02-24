#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Declare the functions that will be called within main
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector_p(int size, int my_rank, int uni_size);
void check_uni_size(int uni_size);
void root_task(int size, int uni_size);

// Necessary for timing
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv)
{
	// For timing 
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	FILE *data_file;
	
	// Get the time
	timespec_get(&start_time, TIME_UTC);
	
	// Error handling variable
	int ierror = 0;

	// Declare and initialise the numerical argument variable
	int num_arg = check_args(argc, argv);

	// Intialise MPI
	ierror = MPI_Init(&argc, &argv);

	// Declare and initialise rank and size
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// Get the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
	
	// Check the universe size is correct
	check_uni_size(uni_size);
	
	if (my_rank == 0)
	{
		root_task(num_arg, uni_size);
	}

	// Find the sum in parallel
	sum_vector_p(num_arg, my_rank, uni_size);

	// Finalise MPI
	ierror = MPI_Finalize();

	// If the root node finish up the timing stuff
	if (my_rank == 0)
	{
		// Get end time
		timespec_get(&end_time, TIME_UTC);

		time_diff = calculate_runtime(start_time, end_time);
		runtime = to_second_float(time_diff);

		// Save the time taken to a file
		data_file = fopen("./data/vector_send_recv_time.txt", "a");
		// File format: no. processors, input, time
		fprintf(data_file, "%d, %d, %lf\n", uni_size, num_arg, runtime);
		fclose(data_file);
	}

	return 0;
}

// Split up the vector and then send it to all processes
void root_task(int size, int uni_size)
{
	// Create a vector variable
	int* my_vector = malloc (size * sizeof(int));
	// And initialise every element
	initialise_vector(my_vector, size, 0);	

	int chunk = size / uni_size;

	// Variables for splitting up the vector
	int start, stop;

	// Create the transmission variables
	int count, tag;
	tag = 0;

	// Send a chunk to each process
	for (int dest = 0; dest < uni_size; dest++)
	{
		start = dest * chunk;

		// If this is the last process to be sent to, send elements at the end as well
		if (uni_size - 1 == dest)
		{
			stop = size;
		}
		else // Otherwise send up to the start of the next chunk
		{
			stop = (dest + 1) * chunk;
		}

		// The count is the size of the vector
		count = stop - start;
		
		// Send a vector with the start and end points
		int* send_vector = malloc(count * sizeof(int));
		for (int i = 0; i < count; i++)
		{
			send_vector[i] = my_vector[i + start];
		}
		
		MPI_Send(send_vector, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
	free(my_vector);
}

// Sum the vector in parallel
int sum_vector_p(int size, int my_rank, int uni_size)
{
	// Initialise some variables
	int start, stop;

	// Receive the sent vector

	// First find the size of the vector
	int chunk = size / uni_size;
	start = my_rank * chunk;
	// If this is the last process to be sent to, get elements at the end as well
	if (uni_size - 1 == my_rank)
	{
		stop = size;
	}
	else // Otherwise send up to the start of the next chunk
	{
		stop = (my_rank + 1) * chunk;
	}

	// Create the transmission variables
	int recv_message, count, source, tag;
	recv_message = source = tag = 0;
	count = stop - start;
	MPI_Status status;
	
	int* vector = malloc(count * sizeof(int));

	MPI_Recv(vector, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
	

	// Store the partial sum
	int partial_sum = 0;

	// Sum this specific chunk
	for (int i = 0; i < count; i++)
	{
		partial_sum += vector[i];
	}

	// If not the root, send to the root
	if (my_rank != 0)
	{
		// Create the transmission variables
		int count, dest, tag;
		dest = tag = 0;
		count = 1;
		
		MPI_Send(&partial_sum, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
	else // If the root, receive the vector and sum it
	{
		// Create the transmission variables
		int recv_message, count, source, tag;
		recv_message = source = tag = 0;
		count = 1;
		MPI_Status status;

		// The final sum, starting with what was calculated here
		int total_sum = partial_sum;

		// For all other ranks
		for (source = 1; source < uni_size; source++)
		{
			// Receive the message
			MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			// Add this to the running total
			total_sum += recv_message;
		}

		// Output and return
		printf("Sum: %d\n", total_sum);
		return total_sum;
	}
	free(vector);
}

// Define a function to initialise all values in a vector
void initialise_vector(int vector[], int size, int initial)
{
	// Iterates through the vector
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
	if (argc == 2) // program name and numerical argument
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

// Ensure the universe size is ok
void check_uni_size(int uni_size)
{
	// Set the minimum universe size
	int min_uni_size = 1;
	// Check there are sufficient tasks to communicate with
	if (uni_size >= min_uni_size)
	{
		return;
	}
	else
	{
		// Raise an error
		fprintf(stderr, "Unable to communicate with fewer than %d processes.", min_uni_size);
		fprintf(stderr, "MPI communicator size = %d\n", uni_size);

		// And exit COMPLETELY
		exit(-1);
	}
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

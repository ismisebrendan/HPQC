#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Necessary for timing
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

void check_uni_size(int uni_size), root_task(int uni_size, int my_rank), client_task(int uni_size, int my_rank), check_task(int uni_size, int my_rank);

int main(int argc, char **argv) 
{	
	// Declare and initialise error handling variable
	int ierror = 0;
	
	// Declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// Intitalise MPI
	ierror = MPI_Init(&argc, &argv);

	// Get the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);

	// Check the number of processors
	check_uni_size(uni_size);

	// Check which task to do and do it
	check_task(uni_size, my_rank);

	// Finalise MPI
	ierror = MPI_Finalize();
	return 0;
}

void check_task(int uni_size, int my_rank)
{
	if (0 == my_rank)
	{
		root_task(uni_size, my_rank);
	}
	else
	{
		client_task(uni_size, my_rank);
	}
}

void client_task(int uni_size, int my_rank)
{
	// For timing 
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	FILE *data_file;

	// Get the time
	timespec_get(&start_time, TIME_UTC);
	
	// Create and initialise transmission variables
	int send_message, count, dest,  tag;
	send_message = dest = tag = 0;
	count = 1;

	// Create message
	send_message = my_rank * 10;

	// Send the message
	MPI_Ssend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

	// Print the message
	printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);

	// Get end time
	timespec_get(&end_time, TIME_UTC);

	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);

	printf("%d took %f s to send\n", my_rank, runtime);

	// Save the time taken to a file
	data_file = fopen("./data/Ssend_send_time.txt", "a");
	// File format: no. processors, rank, time to send
	fprintf(data_file, "%d, %d, %lf\n", uni_size, my_rank, runtime);
	fclose(data_file);
}

void root_task(int uni_size, int my_rank)
{	
	// For timing 
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	FILE *data_file;
	
	// Create and initialise transmission variables
	int recv_message, count, source, tag;
	recv_message = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	// iterate through other ranks
	for (int their_rank = 1; their_rank < uni_size; their_rank++)
	{
		// Get the time
		timespec_get(&start_time, TIME_UTC);
		
		// Set source to sender rank
		source = their_rank;

		// Receive the message
		MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

		// Print the message from the sender
		printf("Hello, I am %d of %d. Received %d from Rank %d\n", my_rank, uni_size, recv_message, source);
	
		// Get end time
		timespec_get(&end_time, TIME_UTC);

		time_diff = calculate_runtime(start_time, end_time);
		runtime = to_second_float(time_diff);

		printf("%d took %f s to receive\n", my_rank, runtime);
		// Save the time taken to a file
		data_file = fopen("./data/Ssend_revc_time.txt", "a");
		// File format: no. processors, rank from, time to receive
		fprintf(data_file, "%d, %d, %lf\n", uni_size, their_rank, runtime);
		fclose(data_file);
	}
}

void check_uni_size(int uni_size)
{
	// Minimum size
	int min_uni_size = 2;
	// Check we have the minimum size
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
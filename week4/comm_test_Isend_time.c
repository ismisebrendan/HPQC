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
	// declare and initialise error handling variable
	int ierror = 0;
	
	// declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);

	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);

	// check the number of processors
	check_uni_size(uni_size);

	// check which task to do and do it
	check_task(uni_size, my_rank);

	// finalise MPI
	ierror = MPI_Finalize();
	return 0;
}

void check_task(int uni_size, int my_rank)
{
	if (0 == my_rank)
	{
		root_task(uni_size, my_rank);
	} // end if (0 == my_rank)
	else // i.e. (0 != my_rank)
	{
		client_task(uni_size, my_rank);
	} // end else // i.e. (0 != my_rank)
}

void client_task(int uni_size, int my_rank)
{
	// for timing 
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	
	// get the time
	timespec_get(&start_time, TIME_UTC);
	
	// creates and initialies transmission variables
	int send_message, count, dest,  tag;
	send_message = dest = tag = 0;
	count = 1;
	MPI_Request requestHandle;

	// create message
	send_message = my_rank * 10;

	// send the message
	MPI_Isend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD, &requestHandle);

	// print the message
	printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);

	// get end time
	timespec_get(&end_time, TIME_UTC);

	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);

	printf("%d took %f s to send\n", my_rank, runtime);
}

void root_task(int uni_size, int my_rank)
{	
	// for timing 
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	
	// creates and initialies transmission variables
	int recv_message, count, source, tag;
	recv_message = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	// iterate through other ranks
	for (int their_rank = 1; their_rank < uni_size; their_rank++)
	{
		// get the time
		timespec_get(&start_time, TIME_UTC);
		
		// set source to sender rank
		source = their_rank;

		// receive the message
		MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

		// prints the message from the sender
		printf("Hello, I am %d of %d. Received %d from Rank %d\n", my_rank, uni_size, recv_message, source);
	
		// get end time
		timespec_get(&end_time, TIME_UTC);

		time_diff = calculate_runtime(start_time, end_time);
		runtime = to_second_float(time_diff);

		printf("%d took %f s to receive\n", my_rank, runtime);
	}
}

void check_uni_size(int uni_size)
{
	// minimum size
	int min_uni_size = 2;
	// check we have the minimum size
	if (uni_size >= min_uni_size)
	{
		return;
	}
	else
	{
		// Raise an error
                fprintf(stderr, "Unable to communicate with fewer than %d processes.", min_uni_size);
                fprintf(stderr, "MPI communicator size = %d\n", uni_size);

                // and exit COMPLETELY
                exit(-1);
	}
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
	long int seconds, nanoseconds;
	seconds = nanoseconds = 0;
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

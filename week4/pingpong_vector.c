#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Necessary for timing
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int check_args(int argc, char **argv);
void check_uni_size(int uni_size), ping(), pong(), ping_pong(int my_rank);
void print_vector(int vector[], int size);
int counter;
void initialise_vector(int vector[], int size, int initial);
int* vector;

int main (int argc, char **argv)
{
	// for timing 
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	double average_time = 0.0;
	FILE *data_file;

	// initialise the counter
	counter = 0;

	// declare and initialise the numerical argument variable
	int num_pings = check_args(argc, argv);
	
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

	// creates a vector variable
	// int my_vector[num_arg]; // suffers issues for large vectors
	vector = malloc (num_pings * sizeof(int));
	// and initialises every element
	initialise_vector(vector, num_pings, 0);

	// get the time
	timespec_get(&start_time, TIME_UTC);

	while (vector[0] < num_pings)
	{
		// ping and pong
		ping_pong(my_rank);
	}
	
	// finalise MPI
	ierror = MPI_Finalize();

	timespec_get(&end_time, TIME_UTC);

	// only do the timing stuff if root node
	if (my_rank == 0)
	{
		time_diff = calculate_runtime(start_time, end_time);
		runtime = to_second_float(time_diff);

		average_time = runtime / vector[0];
		
		// print to console
		printf("Size of vector: %ld\nTotal time: %f\nAverage time: %f\n", vector[0] * sizeof(int), runtime, average_time);
	
		// also print to a file
		data_file = fopen("./data/pingpong_vector.txt", "a");
		// File format: size, total time, average time
		fprintf(data_file, "%ld, %lf, %lf \n", vector[0] * sizeof(int), runtime, average_time);
		fclose(data_file);
	}

	free(vector);

	return 0;
}

// defines a function to initialise all values in a vector
void initialise_vector(int vector[], int size, int initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the (initial value + i)**2
		vector[i] = 0;
	}
}

// check whether root or not, then do required task
void ping_pong(int my_rank)
{
	if (0 == my_rank)
	{
		ping();
	}
	else
	{
		pong();
	}
}

// client receives and then sends
void pong()
{
	// create and initialise transmission variables
	int send_message, recv_message, count, dest, source, tag;
	recv_message = dest = source = tag = 0;
	count = 1;
	MPI_Status status;

	// Wait to receive the counter
	MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

	// set the first element to the received message
	//counter = recv_message;
	vector[0] = recv_message;

	// increment the counter and send it back
	vector[0] = vector[0] + 1;
	send_message = vector[0];
	MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

}

// root sends and then receives
void ping()
{
	// create and initialise transmission variables
	int send_message, recv_message, count, dest, source, tag;
	recv_message = tag = 0;
	count = 1;
	dest = source = 1;
	MPI_Status status;

	// send the first element of the array
	send_message = vector[0];

	// send the message
	MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

	// Wait to receive the counter back
	MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
	vector[0] = recv_message;
}

int check_args(int argc, char **argv)
{
	// declare and initialise the numerical argument
	int num_arg = 0;

	// check the number of arguments
	if (argc == 2) // program name and numerical argument
	{
		// declare and initialise the numerical argument
		num_arg = atoi(argv[1]);
	}
	else // the number of arguments is incorrect
	{
		// raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

		// and exit COMPLETELY
		exit (-1);
	}
	return num_arg;
}

void check_uni_size(int uni_size)
{
	// required size
	int req_uni_size = 2;
	// check we have the minimum size
	if (uni_size == req_uni_size)
	{
		return;
	}
	else
	{
		// Raise an error
                fprintf(stderr, "Must have %d processors", req_uni_size);
                fprintf(stderr, "MPI communicator size = %d\n", uni_size);

                // and exit COMPLETELY
                exit(-1);
	}
}

// for timing
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

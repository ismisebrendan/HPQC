#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

// Declare the functions that will be called within main
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double first_pos);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);
void check_uni_size(int uni_size);
void root_task(FILE** out_file, int uni_size, int points, double* time_stamps, int time_steps);
void client_task(int my_rank, int uni_size, int points, double* time_stamps, int time_steps);

// Necessary for timing
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

// Struct for inputs
struct Input
{
	int points;
	int cycles;
	int samples;
	char* out;
};

// Define a function that checks the arguments to make sure they'll do what you need and return as Input
struct Input check_args(int argc, char **argv)
{
	struct Input i;

	// Check the number of arguments
	if (argc == 4 || argc == 5) // Program name and 3 or 4 arguments (3 numerical, optionally 1 char): points, cycles, samples, out
	{
		i.points = atoi(argv[1]);
		i.cycles = atoi(argv[2]);
		i.samples = atoi(argv[3]);
		if (argc == 5)
		{
			i.out = argv[4];
		}
		else
			i.out = "data/string_wave_parallel.csv";
	}
	else // The number of arguments is incorrect
	{
		// Raise an error
		fprintf(stderr, "ERROR: You did not provide the correct arguments!\n");
		fprintf(stderr, "Correct use: %s [POINTS] [CYCLES] [SAMPLES] (optional [OUTPUT FILE PATH])\n", argv[0]);
		fprintf(stderr, "The output should be given as a full path, including file extension.\n");
		fprintf(stderr, "The output defaults to 'data/string_wave.csv' if no input is provided.\n");

		// Exit COMPLETELY
		exit (-1);
	}
	
	return i;
}

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

	// Declare and initialise the numerical argument variables
	struct Input in = check_args(argc, argv);
	
	int points, cycles, samples;
	char* out;
	points = in.points;
       	cycles = in.cycles;
       	samples = in.samples;
	out = in.out;

	int time_steps = cycles * samples + 1; // total timesteps
	double step_size = 1.0/samples;

	// Create a vector for the time stamps in the data, all nodes need this
	double* time_stamps = (double*) malloc(time_steps * sizeof(double));
	initialise_vector(time_stamps, time_steps, 0.0);
	generate_timestamps(time_stamps, time_steps, step_size);

	// Create a file if root node
	FILE* out_file;
	if (my_rank == 0)
	{
     		out_file = fopen(out, "w");
		print_header(&out_file, points);
	}

	// Carry out the appropriate task
	if (my_rank == 0)
	{
		root_task(&out_file, uni_size, points, time_stamps, time_steps);
	}
	else
	{
		client_task(my_rank, uni_size, points, time_stamps, time_steps);
	}

	// Finalise MPI
	ierror = MPI_Finalize();
	
	// if we use malloc, must free when done!
	free(time_stamps);

	// Close the file if root node
	if (my_rank == 0)
	{
		fclose(out_file);
	}

	// If the root node finish up the timing stuff
	if (my_rank == 0)
	{
		// Get end time
		timespec_get(&end_time, TIME_UTC);

		time_diff = calculate_runtime(start_time, end_time);
		runtime = to_second_float(time_diff);

		// Save the time taken to a file
		data_file = fopen("./data/string_parallel_time.txt", "a");
		// File format: no. processors, points, cycles, samples, time
		fprintf(data_file, "%d, %d, %d, %d, %lf\n", uni_size, points, cycles, samples, runtime);
		fclose(data_file);
	}
	return 0;
}

// prints a header to the file
// double-pointer used to allow this function to move the file pointer
void print_header(FILE** p_out_file, int points)
{
	fprintf(*p_out_file, "#, time");
	for (int j = 0; j < points; j++)
	{
		fprintf(*p_out_file, ", y[%d]", j);
	}
	fprintf(*p_out_file, "\n");
}

void root_task(FILE** out_file, int uni_size, int points, double* time_stamps, int time_steps)
{
	// Create a vector variable for the current positions, only the root needs this
	double* positions = (double*) malloc(points * sizeof(double));

	// And initialise every element to zero
	initialise_vector(positions, points, 0.0);
	
	// Send the chunks to each node
	
	// Transmission variables
	int count, tag;
	tag = 0;

	// Send a chunk to each other node
	
	// Find how big each chunk is
	int chunk = points / (uni_size - 1);
	int start, stop;
	
	for (int dest = 1; dest < uni_size; dest++)
	{
		start = (dest - 1) * chunk;

		// If this is the last process to be sent to, send elements at the end as well
		if (uni_size - 1 == dest)
		{
			stop = points;
		}
		else // Otherwise send up to the start of the next chunk
		{
			stop = dest * chunk;
		}

		// The count is the size of the vector
		count = stop - start;
		
		// Send a vector with the start and end points
		int* send_positions = malloc(count * sizeof(int));
		for (int i = 0; i < count; i++)
		{
			send_positions[i] = positions[i + start];
		}
		
		MPI_Send(send_positions, count, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
		free(send_positions);
	}

	// Transmission variables for receiving the chunks
	double* recv_points;
	int tag_chunk = 2; // Tag to receive chunks from other nodes
	MPI_Status status;

	// Iterate through each time step in the collection
	for (int i = 0; i < time_steps; i++)
	{
		// Print an index and time stamp
		fprintf(*out_file, "%d, %lf", i, time_stamps[i]);


		// Receive the chunks from the other nodes and update positions with them
		for (int source = 1; source < uni_size; source++)
		{
			start = (source - 1) * chunk;
			
			if (uni_size - 1 == source)
			{
				stop = points;
			}
			else
			{
				stop = source * chunk;
			}
			// The count is the size of the vector
			count = stop - start;
		
			recv_points = malloc(count * sizeof(double));

			// Receive the message
			MPI_Recv(recv_points, count, MPI_DOUBLE, source, tag_chunk, MPI_COMM_WORLD, &status);


			// Now update the positions vector
			for (int j = start; j < stop; j++)
		        {
		                positions[j] = recv_points[j - start];
		        }
		}	
	
		// Iterate over all of the points on the line
		for (int j = 0; j < points; j++)
		{
			// Print each y-position to a file
			fprintf(*out_file, ", %lf", positions[j]);
		}
		// Print a new line
		fprintf(*out_file, "\n");
	}

	free(recv_points);
	free(positions);
}

void client_task(int my_rank, int uni_size, int points, double* time_stamps, int time_steps)
{
	// Receive the part of the string relevant to this node
	
	// Find how big each chunk is
	int chunk = points / (uni_size - 1);

	int start, stop;
	start = (my_rank - 1) * chunk;
	
	// If this is the last process to receive, get elements at the end as well
	if (uni_size - 1 == my_rank)
	{
		stop = points;
	}
	else // Otherwise receive up to the start of the next chunk
	{
		stop = my_rank * chunk;
	}

	// Create the transmission variables
	double recv_points;
	int source, tag;
	recv_points = source = tag = 0;
	chunk = stop - start;
	MPI_Status status;
	
	double* my_positions = malloc(chunk * sizeof(double));
	
	MPI_Recv(my_positions, chunk, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);

	// Transmission variables for the loop
	double last_pos; // Position of the last point on this chunk of the string, to be sent
	double first_pos; // First position, either found from the driver, or received from previous node
	int count = 1;
	int dest = my_rank + 1; // Send to next node
	source = my_rank - 1; // Receive from previous node
	int tag_points = 1; // Tag for messages about the last point on each chunk
	int tag_root = 2; // Tag for sending chunks to root
	
	// Iterate through each time step in the collection
	for (int i = 0; i < time_steps; i++)
	{
		// Assign the last position
		last_pos = my_positions[chunk - 1];

		// First get the first value
	
		// If node 1 run the driver
		if (my_rank == 1)
		{
			// Driver
			first_pos = driver(time_stamps[i]);
		}
		else // If not node 1 get message from previous node
		{
			MPI_Recv(&first_pos, count, MPI_DOUBLE, source, tag_points, MPI_COMM_WORLD, &status);
		}
		
		// Unless last node send initial value of final point to next node
		if (my_rank != uni_size - 1)
		{
			MPI_Send(&last_pos, count, MPI_DOUBLE, dest, tag_points, MPI_COMM_WORLD);
		}	
		// Now update the string
		
		// Update positions
		update_positions(my_positions, chunk, first_pos);		
		// Send all values to root	
		MPI_Send(my_positions, chunk, MPI_DOUBLE, 0, tag_root, MPI_COMM_WORLD);
	}

	free(my_positions);

}

// defines a simple harmonic oscillator as the driving force
double driver(double time)
{
	double value = sin(time*2.0*M_PI);
	return(value);
}

// defines a function to update the positions
void update_positions(double* positions, int points, double first_pos)
{
	// creates a temporary vector variable for the new positions
        double* new_positions = (double*) malloc(points * sizeof(double));

	// initialises the index
	int i = 0;
	new_positions[i] = first_pos;
	// creates new positions by setting value of previous element 
	for (i = 1; i < points; i++)
	{
		new_positions[i] = positions[i-1];
	}
	// propagates these new positions to the old ones
	for (i = 0; i < points; i++)
        {
                positions[i] = new_positions[i];
        }

	// frees the temporary vector
	free(new_positions);
}

// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
	for (int i = 0; i < time_steps ; i++)
	{
		timestamps[i]=i*step_size;
	}	
	return time_steps;
}

// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
	// creates a variable to hold the sum
	int sum = 0;

	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		sum += vector[i];
	}

	// returns the sum
	return sum;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(double vector[], int size, double initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = initial;
	}
}

// defines a function to print a vector of ints
void print_vector(double vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// prints the elements of the vector to the screen
		printf("%d, %lf\n", i, vector[i]);
	}
}

// Ensure the universe size is ok
void check_uni_size(int uni_size)
{
	// Set the minimum universe size
	int min_uni_size = 2;
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

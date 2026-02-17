#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Declares the functions that will be called within main
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector_p(int vector[], int size, int my_rank, int uni_size);
void check_uni_size(int uni_size);

int main(int argc, char **argv)
{
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

	// Variables for broadcasting
	int count;
	count = num_arg;

	// Initialise the vector variable
	int* my_vector = malloc(num_arg * sizeof(int));
	
	// Create a vector variable if the root node
	if (my_rank == 0)
	{
		my_vector = malloc(num_arg * sizeof(int));
		initialise_vector(my_vector, num_arg, 0);
	}

	MPI_Bcast(my_vector, count, MPI_INT, 0, MPI_COMM_WORLD);
	
	// Find the sum in parallel
	sum_vector_p(my_vector, num_arg, my_rank, uni_size);

	// Finalise MPI
	ierror = MPI_Finalize();

	// Free when done
	free(my_vector);

	return 0;
} 

// Sum the vector in parallel
int sum_vector_p(int vector[], int size, int my_rank, int uni_size)
{

	// Store the partial sum
	int partial_sum = 0;

	// Initialise stop
	int stop;

	// Break up the vector
	int chunk = size / uni_size;

	int start = my_rank * chunk;

	// If this is the last process take all elements at the end as well
	if (uni_size - 1 == my_rank)
	{
		stop = size;
	}
	else // otherwise take up to the start of the next chunk
	{
		stop = (my_rank + 1) * chunk;
	}

	// Sum this specific chunk
	for (int i = start; i < stop; i++)
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
	else // If the root, receive the vector sums and sum them
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

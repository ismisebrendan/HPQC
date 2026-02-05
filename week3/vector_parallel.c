#include <stdio.h>
#include <stdlib.h>

// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector(int vector[], int size);

int main(int argc, char **argv)
{
	// error handling variable
	int ierror = 0;

	// declare and initialise the numerical argument variable
	int num_arg = check_args(argc, argv);

	// intialise MPI
	ierror = MPI_Init(&argc, &argv);

	// declare and initialise rank and size
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
	
	// checks the universe size is correct
	check_uni_size(uni_size);
	
	// creates a vector variable
	// int my_vector[num_arg]; // suffers issues for large vectors
	int* my_vector = malloc (num_arg * sizeof(int));
	// and initialises every element
	initialise_vector(my_vector, num_arg, 0);

	// find the sum in parallel
	sum_vector_p(my_vector, my_rank, uni_size);

	// finalise MPI
	ierror = MPI_Finalize();

	// if we use malloc, must free when done!
	free(my_vector);

	return 0;
}

// sum the vector in parallel
int sum_vector_p(int vector[], int my_rank, int uni_size)
{
	// store the partial sum
	int partial_sum = 0;
	// break up the vector
	int chunk = sizeof(vector) / uni_size;

	int start = my_rank * chunk;

	// if this is the last process take all elements at the end as well
	if (uni_size - 1 == my_rank)
	{
		int stop = sizeof(vector);
	}
	else // otherwise take up to the start of the next chunk
	{
		int stop = (my_rank + 1) * chunk;
	}

	// sum this specific chunk
	for (int i = start; i < stop; i++)
	{
		partial_sum += vector[i];
	}

	// if not the root, send to the root
	if (my_rank != 0)
	{
		// create the transmission variables
		int count, dest, tag;
		dest = tag = 0;
		count = 1;
		
		MPI_Send(&partial_sum, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
	else // if the root, receive the vector and sum it
	{
		// create the transmission variables
		int recv_message, count, source, tag;
		recv_message = source = tag = 0;
		count = 1;
		MPI_Status status

		// the final sum, starting with what was calculated here
		int total_sum = partial_sum;

		// for all other ranks
		for (source = 1; source < uni_size; source++)
		{
			// receive the message
			MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			// add this to the running total
			total_sum += recv_message
		}

		// output and return
		printf("Sum: %d\n", total_sum);
		return total_sum
	}
}

// defines a function to initialise all values in a vector
void initialise_vector(int vector[], int size, int initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the (initial value + i)**2
		vector[i] = (initial + i)*(initial + i);
	}
}

// defines a function to print a vector of ints
void print_vector(int vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// prints the elements of the vector to the screen
		printf("%d\n", vector[i]);
	}
}

// defines a function that checks your arguments to make sure they'll do what you need
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
	// sets the minimum universe size
	int min_uni_size = 1;
	// checks there are sufficient tasks to communicate with
	if (uni_size >= min_uni_size)
	{
		return;
	} // end if (uni_size >= min_uni_size)
	else // i.e. uni_size < min_uni_size
	{
		// Raise an error
		fprintf(stderr, "Unable to communicate with fewer than %d processes.", min_uni_size);
		fprintf(stderr, "MPI communicator size = %d\n", uni_size);

		// and exit COMPLETELY
		exit(-1);
	}
}

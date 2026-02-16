#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
	// Create and initialise transmission variables
	int send_message, count, dest,  tag;
	send_message = dest = tag = 0;
	count = 1;

	// Create message
	send_message = my_rank * 10;

	// Send the message
	MPI_Bsend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

	// Print the message
	printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
}

void root_task(int uni_size, int my_rank)
{
	// Create and initialise transmission variables
	int recv_message, count, source, tag;
	recv_message = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	// Iterate through other ranks
	for (int their_rank = 1; their_rank < uni_size; their_rank++)
	{
		// Set source to sender rank
		source = their_rank;

		// Receive the message
		MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

		// Print the message from the sender
		printf("Hello, I am %d of %d. Received %d from Rank %d\n", my_rank, uni_size, recv_message, source);
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
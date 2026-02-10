#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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

	// creates and initialies transmission variables
	int send_message, recv_message, count, dest, source, tag;
	send_message = recv_message = dest = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	check_uni_size(uni_size);

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
	// creates and initialies transmission variables
	int send_message, recv_message, count, dest, source, tag;
	send_message = recv_message = dest = source = tag = 0;
	count = 1;
	MPI_Status status;

	// create message
	send_message = my_rank * 10;

	// send the message
	MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

	// print the message
        printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
}

void root_task(int uni_size, int my_rank)
{
	// creates and initialies transmission variables
	int send_message, recv_message, count, dest, source, tag;
	send_message = recv_message = dest = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	// iterate through other ranks
	for (int their_rank = 1; their_rank < uni_size; their_rank++)
	{
		// set source to sender rank
		source = their_rank;

		// receive the message
		MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

		// prints the message from the sender
		printf("Hello, I am %d of %d. Received %d from Rank %d\n", my_rank, uni_size, recv_message, source);
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



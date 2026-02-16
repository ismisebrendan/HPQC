#include <stdio.h>
#include <mpi.h>

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

	// Create and initialise transmission variables
	int send_message, recv_message, count, dest, source, tag;
	send_message = recv_message = dest = source = tag = 0;
	count = 1;
	MPI_Status status;
	
	if (uni_size > 1)
	{
		if (0 == my_rank)
		{
			// Iterate through all the other ranks
			for (int their_rank = 1; their_rank < uni_size; their_rank++)
			{
				// Set the source argument to the rank of the sender
				source = their_rank;

				// Receive the messages
				MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

				// Print the message from the sender
				printf("Hello, I am %d of %d. Received %d from Rank %d\n",
						my_rank, uni_size, recv_message, source);
			} // end for (int their_rank = 1; their_rank < uni_size; their_rank++)
		} // end if (0 == my_rank)
		else // i.e. (0 != my_rank)
		{
			// Set the destination for the message
			dest = 0; // Destination is root

			// Create the message
			send_message = my_rank * 10;

			// Send the message
			MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

			// Print the message from the sender
                        printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
                                         my_rank, uni_size, send_message, dest);

		} // end else // i.e. (0 != my_rank)
	} // end if (uni_size > 1)
	else // i.e. uni_size <=1
	{
		// Print a warning
		printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
	}

	// Finalise MPI
	ierror = MPI_Finalize();
	return 0;
}
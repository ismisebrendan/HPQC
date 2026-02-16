#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	int myRank, uniSize, ierror;

	// Set up MPI
	ierror = MPI_Init(&argc, &argv);

	// Get rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);

	// Print to screen
	printf("Hello world, I am process %d of %d\n", myRank, uniSize);

	// Close MPI connections
	ierror = MPI_Finalize();
	return 0;
}

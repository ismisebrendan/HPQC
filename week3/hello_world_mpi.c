#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	int myRank, uniSize, ierror;

	// set up MPI
	ierror = MPI_Init(&argc, &argv);

	// get rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);

	// print to screen
	printf("Hello world, I am process %d of %d\n", myRank, uniSize);

	// close MPI connections
	ierror = MPI_Finalize();
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

void initialise_vector(int vector[], int size, int initial);
int check_args(int argc, char **argv);
void print_vector(int vector[], int size);

int main (int argc, char **argv)
{
	// Declare and initialise numerical argument variable
	int num_arg = check_args(argc, argv);
	
	// Creates a vector variable
	int* my_vector = malloc (num_arg * sizeof(int));
	// And initialise every element
	initialise_vector(my_vector, num_arg, 0);

	// Print the size of the vector
	printf("Size of vector elements: %ld\n", sizeof(*my_vector));	

	free(my_vector);

	return 0;
}

// Define a function to initialise all values in a vector
void initialise_vector(int vector[], int size, int initial)
{
	// Iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// Set the elements of the vector to the initial value
		vector[i] = initial + i;
	}
}

// Define a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
	// Declare and initialise the numerical argument
	int num_arg = 0;

	// Check the number of arguments
	if (argc == 2) // Program name and numerical argument
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
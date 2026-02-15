#include <stdio.h>
#include <stdlib.h>

void initialise_vector(int vector[], int size, int initial);
int check_args(int argc, char **argv);
void print_vector(int vector[], int size);

int main (int argc, char **argv)
{
	// declare and initialise numerical argument variable
	int num_arg = check_args(argc, argv);
	
	// creates a vector variable
	// int my_vector[num_arg]; // suffers issues for large vectors
	int* my_vector = malloc (num_arg * sizeof(int));
	// and initialises every element
	initialise_vector(my_vector, num_arg, 0);

	// print the size of the vector
	printf("Size of vector elements: %ld\n", sizeof(*my_vector));	

	free(my_vector);

	return 0;
}

// defines a function to initialise all values in a vector
void initialise_vector(int vector[], int size, int initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = initial + i;
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


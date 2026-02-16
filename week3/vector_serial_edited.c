#include <stdio.h>
#include <stdlib.h>

// Declares the functions that will be called within main
// Note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector(int vector[], int size);

int main(int argc, char **argv)
{
	// Declare and initialise the numerical argument variable
	int num_arg = check_args(argc, argv);

	// Creates a vector variable
	int* my_vector = malloc (num_arg * sizeof(int));
	// And initialise every element
	initialise_vector(my_vector, num_arg, 0);

	// Sums the vector
	int my_sum = sum_vector(my_vector, num_arg);

	// Prints the sum
	printf("Sum: %d\n", my_sum);

	// If we use malloc, must free when done!
	free(my_vector);

	return 0;
}

// Defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
	// Creates a variable to hold the sum
	int sum = 0;

	// Iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// Sets the elements of the vector to the initial value
		sum += vector[i];
	}

	// Returns the sum
	return sum;
}

// Define a function to initialise all values in a vector
void initialise_vector(int vector[], int size, int initial)
{
	// Iterate through the vector
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
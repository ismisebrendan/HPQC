#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Declare the functions that will be called within main
// Note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);

// Struct for inputs
struct Input
{
	int points;
	int cycles;
	int samples;
	char* out;
};

// Define a function that checks the arguments to make sure they'll do what you need and return as Input
struct Input check_args(int argc, char **argv)
{
	struct Input i;

	// Check the number of arguments
	if (argc == 4 || argc == 5) // Program name and 3 or 4 arguments (3 numerical, optionally 1 char): points, cycles, samples, out
	{
		i.points = atoi(argv[1]);
		i.cycles = atoi(argv[2]);
		i.samples = atoi(argv[3]);
		if (argc == 5)
		{
			i.out = argv[4];
		}
		else
			i.out = "data/string_wave.csv";
	}
	else // The number of arguments is incorrect
	{
		// Raise an error
		fprintf(stderr, "ERROR: You did not provide the correct arguments!\n");
		fprintf(stderr, "Correct use: %s [POINTS] [CYCLES] [SAMPLES] (optional [OUTPUT FILE PATH])\n", argv[0]);
		fprintf(stderr, "The output should be given as a full path, including file extension.\n");
		fprintf(stderr, "The output defaults to 'data/string_wave.csv' if no input is provided.\n");

		// Exit COMPLETELY
		exit (-1);
	}
	
	return i;
}

int main(int argc, char **argv)
{
	// Declare and initialise the numerical argument variables
	struct Input in = check_args(argc, argv);
	
	int points, cycles, samples;
	char* out;
	points = in.points;
       	cycles = in.cycles;
       	samples = in.samples;
	out = in.out;

	int time_steps = cycles * samples + 1; // total timesteps
	double step_size = 1.0/samples;

	// creates a vector for the time stamps in the data
	double* time_stamps = (double*) malloc(time_steps * sizeof(double));
	initialise_vector(time_stamps, time_steps, 0.0);
	generate_timestamps(time_stamps, time_steps, step_size);

	// creates a vector variable for the current positions
	double* positions = (double*) malloc(points * sizeof(double));
	// and initialises every element to zero
	initialise_vector(positions, points, 0.0);

	// creates a file
	FILE* out_file;
     	out_file = fopen(out, "w");
	print_header(&out_file, points);

	// iterates through each time step in the collection
	for (int i = 0; i < time_steps; i++)
	{
		// updates the position using a function
		update_positions(positions, points, time_stamps[i]);

		// prints an index and time stamp
		fprintf(out_file, "%d, %lf", i, time_stamps[i]);

		// iterates over all of the points on the line
		for (int j = 0; j < points; j++)
		{
			// prints each y-position to a file
			fprintf(out_file, ", %lf", positions[j]);
		}
		// prints a new line
		fprintf(out_file, "\n");
	}

	// if we use malloc, must free when done!
	free(time_stamps);
	free(positions);

	// closes the file
	fclose(out_file);

	return 0;
}

// prints a header to the file
// double-pointer used to allow this function to move the file pointer
void print_header(FILE** p_out_file, int points)
{
	fprintf(*p_out_file, "#, time");
	for (int j = 0; j < points; j++)
	{
		fprintf(*p_out_file, ", y[%d]", j);
	}
	fprintf(*p_out_file, "\n");
}

// defines a simple harmonic oscillator as the driving force
double driver(double time)
{
	double value = sin(time*2.0*M_PI);
	return(value);
}

// defines a function to update the positions
void update_positions(double* positions, int points, double time)
{
	// creates a temporary vector variable for the new positions
        double* new_positions = (double*) malloc(points * sizeof(double));

	// initialises the index
	int i = 0;
	new_positions[i] = driver(time);
	// creates new positions by setting value of previous element 
	for (i = 1; i < points; i++)
	{
		new_positions[i] = positions[i-1];
	}
	// propagates these new positions to the old ones
	for (i = 0; i < points; i++)
        {
                positions[i] = new_positions[i];
        }

	// frees the temporary vector
	free(new_positions);
}

// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
	for (int i = 0; i < time_steps ; i++)
	{
		timestamps[i]=i*step_size;
	}	
	return time_steps;
}

// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
	// creates a variable to hold the sum
	int sum = 0;

	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		sum += vector[i];
	}

	// returns the sum
	return sum;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(double vector[], int size, double initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = initial;
	}
}

// defines a function to print a vector of ints
void print_vector(double vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// prints the elements of the vector to the screen
		printf("%d, %lf\n", i, vector[i]);
	}
}

//// Define a function that checks the arguments to make sure they'll do what you need
//int check_args(int argc, char **argv)
//{
//	int points, cycles, samples;
//
//	// Check the number of arguments
//	if (argc == 4) // Program name and 3 numerical arguments
//	{
//		points = atoi(argv[1]);
//		cycles = atoi(argv[2]);
//		samples = atoi(argv[3]);
//	}
//	else // The number of arguments is incorrect
//	{
//		// Raise an error
//		fprintf(stderr, "ERROR: You did not provide the correct numerical arguments!\n");
//		fprintf(stderr, "Correct use: %s [POINTS] [CYCLES] [SAMPLES]\n", argv[0]);
//
//		// Exit COMPLETELY
//		exit (-1);
//	}
//	
//	printf("%d, %d, %d \n", points, cycles, samples);
//	return points, cycles, samples;
//}

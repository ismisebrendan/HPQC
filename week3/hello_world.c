#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int i, input;
	i = input = 0;

	// check if right number of arguments
	if (argc == 2)
	{
		// convert to a number
		input = atoi(argv[1]);
	}
	else
	{
		fprintf(stderr, "Incorrect arguments. Usage: hello_world [NUM]\ne.g. \n hello_world 3\n");
		exit(-1);
	}

	// print to screen input number of times
	for (i=0; i<input; i++)
	{
		printf("Hello world, I am line %d of %d\n", i, input);

	}

	return 0;
}

import sys
import time

def main(): 
    output = 0
    # Check if there are the right number of arguments
    try:
        # Convert the first two arguments to integers
        in_arg = int(sys.argv[1])
    except:
        # Raise an error
        raise Exception("Incorrect arguments.\nUsage: python time_print.py [NUM]\ne.g.\npython time_print.py 23")

    # Get the start time for the loop
    start_time = time.time()
    # Iterate over all numbers up to the input
    for i in range(in_arg):
        # Print the index
        print("{}, ".format(i), end="")
    # Get the end time for the loop
    end_time = time.time()

    # Get the total time
    run_time = end_time - start_time
    # Print the output
    print("\n\nTime for loop: {} seconds\n".format(run_time))

    # Save the input number and run time to a file for analysis of data
    f = open("./data/python_time_print_results.txt", "a")
    # File format: number of iterations, run time
    f.write(f"{in_arg}, {run_time}\n")
    f.close()

if __name__ == "__main__":
    main()

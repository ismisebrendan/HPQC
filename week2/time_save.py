import sys
import time

def main(): 
    output = 0
    # Check if there are the right number of arguments
    try:
        # Convert the first two arguments to integers
        in_arg = int(sys.argv[1])
    except:
        # Raises an error
        raise Exception("Incorrect arguments.\nUsage: python time_print.py [NUM]\ne.g.\npython time_print.py 23")

    # Get the start time for the loop
    start_time = time.time()
    
    # Open or create a new file
    f = open("./data/python_time_out.txt", "w")

    # Iterate over all numbers up to the input
    for i in range(in_arg):
        # Write the index to the file
        f.write(f"{i}, ")

    # Close the file
    f.close()

    # Get the end time for the loop and saving the file
    end_time = time.time()

    # Get the total time, including saving the file
    run_time = end_time - start_time
    
    # Print the output
    print("\n\nTime for loop and to save file: {} seconds\n".format(run_time))
 
    # Save the input number and run time to a file for analysis of data
    f = open("./data/python_time_save_results.txt", "a")
    # File format: number of iterations, run time
    f.write(f"{in_arg}, {run_time}\n")
    f.close()

if __name__ == "__main__":
    main()
import sys
import time
import os # New library but only looked at outside of the scope that is being timed so shouldn't make a difference

def main(): 
    #output = 0

    # gets the start time
    start_time = time.time()
    
    # open an existing file and read it
    f = open("./data/python_time_out.txt", "r")

    # close the file
    f.close()

    # gets the end time for reading the file
    end_time = time.time()

    # get the total time
    run_time = end_time - start_time

    # print the output
    print("\n\nTime to read file: {} seconds\n".format(run_time))

    # Get the size of the file read in
g   file_size = os.path.getsize("./data/python_time_out.txt")

    # Save the input number and run time to a file for analysis of data
    f = open("./data/python_time_read_results.txt", "a")
    # File format: size of file, run time
    f.write(f"{file_size}, {run_time}\n")
    f.close()

if __name__ == "__main__":
    main()

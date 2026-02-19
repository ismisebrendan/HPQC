# Week 4

## Files in this directory

 - ```comm_test_mpi.c``` - File copied from the course repository. [AN MPI FILE]
 - ```comm_test_func_mpi.c``` - ```comm_test_mpi.c``` edited to be function-based. [AN MPI FILE]
 - ```comm_test_Ssend_mpi.c``` - ```comm_test_func_mpi.c``` edited to explicitly use ```MPI_Ssend``` only. [AN MPI FILE]
 - ```comm_test_Bsend_mpi.c``` - ```comm_test_func_mpi.c``` edited to explicitly use ```MPI_Bsend``` only. [AN MPI FILE]
 - ```comm_test_Rsend_mpi.c``` - ```comm_test_func_mpi.c``` edited to explicitly use ```MPI_Rsend``` only. [AN MPI FILE]
 - ```comm_test_Isend_mpi.c``` - ```comm_test_func_mpi.c``` edited to explicitly use ```MPI_Isend``` only. [AN MPI FILE]
 - ```comm_test_Ssend_time.c``` - ```comm_test_Ssend_mpi.c``` edited to explicitly recorde how long it takes each client to send the messages and how long it takes the root to receive each message. [AN MPI FILE]
 - ```comm_test_Bsend_time.c``` - ```comm_test_Bsend_mpi.c``` edited to explicitly recorde how long it takes each client to send the messages and how long it takes the root to receive each message. [AN MPI FILE]
 - ```comm_test_Rsend_time.c``` - ```comm_test_Rsend_mpi.c``` edited to explicitly recorde how long it takes each client to send the messages and how long it takes the root to receive each message. [AN MPI FILE]
 - ```comm_test_Isend_time.c``` - ```comm_test_Isend_mpi.c``` edited to explicitly recorde how long it takes each client to send the messages and how long it takes the root to receive each message. [AN MPI FILE]
 - ```run_send_timing.sh``` - Runs each ```comm_test_*send_time.c``` files 100 times for 2 to 16 processors to see the variability in the send and receive times for each of these for a varitey of different numbers of processors.
 - ```pingpong.c``` - A file that sends a counter back and forth between two processors and times how long it takes. [AN MPI FILE]
 - ```run_pingpong.sh``` - Runs ```pingpong.c``` repeatedly for a number of different inputs.
 - ```vector_broadcast``` - Like the ```vector_parallel.c``` file from [week3](https://github.com/ismisebrendan/HPQC/edit/main/week3/vector_parallel.c), however only the root node initialises the vector, it then sends the vector to each node.
 - ```vector_send_recv``` - Like the ```vector_parallel.c``` file from [week3](https://github.com/ismisebrendan/HPQC/edit/main/week3/vector_parallel.c), however only the root node initialises the vector, it then splits up the vector and sends the chunks to the other nodes using ```MPI_Send``` and ```MPI_Recv``` commands.
 - ```vector_scatter``` - Like the ```vector_parallel.c``` file from [week3](https://github.com/ismisebrendan/HPQC/edit/main/week3/vector_parallel.c), however only the root node initialises the vector, it then splits up the vector and sends the chunks to the other nodes using ```MPI_Scatter```.
 - ```plotting.py``` - Generates histograms of the time taken for each of the different send types and the number of processors, finds the mean times and standard deviation in these. Also generates the plots for the ping pong files.

## What comm_test_mpi.c does

When running the file for N processors, the processors [1, N-1] multiply their rank by 10 and then send that to the root (rank 0).

The non-root processors (rank i) print

```
Hello, I am i of N. Sent 10i to Rank 0
```

Then, upon receiving the message the root processor prints

```
Hello, I am 0 of N. Received 10i from Rank i
```

The order in which the ranks return is variable, and the message from rank i need not be printed before the corresponding message from rank 0 is. The root node always prints in increasing order of i.


## Different send types

The first day that I tested this the below is what I got (all more or less behaving the same), however I plan to check again later.

### Ssend

When running ```comm_test_Ssend_mpi.c``` the client tasks can still print in any order (they are mostly in order, however there are some times when they print out of order), interspersed with the prints from the root tasks. However, the root task messages are printed completely in order (increasing i).

### Bsend

This is similar to the original file, the client tasks return messages in seemingly random order, while the root task prints in increasing order of i, and can print before the corresponding client task does.

### Rsend

This appears to behave the same as the Bsend file.

### Isend

Also appears to behave the same as the Bsend file.

### Benchmarking these

The table below shows the mean times and standard devaiation to send a message for each of the send types
| np | Bsend | Isend | Rsend | Ssend |
| --- | --- | --- | --- | --- |
|2 | 3.8e-05 +/- 6e-06 | 3.7e-05 +/- 6e-06 | 4.1e-05 +/- 7e-06 | 7e-05 +/- 2.7e-05
|3 | 4.1e-05 +/- 1.2e-05 | 4e-05 +/- 1.2e-05 | 4.5e-05 +/- 1.3e-05 | 0.011462 +/- 0.140206
|4 | 4.1e-05 +/- 1.2e-05 | 4e-05 +/- 1.2e-05 | 4.4e-05 +/- 1.4e-05 | 0.00938 +/- 0.11414
|5 | 3.9e-05 +/- 1.3e-05 | 4e-05 +/- 1.5e-05 | 4.4e-05 +/- 1.6e-05 | 0.004442 +/- 0.004484
|6 | 4e-05 +/- 1.2e-05 | 4.1e-05 +/- 1.5e-05 | 4.5e-05 +/- 1.5e-05 | 0.006123 +/- 0.005999
|7 | 4e-05 +/- 1.2e-05 | 4e-05 +/- 1.2e-05 | 4.4e-05 +/- 1.3e-05 | 0.065974 +/- 0.336277
|8 | 4.2e-05 +/- 1.1e-05 | 4.2e-05 +/- 1e-05 | 4.5e-05 +/- 1.2e-05 | 0.008104 +/- 0.007831
|9 | 4.3e-05 +/- 1.1e-05 | 4.3e-05 +/- 1.1e-05 | 4.7e-05 +/- 1.1e-05 | 0.041303 +/- 0.247855
|10 | 4.3e-05 +/- 9e-06 | 4.4e-05 +/- 1e-05 | 0.002247 +/- 0.066296 | 0.025795 +/- 0.172062
|11 | 4.4e-05 +/- 9e-06 | 4.4e-05 +/- 9e-06 | 4.8e-05 +/- 1e-05 | 0.037668 +/- 0.220353
|12 | 4.5e-05 +/- 9e-06 | 4.5e-05 +/- 9e-06 | 4.8e-05 +/- 1e-05 | 0.058112 +/- 0.290809
|13 | 4.5e-05 +/- 9e-06 | 4.5e-05 +/- 9e-06 | 4.9e-05 +/- 1e-05 | 0.015643 +/- 0.014113
|14 | 4.6e-05 +/- 9e-06 | 4.7e-05 +/- 1e-05 | 5e-05 +/- 1e-05 | 0.06137 +/- 0.292466
|15 | 4.7e-05 +/- 9e-06 | 4.7e-05 +/- 1e-05 | 5.1e-05 +/- 1e-05 | 0.038908 +/- 0.199839
|16 | 4.7e-05 +/- 9e-06 | 4.8e-05 +/- 1e-05 | 5.1e-05 +/- 1e-05 | 0.055639 +/- 0.262922

The table below shows the mean times and standard devaiation to receive a message for each of the send types
| np | Bsend | Isend | Rsend | Ssend |
| --- | --- | --- | --- | --- |
|2 | 7.7e-05 +/- 4.5e-05 | 7.1e-05 +/- 4.2e-05 | 8.3e-05 +/- 5.1e-05 | 8.9e-05 +/- 4.7e-05
|3 | 5.5e-05 +/- 5.4e-05 | 5.6e-05 +/- 5.2e-05 | 6e-05 +/- 5.7e-05 | 6.3e-05 +/- 6.5e-05
|4 | 4.9e-05 +/- 5.4e-05 | 4.7e-05 +/- 4.9e-05 | 4.9e-05 +/- 5.9e-05 | 5.1e-05 +/- 5.8e-05
|5 | 4.2e-05 +/- 4.8e-05 | 4.3e-05 +/- 4.8e-05 | 4e-05 +/- 4.5e-05 | 4.5e-05 +/- 4.9e-05
|6 | 3.9e-05 +/- 4.5e-05 | 3.9e-05 +/- 4.9e-05 | 4.2e-05 +/- 5.5e-05 | 4.3e-05 +/- 5e-05
|7 | 4.3e-05 +/- 6.1e-05 | 4e-05 +/- 5.5e-05 | 4.1e-05 +/- 5.5e-05 | 4.4e-05 +/- 6.1e-05
|8 | 4.1e-05 +/- 6.1e-05 | 4e-05 +/- 5.5e-05 | 4e-05 +/- 5.5e-05 | 4.4e-05 +/- 6.6e-05
|9 | 3.8e-05 +/- 5.8e-05 | 3.8e-05 +/- 5.7e-05 | 3.9e-05 +/- 6e-05 | 4.1e-05 +/- 6.1e-05
|10 | 3.7e-05 +/- 6e-05 | 3.8e-05 +/- 6e-05 | 3.8e-05 +/- 6e-05 | 4.2e-05 +/- 6.7e-05
|11 | 3.5e-05 +/- 5.6e-05 | 3.6e-05 +/- 5.8e-05 | 3.5e-05 +/- 5.7e-05 | 4.4e-05 +/- 6.5e-05
|12 | 3.5e-05 +/- 5.6e-05 | 3.5e-05 +/- 5.6e-05 | 3.5e-05 +/- 6e-05 | 4.3e-05 +/- 6.7e-05
|13 | 3.2e-05 +/- 5.5e-05 | 3.3e-05 +/- 5.8e-05 | 3.3e-05 +/- 6e-05 | 4.1e-05 +/- 6.8e-05
|14 | 3.2e-05 +/- 6e-05 | 3.3e-05 +/- 6.1e-05 | 3.3e-05 +/- 6.1e-05 | 4.1e-05 +/- 6.9e-05
|15 | 3.1e-05 +/- 6e-05 | 3.1e-05 +/- 6e-05 | 3.1e-05 +/- 5.7e-05 | 3.8e-05 +/- 6.8e-05
|16 | 3e-05 +/- 5.9e-05 | 3.1e-05 +/- 5.9e-05 | 3.1e-05 +/- 5.9e-05 | 3.7e-05 +/- 7.1e-05


In most cases the variance is on the order of the mean time or even greater than the mean time, showing the inconsistencies in the time taken. It is also generally very short.

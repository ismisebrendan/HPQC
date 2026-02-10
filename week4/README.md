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
 - ```pingpong.c``` - A file that sends a counter back and forth between two processors and times how long it takes. [AN MPI FILE]
 - ```run_pingpong.sh``` - Runs ```pingpong.c``` repeatedly for a number of different inputs.

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

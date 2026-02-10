# Week 4

## Files in this directory

 - ```comm_test_mpi.c``` - File copied from the course repository. [AN MPI FILE]




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

The order in which the ranks return is variable, and the message from rank i need not be printed before the corresponding message from rank 0 is.

# HPQC


A repository for all the work done in the High Performance and Quantum Computing module.


The important stuff for each week's work is given in the corresponding folder.

## Repository structure and running the files

Unless otherwise stated it should be assumed that within the folders for each week (```weekX```) there should be a ```data/``` and ```bin/``` folder (that have been ignored by git). All C and MPI files should be compiled to the ```bin/``` folder. All files should be run from their main ```weekX``` folder unless otherwise stated.

**Python files** should be run with
```
python3 python/file/path.py
```

**C files** should be compiled with 
```
gcc -o weekX/bin/output_file_name ./c/file/path.c
```
and run with
```
./weekX/bin/output_file_name
```

**MPI files** should be compiled with
```
mpicc -o weekX/bin/output_file_name ./mpi/file/path.c
```
and run with
```
mpirun -np numer_of_processors weekX/bin/output_file_name
```

MPI files are labelled in the ```README.md``` files in their respective folders with [AN MPI FILE].

All ```.sh``` files assume the C and MPI files have been compiled to ```./bin/file_name_without_extension```. They should be run with
```
bash file_name.sh
```

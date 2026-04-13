# Week 5

## Files in this directory

Note: when compiling any of the ```.c``` files in this directory ```-lm``` should be added to the end of the statement in order to include the ```math.h``` library.

 - ```animate_line_file.py``` - File copied from course directory.
 - ```animate_line.py``` - File copied from course directory.
 - ```string_wave.c``` - File copied from course directory.
 - ```sample_animation.gif``` - Animation created by copie files.
 - ```string_params.c``` - Edited version of ```string_wave.c``` that avoids the hardcoding of various values used when simulating the vibrating string. Times how long it takes internally. Takes three numerical inputs: the number of points to simulate on the string, the number of cycles to simulate, and the number of samples to take per cycle. Also optionally takes the path of an output file, otherwise it defaults to ```data/string_wave.csv```.
 - ```animate_line_params.py``` - Edited version of ```animate_line_file.py``` that avoids hardcoding the output file and colour of the points of the animation (the input file was already not hardcoded).
 - ```string_parallel.c``` - A parallelised version of ```string_params.c```. Takes three numerical inputs: the number of points to simulate on the string, the number of cycles to simulate, and the number of samples to take per cycle. Also optionally takes the path of an output file, otherwise it defaults to ```data/string_wave.csv```. [AN MPI FILE]
 - ```run_string.sh``` - Runs ```string_params.c``` and ```string_parallel.c``` repeatedly for different inputs.

## Removing hardcoding

### Simulation

```string_params.c``` was copied from ```string_wave.c``` and edited so that the user could specify:

 1. The number of points to simulate on the string.
 2. The number of cycles to simulate.
 3. The number of time samples to take per sample.
 4. The path of an output file for the results of the simulation.

The output is optional, if it is not given then the output defaults to ```data/string_wave.csv```.

The correct usage is:
```string_params [POINTS] [CYCLES] [SAMPLES] (optional [OUTPUT FILE PATH])```

### Animation

```animate_line_params.py``` was copied from ```animate_line_file.py``` and edited so that the user could specify:

 1. The path of an input file to generate the animation from.
 2. The path of an output file for the animation to be save to.
 3. The colour of the points in the animation.

The colour is optional, if it is not given then it defaults to green.

The correct usage is:
```animate_line_params.py [FILENAME_IN] [FILENAME_OUT] (optional [COLOUR])```


## Parallelising the code

### Strategy for parallelising

The code will initially break the string into as many chunks as there are processors minus 1. These chunks will be assigned to each node. Except for the root node (node 0) and node 1 which will host the driver each node will take the position of the last point on the previous node at the previous timestamp in order to propagate the wave. Each node except for the last will broadcast the position of their final point to the adjoining node.

The results from each timestamp will be broadcast to the root node which will aggregate them and write them to the output file.

### Strategy for aggregating the data

In order to minimise the overhead associated with opening and saving the file each time, the data will be stored in memory until the program is finished, and then the root node will save the file.

```MPI_Gather()``` and ```MPI_Scatter()``` will not be used as the uneven lengths of chunks causes problems for this method. I cannot use the trick of adding extra 0s to the end of the last chunk here as it would add extra points, and limit freedom in part 3.



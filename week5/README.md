# Week 5

## Files in this directory

 - ```animate_line_file.py``` - File copied from course directory.
 - ```animate_line.py``` - File copied from course directory.
 - ```string_wave.c``` - File copied from course directory.
 - ```sample_animation.gif``` - Animation created by copie files.
 - ```string_params.c``` - Edited version of ```string_wave.c``` that avoids the hardcoding of various values used when simulating the vibrating string. Takes three numerical inputs: the number of points to simulate on the string, the number of cycles to simulate, and the number of samples to take per cycle. Also optionally takes the path of an output file, otherwise it defaults to ```data/string_wave.csv```.
 - ```animate_line_params.py``` - Edited version of ```animate_line_file.py``` that avoids hardcoding the output file and colour of the points of the animation (the input file was already not hardcoded).


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





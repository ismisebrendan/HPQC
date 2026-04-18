import numpy as np
import matplotlib.pyplot as plt


# Import data

ser = np.genfromtxt('./data/string_serial_time.txt', delimiter=',')
par = np.genfromtxt('./data/string_parallel_time.txt', delimiter=',')

# Files in format
# no. processors, points, cycles, samples, time

# Totals
## First compute them
total_ser = ser[:,1] * ser[:,2] * ser[:,3] # The total number of points to compute
total_par = par[:,1] * par[:,2] * par[:,3] # The total number of points to compute

## Then add all to the one list
totals = []
totals.append(total_ser)

for i in range(3, 17):
    totals.append(total_par[par[:,0] == i])

# Times
time_ser = ser[:,4]
time_par = par[:,4]

times = []
times.append(time_ser)

for i in range(3, 17):
    times.append(time_par[par[:,0] == i])

np = [1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
# markers = ['o', 'x', '*']
markers = ['o', 'x', '+']

plt.figure(figsize=(9, 7), dpi=100)

for i in range(len(times)):
    plt.scatter(totals[i], times[i], label=np[i], marker=markers[i%len(markers)])#, s=5)

plt.xlabel('Number of simulated points')
plt.ylabel('Time [s]')
plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
plt.title('Time taken to run against number of simulated points for all combinations tested')
plt.savefig('./data/all_plot.png')
plt.close()

plt.figure(figsize=(9, 7), dpi=100)

plt.scatter(totals[0], times[0])

plt.xlabel('Number of simulated points')
plt.ylabel('Time [s]')
plt.title('Time taken to run against number of simulated points for the serial program')
plt.savefig('./data/serial_plot.png')
plt.close()




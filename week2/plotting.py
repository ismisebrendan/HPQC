import numpy as np
import matplotlib.pyplot as plt

# Plot the time for each language separately

## Python
print_py = np.genfromtxt('./data/python_time_print_results.txt', delimiter=',')
save_py = np.genfromtxt('./data/python_time_save_results.txt', delimiter=',')
read_py = np.genfromtxt('./data/python_time_read_results.txt', delimiter=',')

fig, ax1 = plt.subplots()

# Plot the print and save results on the same axes
ax1.plot(print_py[:,0], print_py[:,1], marker='o', markersize=3, label='Print')
ax1.plot(save_py[:,0], save_py[:,1], marker='o', markersize=3, label='Save')
ax1.set_ylabel('Time [s]')
ax1.set_xlabel('Input value')
ax1.grid()

# Plot the read results on a different x-axis, same y-axis
ax2 = ax1.twiny()
ax2.plot(read_py[:,0], read_py[:,1], marker='o', markersize=3, color='tab:green', label='Read')
ax2.set_xlabel('Input file size [B]', color='tab:green')
ax2.tick_params('x', colors='tab:green')

# Sort out the legend
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
lines = lines1 + lines2
labels = labels1 + labels2
plt.legend(lines, labels, loc='upper left')

plt.title('Time for Python programs')
fig.tight_layout()
plt.savefig('./data/python_plot.png')
# Plot with logarithmic axes
ax1.set_yscale('log')
ax1.set_xscale('log')
ax2.set_yscale('log')
ax2.set_xscale('log')
plt.savefig('./data/python_plot_log.png')
plt.close()

## C 
print_c = np.genfromtxt('./data/c_time_print_results.txt', delimiter=',')
save_c = np.genfromtxt('./data/c_time_save_results.txt', delimiter=',')
read_c = np.genfromtxt('./data/c_time_read_results.txt', delimiter=',')

fig, ax1 = plt.subplots()

# Plot the print and save results on the same axes
ax1.plot(print_c[:,0], print_c[:,1], marker='o', markersize=3, label='Print')
ax1.plot(save_c[:,0], save_c[:,1], marker='o', markersize=3, label='Save')
ax1.set_ylabel('Time [s]')
ax1.set_xlabel('Input value')
ax1.grid()

# Plot the read results on a different x-axis, same y-axis
ax2 = ax1.twiny()
ax2.plot(read_c[:,0], read_c[:,1], marker='o', markersize=3, color='tab:green', label='Read')
ax2.set_xlabel('Input file size [B]', color='tab:green')
ax2.tick_params('x', colors='tab:green')

# Sort out the legend
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
lines = lines1 + lines2
labels = labels1 + labels2
plt.legend(lines, labels, loc='upper left')

plt.title('Time for C programs')
fig.tight_layout()
plt.savefig('./data/c_plot.png')
# Plot with logarithmic axes
ax1.set_yscale('log')
ax1.set_xscale('log')
ax2.set_yscale('log')
ax2.set_xscale('log')
plt.savefig('./data/c_plot_log.png')
plt.close()

# Plot the time for each operation separately, with both languages

## Print
fig, ax1 = plt.subplots()

ax1.plot(print_py[:,0], print_py[:,1], marker='o', markersize=3, label='Python')
ax1.plot(print_c[:,0], print_c[:,1], marker='o', markersize=3, label='C')
ax1.set_ylabel('Time [s]')
ax1.set_xlabel('Input value')
ax1.grid()

plt.legend()

plt.title('Time to print the data')
fig.tight_layout()
plt.savefig('./data/print_plot.png')
# Plot with logarithmic axes
ax1.set_yscale('log')
ax1.set_xscale('log')
plt.savefig('./data/print_plot_log.png')
plt.close()

## Save
fig, ax1 = plt.subplots()

ax1.plot(save_py[:,0], save_py[:,1], marker='o', markersize=3, label='Python')
ax1.plot(save_c[:,0], save_c[:,1], marker='o', markersize=3, label='C')
ax1.set_ylabel('Time [s]')
ax1.set_xlabel('Input value')
ax1.grid()

plt.legend()

plt.title('Time to save the data')
fig.tight_layout()
plt.savefig('./data/save_plot.png')
# Plot with logarithmic axes
ax1.set_yscale('log')
ax1.set_xscale('log')
plt.savefig('./data/save_plot_log.png')
plt.close()

## Read
fig, ax1 = plt.subplots()

ax1.plot(read_py[:,0], read_py[:,1], marker='o', markersize=3, label='Python')
ax1.plot(read_c[:,0], read_c[:,1], marker='o', markersize=3, label='C')
ax1.set_ylabel('Time [s]')
ax1.set_xlabel('Input file size [B]')
ax1.grid()

plt.legend()

plt.title('Time to read the data')
fig.tight_layout()
plt.savefig('./data/read_plot.png')
# Plot with logarithmic axes
ax1.set_yscale('log')
ax1.set_xscale('log')
plt.savefig('./data/read_plot_log.png')
plt.close()

# Plot all values together
fig, ax1 = plt.subplots()

# Plot the print and save results on the same axes
ax1.plot(print_py[:,0], print_py[:,1], marker='o', markersize=3, color='tab:blue', label='Print, Python')
ax1.plot(print_c[:,0], print_c[:,1], marker='x', markersize=3, color='tab:blue', linestyle=':', label='Print, C')
ax1.plot(save_py[:,0], save_py[:,1], marker='o', markersize=3, color='tab:orange',  label='Save, Python')
ax1.plot(save_c[:,0], save_c[:,1], marker='x', markersize=3, color='tab:orange', linestyle=':', label='Save, C')
ax1.set_ylabel('Time [s]')
ax1.set_xlabel('Input value')
ax1.grid()

# Plot the read results on a different x-axis, same y-axis
ax2 = ax1.twiny()
ax2.plot(read_py[:,0], read_py[:,1], marker='o', markersize=3, color='tab:green', label='Read, Python')
ax2.plot(read_c[:,0], read_c[:,1], marker='x', markersize=3, color='tab:green', linestyle=':', label='Read, C')
ax2.set_xlabel('Input file size [B]', color='tab:green')
ax2.tick_params('x', colors='tab:green')

# Sort out the legend
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
lines = lines1 + lines2
labels = labels1 + labels2
plt.legend(lines, labels, loc='upper left')

plt.title('Time for all programs')
fig.tight_layout()
plt.savefig('./data/all_plot.png')
# Plot with logarithmic axes
ax1.set_yscale('log')
ax1.set_xscale('log')
ax2.set_yscale('log')
ax2.set_xscale('log')
plt.savefig('./data/all_plot_log.png')
plt.close()

import numpy as np
import matplotlib.pyplot as plt

# Plot the time for each method separately

## Internal
internal = np.genfromtxt('./data/internal_timing.txt', delimiter=',')

fig, ax1 = plt.subplots()

# Plot the print and save results on the same axes
ax1.plot(internal[:,0], internal[:,1], marker='o', markersize=3, label='Print')
plt.legend(loc='upper left')

plt.title('Time recorded internally')
fig.tight_layout()
plt.savefig('./data/internal_plot.png')
# Plot with logarithmic axes
ax1.set_yscale('log')
ax1.set_xscale('log')
plt.savefig('./data/internal_plot_log.png')
plt.close()

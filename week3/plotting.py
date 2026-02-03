import numpy as np
import matplotlib.pyplot as plt

# Plot the time for each method separately

## Internal
internal = np.genfromtxt('./data/internal_timing.txt', delimiter=',')


# Plot the print and save results on the same axes
plt.plot(internal[:,0], internal[:,1], marker='o', markersize=3, label='Internal')
plt.grid()
plt.legend(loc='upper left')

plt.title('Time recorded internally')
plt.tight_layout()
plt.savefig('./data/internal_plot.png')
# Plot with logarithmic axes
plt.yscale('log')
plt.xscale('log')
plt.tight_layout()
plt.savefig('./data/internal_plot_log.png')
plt.close()

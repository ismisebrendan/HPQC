import numpy as np
import matplotlib.pyplot as plt

# Plot the time for each method separately

## Internal
internal_serial = np.genfromtxt('./data/serial_timing_internal.txt', delimiter=',')
internal_parallel = np.genfromtxt('./data/parallel_timing_internal.txt', delimiter=',')


plt.plot(internal_serial[:,0], internal_serial[:,1], marker='o', markersize=3, label='Serial Internal')

# Plot each number of processes separately
for i in range(2, 17):
    parallel_plot = internal_parallel[internal_parallel[:,0]==i]
    plt.plot(parallel_plot[:,1], parallel_plot[:,2], marker='o', markersize=3, label=f'Parallel Internal np={i}')

plt.grid()
plt.legend(loc='upper left')

plt.title('Time recorded internally')
plt.tight_layout()
plt.show()
# plt.savefig('./data/internal_plot.png')
# Plot with logarithmic axes
plt.yscale('log')
plt.xscale('log')
plt.tight_layout()
# plt.savefig('./data/internal_plot_log.png')
plt.close()
















# ## External, only get real values
# external = np.genfromtxt('./data/external_timing.txt', delimiter=',', usecols=[0])
# external = np.reshape(external, (int(len(external)/2), 2))

# plt.plot(external[:,0], external[:,1], marker='o', markersize=3, label='External')
# plt.grid()
# plt.legend(loc='upper left')

# plt.title('Time recorded externally')
# plt.tight_layout()
# plt.savefig('./data/external_plot.png')
# # Plot with logarithmic axes
# plt.yscale('log')
# plt.xscale('log')
# plt.tight_layout()
# plt.savefig('./data/external_plot_log.png')
# plt.close()

# # Plot them together
# plt.plot(external[:,0], external[:,1], marker='o', markersize=3, label='External')
# plt.plot(internal[:,0], internal[:,1], marker='o', markersize=3, label='Internal')
# plt.grid()
# plt.legend(loc='upper left')

# plt.title('Time recorded')
# plt.tight_layout()
# plt.savefig('./data/serial_plot.png')
# # Plot with logarithmic axes
# plt.yscale('log')
# plt.xscale('log')
# plt.tight_layout()
# plt.savefig('./data/serial_plot_log.png')
# plt.close()

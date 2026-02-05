import numpy as np
import matplotlib.pyplot as plt

# Plot the time for each method separately

# No overhead
internal_serial = np.genfromtxt('./data/serial_timing_internal.txt', delimiter=',')
internal_parallel = np.genfromtxt('./data/parallel_timing_internal.txt', delimiter=',')

# Plot each number of processes separately
for i in range(2, 17):
    plt.plot(internal_serial[:,0], internal_serial[:,1], marker='o', markersize=3, label='Serial Internal')

    parallel_plot = internal_parallel[internal_parallel[:,0]==i]
    plt.plot(parallel_plot[:,1], parallel_plot[:,2], marker='o', markersize=3, label=f'Parallel Internal np={i}')

    plt.grid()
    plt.legend()

    plt.title('Time recorded internally')
    plt.ylim([-0.00001, 0.0006])
    plt.tight_layout()
    plt.savefig(f'./data/internal_plot_{i}_processors.png')
    # plt.show()
    
    # Plot with logarithmic axes
    plt.yscale('log')
    plt.xscale('log')
    plt.ylim([7e-7, 0.0006])
    plt.tight_layout()
    plt.savefig(f'./data/internal_plot_{i}_processors_log.png')
    # plt.show()
    plt.close()

# Overhead
overhead_serial = np.genfromtxt('./data/serial_timing_internal_overhead.txt', delimiter=',')
overhead_parallel = np.genfromtxt('./data/parallel_timing_internal_overhead.txt', delimiter=',')

# Plot the serial process on its own
plt.plot(overhead_serial[:,0], overhead_serial[:,1], marker='o', markersize=3, label='Serial Internal')

plt.grid()
plt.legend()

plt.title('Time recorded internally for serial, including overhead')
plt.tight_layout()
plt.savefig(f'./data/internal_plot_serial_overhead.png')
# plt.show()

# Plot with logarithmic axes
plt.yscale('log')
plt.xscale('log')
plt.tight_layout()
plt.savefig(f'./data/internal_plot_serial_overhead_log.png')
# plt.show()
plt.close()

# Plot each number of processes separately
for i in range(2, 17):
    plt.plot(overhead_serial[:,0], overhead_serial[:,1], marker='o', markersize=3, label='Serial Internal')

    parallel_plot = overhead_parallel[overhead_parallel[:,0]==i]
    plt.plot(parallel_plot[:,1], parallel_plot[:,2], marker='o', markersize=3, label=f'Parallel Internal np={i}')

    plt.grid()
    plt.legend()

    plt.title('Time recorded internally, including overhead')
    plt.tight_layout()
    plt.savefig(f'./data/internal_plot_{i}_processors_overhead.png')
    # plt.show()
    
    # Plot with logarithmic axes
    plt.yscale('log')
    plt.xscale('log')
    plt.tight_layout()
    plt.savefig(f'./data/internal_plot_{i}_processors_overhead_log.png')
    # plt.show()
    plt.close()
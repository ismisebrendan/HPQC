import numpy as np
import matplotlib.pyplot as plt

# Plot the time for each method separately

# Internal
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


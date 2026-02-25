import numpy as np
import matplotlib.pyplot as plt


# Bsend
Bsend_recv = np.genfromtxt('./data/Bsend_revc_time.txt', delimiter=',')
Bsend_send = np.genfromtxt('./data/Bsend_send_time.txt', delimiter=',')

hbins = np.linspace(0, np.max([Bsend_recv[:,2], Bsend_send[:,2]]), 20)
for i in range(2, 17):
     print(f'For Bsend, with {i} processors:')
     print(f'Mean time to send: {np.mean(Bsend_send[Bsend_send[:,0] == i][:,2])}, std = {np.std(Bsend_send[Bsend_send[:,0] == i][:,2])}')
     print(f'Mean time to receive: {np.mean(Bsend_recv[Bsend_recv[:,0] == i][:,2])}, std = {np.std(Bsend_recv[Bsend_send[:,0] == i][:,2])}')
     plt.hist(Bsend_recv[Bsend_send[:,0] == i][:,2], hbins, label='Send time', fill=False, edgecolor='tab:orange')
     plt.title(f'Time taken for Bsend for {i} processors to send')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Bsend_send{i}.png')
     plt.close()
 
     plt.hist(Bsend_recv[Bsend_recv[:,0] == i][:,2], hbins, label='Receive time', fill=False, edgecolor='tab:blue')
     plt.title(f'Time taken for Bsend for {i} processors to receive')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Bsend_recv{i}.png')
     plt.close()

# Isend
Isend_recv = np.genfromtxt('./data/Isend_revc_time.txt', delimiter=',')
Isend_send = np.genfromtxt('./data/Isend_send_time.txt', delimiter=',')

hbins = np.linspace(0, np.max([Isend_recv[:,2], Isend_send[:,2]]), 20)
for i in range(2, 17):
     print(f'For Isend, with {i} processors:')
     print(f'Mean time to send: {np.mean(Isend_send[Isend_send[:,0] == i][:,2])}, std = {np.std(Isend_send[Isend_send[:,0] == i][:,2])}')
     print(f'Mean time to receive: {np.mean(Isend_recv[Isend_recv[:,0] == i][:,2])}, std = {np.std(Isend_recv[Isend_send[:,0] == i][:,2])}')
     plt.hist(Isend_recv[Isend_send[:,0] == i][:,2], hbins, label='Send time', fill=False, edgecolor='tab:orange')
     plt.title(f'Time taken for Isend for {i} processors to send')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Isend_send{i}.png')
     plt.close()

     plt.hist(Isend_recv[Isend_recv[:,0] == i][:,2], hbins, label='Receive time', fill=False, edgecolor='tab:blue')
     plt.title(f'Time taken for Isend for {i} processors to receive')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Isend_recv{i}.png')
     plt.close()

# Rsend
Rsend_recv = np.genfromtxt('./data/Rsend_revc_time.txt', delimiter=',')
Rsend_send = np.genfromtxt('./data/Rsend_send_time.txt', delimiter=',')

hbins = np.linspace(0, np.max([Rsend_recv[:,2], Rsend_send[:,2]]), 20)
for i in range(2, 17):
     print(f'For Rsend, with {i} processors:')
     print(f'Mean time to send: {np.mean(Rsend_send[Rsend_send[:,0] == i][:,2])}, std = {np.std(Rsend_send[Rsend_send[:,0] == i][:,2])}')
     print(f'Mean time to receive: {np.mean(Rsend_recv[Rsend_recv[:,0] == i][:,2])}, std = {np.std(Rsend_recv[Rsend_recv[:,0] == i][:,2])}')
     plt.hist(Rsend_recv[Rsend_send[:,0] == i][:,2], hbins, label='Send time', fill=False, edgecolor='tab:orange')
     plt.title(f'Time taken for Rsend for {i} processors to send')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Rsend_send{i}.png')
     plt.close()

     plt.hist(Rsend_recv[Rsend_recv[:,0] == i][:,2], hbins, label='Receive time', fill=False, edgecolor='tab:blue')
     plt.title(f'Time taken for Rsend for {i} processors to receive')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Rsend_recv{i}.png')
     plt.close()

# Ssend
Ssend_recv = np.genfromtxt('./data/Ssend_revc_time.txt', delimiter=',')
Ssend_send = np.genfromtxt('./data/Ssend_send_time.txt', delimiter=',')

hbins = np.linspace(0, np.max([Ssend_recv[:,2], Ssend_send[:,2]]), 20)
for i in range(2, 17):
     print(f'For Ssend, with {i} processors:')
     print(f'Mean time to send: {np.mean(Ssend_send[Ssend_send[:,0] == i][:,2])}, std = {np.std(Ssend_send[Ssend_send[:,0] == i][:,2])}')
     print(f'Mean time to receive: {np.mean(Ssend_recv[Ssend_recv[:,0] == i][:,2])}, std = {np.std(Ssend_recv[Ssend_send[:,0] == i][:,2])}')
     plt.hist(Ssend_recv[Ssend_send[:,0] == i][:,2], hbins, label='Send time', fill=False, edgecolor='tab:orange')
     plt.title(f'Time taken for Ssend for {i} processors to send')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Ssend_send{i}.png')
     plt.close()
 
     plt.hist(Ssend_recv[Ssend_recv[:,0] == i][:,2], hbins, label='Receive time', fill=False, edgecolor='tab:blue')
     plt.title(f'Time taken for Ssend for {i} processors to receive')
     plt.xlabel('Time')
     plt.ylabel('Count')
     plt.legend()
     plt.savefig(f'./data/Ssend_recv{i}.png')
     plt.close()

# Ping pong
pingpong = np.genfromtxt('./data/pingpong.txt', delimiter=',')

plt.scatter(pingpong[:,0], pingpong[:,2])
plt.xscale('log')
plt.yscale('log')
plt.title('Average time taken to send a message between two processors')
plt.xlabel('Number of messages sent')
plt.ylabel('Time')
plt.ylim([7e-7, 1e-2])
plt.savefig('./data/pingpong.png')
plt.close()

pingpong_vector = np.genfromtxt('./data/pingpong_vector.txt', delimiter=',')

# Find the fit
pingpong_vector_low = pingpong_vector[pingpong_vector[:,1] < 1]
slope, intercept = np.polyfit(pingpong_vector[:,0], pingpong_vector[:,1], 1)
slope_low, intercept_low = np.polyfit(pingpong_vector_low[:,0], pingpong_vector_low[:,1], 1)

print('Taking all data into account')
print(f'm = {slope}')
print(f'c = {intercept}')

print('Only taking small values')
print(f'm = {slope_low}')
print(f'c = {intercept_low}')

plt.plot([0, max(pingpong_vector[:,0])], [intercept, intercept + slope*max(pingpong_vector[:,0])], color='red', label='Fit')
plt.plot([0, max(pingpong_vector[:,0])], [intercept_low, intercept_low + slope_low*max(pingpong_vector[:,0])], color='black', label='Fit (no high values)')
plt.scatter(pingpong_vector[:,0], pingpong_vector[:,1], label='Data')
plt.legend()
plt.title('Time taken (T) to send a message of size N')
plt.ylabel('T [s]')
plt.xlabel('N [b]')
plt.savefig('./data/pingpong_vector.png')
plt.close()


# Broadcast, scatter, send recv
broadcast = np.genfromtxt('./data/vector_broadcast_time.txt', delimiter=',')
scatter = np.genfromtxt('./data/vector_scatter_time.txt', delimiter=',')
sendrecv = np.genfromtxt('./data/vector_send_recv_time.txt', delimiter=',')

# Find the mean times for each combination
bmeans = np.mean(broadcast.reshape((15, 11, 10, 3)), axis=2)
smeans = np.mean(scatter.reshape((15, 11, 10, 3)), axis=2)
srmeans = np.mean(sendrecv.reshape((15, 11, 10, 3)), axis=2)

slow_lst = []
sec_lst = []
fast_lst = []

print('Processors, Elements, Fastest  , Second   , Slowest')
for i in range(len(bmeans)):
     for j in range(len(bmeans[i])):
          # Find slowest, then fastest
          if (bmeans[i,j,2] < srmeans[i,j,2]) and (bmeans[i,j,2] < smeans[i,j,2]):
               slowest = 'Broadcast'
               if srmeans[i,j,2] < smeans[i,j,2]:
                    second = 'Send recv'
                    fastest = 'Scatter'
               else:
                    fastest = 'Send recv'
                    second = 'Scatter'
          elif (srmeans[i,j,2] < bmeans[i,j,2]) and (srmeans[i,j,2] < smeans[i,j,2]):
               slowest = 'Send recv'
               if bmeans[i,j,2] < smeans[i,j,2]:
                    second = 'Broadcast'
                    fastest = 'Scatter'
               else:
                    fastest = 'Broadcast'
                    second = 'Scatter'
          elif (srmeans[i,j,2] < bmeans[i,j,2]) and (srmeans[i,j,2] < smeans[i,j,2]):
               slowest = 'Scatter'
               if bmeans[i,j,2] < srmeans[i,j,2]:
                    second = 'Broadcast'
                    fastest = 'Send recv'
               else:
                    fastest = 'Broadcast'
                    second = 'Send recv'
          slow_lst.append(slowest)
          sec_lst.append(second)
          fast_lst.append(fastest)
          print(f'{i+2:10.0f}, {10+20*j:8.0f}, {fastest:9}, {second:9}, {slowest:9}')

print('---')
print(f'Slowest count : Broadcast {slow_lst.count("Broadcast")}, Scatter {slow_lst.count("Scatter")}, Send recv {slow_lst.count("Send recv")}')
print(f'Second count : Broadcast {sec_lst.count("Broadcast")}, Scatter {sec_lst.count("Scatter")}, Send recv {sec_lst.count("Send recv")}')
print(f'Fastest count : Broadcast {fast_lst.count("Broadcast")}, Scatter {fast_lst.count("Scatter")}, Send recv {fast_lst.count("Send recv")}')



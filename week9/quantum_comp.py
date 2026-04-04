import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace, (1, -1))
    workspace = np.kron(workspace, weights)

workspace = np.array([[1.]]) # Empty qubit stack
pushQubit([1, 0])
print(workspace)
pushQubit([3/5, 4/5])
print(workspace)

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace,(-1, gate.shape[0]))
    np.matmul(workspace, gate.T, out=workspace)

# Pauli X gate (equivalent to NOT gate)
X_gate = np.array([[0, 1],
                   [1, 0]])

print('Pauli X')
workspace = np.array([[1.]]) # Reset
pushQubit([1, 0])
print('Input: ', workspace)
applyGate(X_gate)
print('Output: ', workspace)

# Hadamard gate
H_gate = np.array([[1, 1],
                   [1, -1]]) * np.sqrt(1/2)

print('Hadamard')
workspace = np.array([[1.]]) # Reset
pushQubit([1, 0])
print('Input: ', workspace)
applyGate(H_gate)
print('Output: ', workspace)

# T gate
T_gate = np.array([[1, 0],
                   [0, np.exp(np.pi/-4j)]])

print('T')
workspace = np.array([[1.+0j]]) # Reset to complex
pushQubit([0.6, 0.8])
print('Input: ', workspace)
applyGate(T_gate)
print('Output: ', workspace)

# SWAP gate
SWAP_gate = np.array([[1, 0, 0, 0],
                      [0, 1, 0, 0],
                      [0, 0, 1, 0],
                      [0, 0, 0, 1]])

print('SWAP')
workspace = np.array([[1.]]) # Reset
pushQubit([1, 0])
pushQubit([0.6, 0.8])
print('Input: ', workspace)
applyGate(SWAP_gate)
print('Output: ', workspace)

# Top of stack
def tosQubit(k):
    global workspace
    if k > 1: # If non-trivial
        workspace = np.reshape(workspace, (-1, 2, 2**(k-1)))
        workspace = np.swapaxes(workspace, -2, -1)

print('--- Top of stack ---')
workspace = np.array([[1.]]) # Reset
pushQubit([1, 0])
pushQubit([0.6, 0.8])
print('Input: ', workspace)
tosQubit(2)
print('Output: ', workspace)

# Measuring qubits
print('--- Measuring qubits ---')
def probQubit():
    global workspace
    workspace = np.reshape(workspace, (-1, 2)) 
    return np.linalg.norm(workspace, axis=0)**2

def measureQubit():
    global workspace
    prob = probQubit()
    measurement = np.random.choice(2, p=prob) # Select 0 or 1 
    workspace = (workspace[:, [measurement]] / np.sqrt(prob[measurement])) 
    return str(measurement)

workspace = np.array([[1.]]) # Reset
for n in range(30):
    pushQubit([0.6, 0.8])
    print(measureQubit(), end='')

print('')

# Toffoli gate
TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0],
                      [0, 1, 0, 0, 0, 0, 0, 0],
                      [0, 0, 1, 0, 0, 0, 0, 0],
                      [0, 0, 0, 1, 0, 0, 0, 0],
                      [0, 0, 0, 0, 1, 0, 0, 0],
                      [0, 0, 0, 0, 0, 1, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 1],
                      [0, 0, 0, 0, 0, 0, 1, 0]])

workspace = np.array([[1.]]) # Reset
for i in range(16):
    pushQubit([1,0]) # push a zero qubit
    applyGate(H_gate) # set equal 0 and 1 probability
    pushQubit([1,0]) # push a 2nd zero qubit
    applyGate(H_gate) # set equal 0 and 1 probability
    pushQubit([1,0]) # push a dummy zero qubit
    applyGate(TOFF_gate)# compute Q3 = Q1 AND Q2
    q3 = measureQubit() # pop qubit 3
    q2 = measureQubit() # pop qubit 2
    q1 = measureQubit() # pop qubit 1
    print(q1+q2+q3,end=',')

print('')

# Improvements

def pushQubit(name, weights):
    global workspace
    global namestack
    
    # If the workspace is empty then reset the names
    if workspace.shape == (1, 1):
        namestack = []
    
    namestack.append(name) # Add the name
    
    # Normalise
    weights = weights / np.linalg.norm(weights)
    weights = np.array(weights, dtype=workspace[0,0].dtype)
    workspace = np.reshape(workspace,(1,-1)) # To row vector
    workspace = np.kron(workspace,weights)

print('Improved pushQubit')
workspace = np.array([[1.]]) # Reset
pushQubit('Q1', [1, 1])
print(np.reshape(workspace, (1, -1)))
print(namestack)
pushQubit('Q2', [0, 1])
print(np.reshape(workspace, (1, -1)))
print(namestack)

def tosQubit(name):
    global workspace
    global namestack
    
    k = len(namestack) - namestack.index(name)
    if k > 1: # If non-trivial
        namestack.append(namestack.pop(-k)) # Rotate namestack 
        workspace = np.reshape(workspace, (-1, 2, 2**(k-1))) 
        workspace = np.swapaxes(workspace, -2, -1)

print('Improved tosQubit')
print(np.reshape(workspace,(1,-1)))  # Print workspace as vector 
print(namestack)

tosQubit('Q1')                       # Swap qubits
print(np.reshape(workspace,(1,-1)))  # Print workspace as vector
print(namestack)

def applyGate(gate, *names):
    global workspace
    for name in names: # Move qubits to TOS
        tosQubit(name)
    workspace = np.reshape(workspace, (-1, gate.shape[0]))
    np.matmul(workspace, gate.T, out=workspace)

print('Improved applyGate')
print(np.reshape(workspace,(1,-1))) # Print workspace as vector 
print(namestack)
applyGate(H_gate,'Q2') # H gate on qubit 2 
print(np.reshape(workspace,(1,-1))) # turns a 0 qubit to 1 with 50% probability
print(namestack)

def probQubit(name):
    global workspace
    tosQubit(name)
    workspace = np.reshape(workspace, (-1, 2))
    prob = np.linalg.norm(workspace, axis=0)**2
    return prob/prob.sum() # Make sure sum is one

def measureQubit(name): 
    global workspace 
    global namestack
    prob = probQubit(name)
    measurement = np.random.choice(2, p=prob)
    workspace = (workspace[:, [measurement]] / np.sqrt(prob[measurement]))
    namestack.pop()
    return str(measurement)

print('Improved applyQubit and measureQubit')
workspace = np.array([[1.]])
pushQubit('Q1', [1, 0])
applyGate(H_gate, 'Q1')
print('Q1 probabilities: ', probQubit('Q1')) # Peek Q1 prob
pushQubit('Q2', [0.6, 0.8])
print('Q2 probabilities: ', probQubit('Q2')) # Peek Q2 prob
print(measureQubit('Q1'), measureQubit('Q2'))

# Controlled NOT gate (equivalent to XOR gate)
CNOT_gate = np.array([[1, 0, 0, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1],
                      [0, 0, 1, 0]])

Tinv_gate = np.array([[1, 0], 
                      [0, np.exp(np.pi / 4j)]])
def toffEquiv_gate(q1, q2, q3): # define Toffoli gate using H, T, T*, CNOT
    applyGate(H_gate, q3)
    applyGate(CNOT_gate, q2, q3) 
    applyGate(Tinv_gate, q3) 
    applyGate(CNOT_gate, q1, q3) 
    applyGate(T_gate, q3)
    applyGate(CNOT_gate, q2, q3) 
    applyGate(Tinv_gate, q3) 
    applyGate(CNOT_gate, q1, q3) 
    applyGate(T_gate, q2) 
    applyGate(T_gate, q3) 
    applyGate(H_gate, q3) 
    applyGate(CNOT_gate, q1, q2) 
    applyGate(T_gate, q1) 
    applyGate(Tinv_gate, q2) 
    applyGate(CNOT_gate, q1, q2)
    
print('Equivalent to Toffoli gate')
for i in range(16): # Test function
    workspace = np.array([[1.+0j]]) # Reset to complex
    pushQubit('Q1', [1, 1])
    pushQubit('Q2', [1, 1])
    pushQubit('Q3', [1, 0])
    toffEquiv_gate('Q1', 'Q2', 'Q3') # compute Q3 = Q1 AND Q2
    print(measureQubit('Q1') + measureQubit('Q2') + measureQubit('Q3'), end=',')

print('')

print('--- Recycling Qubits ---')
def TOFF3_gate(q1, q2, q3, q4): # q4 = q4 XOR (q1 AND q2 AND q3)
    pushQubit('temp', [1, 0]) # Temporary 0 qubit
    applyGate(TOFF_gate, q1, q2, 'temp') # temp = q1 AND q2
    applyGate(TOFF_gate, 'temp', q3, q4) # q4 = q4 XOR (t AND q3)
    applyGate(TOFF_gate, q1, q2, 'temp') # Reset temp 
    measureQubit('temp')

for i in range(20):
    workspace = np.array([[1.]]) # Reset
    pushQubit('Q1', [1, 1]) 
    pushQubit('Q2', [1, 1]) 
    pushQubit('Q3', [1, 1]) 
    pushQubit('Q4', [1, 0]) # Q4 starts at zero
    TOFF3_gate('Q1', 'Q2', 'Q3', 'Q4') # Q4 = AND of Q1, Q2, Q3
    print(''.join([measureQubit(q) for q in ['Q1', 'Q2', 'Q3', 'Q4']]), end=',')

print('')

def TOFFn_gate(ctl, result): # result = result XOR AND(qubits) 
    n = len(ctl)
    if n == 0: 
        applyGate(X_gate, result)
    if n == 1: 
        applyGate(CNOT_gate, ctl[0], result)
    elif n == 2: 
        applyGate(TOFF_gate, ctl[0], ctl[1], result)
    elif n > 2: 
        k = 0
        while 'temp' + str(k) in namestack: 
            k= k + 1
        temp = 'temp' + str(k) # Generate unique name 
        pushQubit(temp, [1, 0]) # Push zero temp qubit 
        applyGate(TOFF_gate, ctl[0], ctl[1], temp) # Apply TOFF 
        ctl.append(temp) # Add temp to controls 
        TOFFn_gate(ctl[2:], result)  # Recursion 
        applyGate(TOFF_gate, ctl[0], ctl[1], temp) # Uncompute temp 
        measureQubit(temp) # Pop temp

# Test
for i in range(20): # Generate truth table
    workspace = np.array([[1]], dtype=np.single) # Reset
    pushQubit('Q1', [1, 1])
    pushQubit('Q2', [1, 1])
    pushQubit('Q3', [1, 1])
    pushQubit('Q4', [1, 0]) # Q4 starts at zero, becomes AND of Q1, Q2, Q3
    TOFFn_gate(['Q1', 'Q2', 'Q3'], 'Q4')
    print(''.join([measureQubit(q) for q in ['Q1', 'Q2', 'Q3', 'Q4']]),end=',')

print('')

# Optimised applyGate
def applyGate(gate, *names):
    global workspace
    if list(names) != namestack[-len(names):]: # Reorder stack if necessary
        for name in names:
            tosQubit(name)
    workspace = np.reshape(workspace, (-1, 2**(len(names))))
    subworkspace = workspace[:, -gate.shape[0]:]
    np.matmul(subworkspace, gate.T, out=subworkspace)

def TOFF3_gate(q1,q2,q3,q4): 
    applyGate(X_gate,q1,q2,q3,q4)

def TOFFn_gate(ctl,result): 
    applyGate(X_gate,*ctl,result)
    
for i in range(20):
    workspace = np.array([[1]],dtype=np.single) # Reset
    pushQubit('Q1', [1, 1])
    pushQubit('Q2', [1, 1])
    pushQubit('Q3', [1, 1])
    pushQubit('Q4', [1, 0])
    
    TOFF3_gate('Q1', 'Q2', 'Q3', 'Q4')
    print(''.join([measureQubit(q) for q in ['Q1', 'Q2', 'Q3', 'Q4']]),end='/')
    pushQubit('Q1', [1, 1])
    pushQubit('Q2', [1, 1]) 
    pushQubit('Q3', [1, 1]) 
    pushQubit('Q4', [1, 0]) 
    TOFFn_gate(['Q1', 'Q2', 'Q3'], 'Q4') 
    print(''.join([measureQubit(q) for q in ['Q1', 'Q2', 'Q3', 'Q4']]),end=',')

##############
#            #
#   Part 2   #
#            #
##############

print('Part 2')

Z_gate = np.array([[1, 0],  # Pauli Z gate
                   [0,-1]])

def zero_booleanOracle(qubits, result): # All qubits zero? 
    # If all qubits==0 return 1 else return 0
    for qubit in qubits: # Negate all inputs
        applyGate(X_gate, qubit)
    TOFFn_gate(qubits, result) # Compute AND gate
    for qubit in qubits: # Restore inputs
        applyGate(X_gate, qubit)

def zero_phaseOracle(qubits): # All qubits zero? 
    # If all qubits==0 return -weight else return weight
    for qubit in qubits: # Negate all inputs
        applyGate(X_gate, qubit)
    applyGate(Z_gate, *namestack) # Controlled Z gate
    for qubit in qubits: # Restore inputs
        applyGate(X_gate, qubit)

def sample_phaseOracle(qubits): # Sample function
        # If all f(x)==1 return -weight else return weight
    applyGate(X_gate, qubits[1]) # Negate qubit 1
    applyGate(Z_gate, *namestack) # Controlled Z gate
    applyGate(X_gate, qubits[1]) # Restore qubit 1

def groverSearch(n, printProb=True):
    optimalTurns = int(np.pi / 4 * np.sqrt(2**n) - 1/2) # Iterations 
    qubits = list(range(n)) # Generate qubit names
    for qubit in qubits: # Initialize qubits
        pushQubit(qubit, [1, 1])
    for k in range(optimalTurns): # Grover iterations:
        sample_phaseOracle(qubits) # Apply phase oracle
        for qubit in qubits: # H-gate all qubits
            applyGate(H_gate, qubit) 
        zero_phaseOracle(qubits) # Apply 0 phase oracle
        for qubit in qubits: # H-gate all qubits
            applyGate(H_gate, qubit) 
        if printProb: # Peek probabilities
            print(probQubit(qubits[0])) # To show convergence
    for qubit in reversed(qubits): # Print result 
        print(measureQubit(qubit), end="")

workspace = np.array([[1.]]) # Initialize workspace
groverSearch(6) # (Only reals used here)
print('')

# Week 9
import torch as pt
pt.autograd.set_grad_enabled(False) # Disable autogradients
if pt.cuda.is_available(): # Check if GPU is available
    print("GPU available") 
else:
    print("Sorry, only CPU available")

def pushQubit(name, weights):
    global workspace
    global namestack
    if (workspace.shape[0], workspace.shape[1]) == (1, 1): 
        namestack = [] # Reset if workspace empty 
    namestack.append(name)
    weights = weights / np.linalg.norm(weights) # Normalize 
    weights = pt.tensor(weights, device=workspace.device, dtype=workspace[0,0 ].dtype)       
    workspace = pt.reshape(workspace,(1, -1))
    workspace = pt.kron(workspace, weights)               
    
def tosQubit(name):
    global workspace
    global namestack
    k = len(namestack) - namestack.index(name) # Position of qubit 
    if k > 1: # If non-trivial
        namestack.append(namestack.pop(-k))
        workspace = pt.reshape(workspace, (-1, 2, 2**(k-1)))      
        workspace = pt.swapaxes(workspace, -2, -1)              
        
def applyGate(gate, *names):
    global workspace
    if list(names) != namestack[-len(names):]: # Reorder stack
        for name in names: # If necessary 
            tosQubit(name)
    workspace = pt.reshape(workspace, (-1, 2**len(names)))      
    subworkspace = workspace[:, -gate.shape[0]:]
    gate = pt.tensor(gate.T, device=workspace.device, dtype=workspace[0, 0].dtype)               
    if workspace.device.type == 'cuda':                        
        pt.matmul(subworkspace, gate, out=subworkspace)         
    else: # Workaround for issue #114350 in torch.matmul 
        subworkspace[:, :]=pt.matmul(subworkspace, gate) 
        
def probQubit(name): # Check probabilities of qubit being 0 or 1
    global workspace
    tosQubit(name) # Qubit to TOS
    workspace = pt.reshape(workspace, (-1,2)) # To 2 cols
    prob = pt.linalg.norm(workspace, axis=0)**2 # Compute prob 
    prob = pt.Tensor.cpu(prob).numpy() # Convert to numpy
    return prob / prob.sum() # Make sure sum is one
    
def measureQubit(name): # Measure and pop qubit
    global workspace
    global namestack
    prob = probQubit(name) # Compute probabilities
    measurement = np.random.choice(2, p=prob) # 0 or 1 
    workspace = (workspace[:, [measurement]] /  np.sqrt(prob[measurement])) # Extrace col
    namestack.pop() # Pop stacks
    return measurement
    
import time
workspace = pt.tensor([[1.]],device=pt.device('cuda'), dtype=pt.float32) 
t = time.process_time() # With GPU
groverSearch(16, printProb=False) # Skip prob printouts 
print("\nWith GPU:", time.process_time() - t, "s")
workspace = pt.tensor([[1.]], device=pt.device('cpu'), dtype=pt.float32)
t = time.process_time() # With CPU
groverSearch(16, printProb=False) # Skip prob printouts 
print("\nWith CPU (single-core):", time.process_time() - t, "s")







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
    pushQubit('Q4', [1, 0]) # Q4 starts at zero, becomes ANd of Q1, Q2, Q3
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



import numpy as np
import random as rd
import sys, os
import qiskit.result.result as result

from qiskit import QuantumCircuit as qc
from qiskit import QuantumRegister as qr
from qiskit import ClassicalRegister as cr
from qiskit import execute, BasicAer, IBMQ

IBMQ.save_account('081a82898f9ef0dc8c984319f226a3638e039caa51f48da3c3a413207ec77897c2e18ca676494f03c20df0205edec51827bef2f91dcb5e6362bd9a0a27c639f2', overwrite=True)
provider = IBMQ.load_account()

class Alice:
    def prepare(self, n):
        qubits = int(4*n,)
        a = np.random.randint(2, size=qubits,)  #Generate a random data bits
        b = np.random.randint(2, size=qubits,)  #Generate a random data bits
        return (a, b)

class Bob:
    def genRandomBasis(self, n):
        qubits = int(4*n)
        b = np.random.randint(2, size=qubits,)  #Generate a random data bits
        return b

class Eve:
    def genRandomBasis(self, n):
        qubits = int(4*n)
        b = np.random.randint(2, size=qubits,)  #Generate a random data bits
        return b

def BB84(keylen):
    qubits = int(4*keylen)
    n = keylen

    qr_1 = qr(name="qr_1", size=qubits)         #Create a quantum register for Alice, Bob
    qr_2 = qr(name="qr_2", size=qubits)         #Create a quantum register for Eve
    cr_1 = cr(name="cr_1", size=qubits)         #Create a classical register for Alice, Bob
    cr_2 = cr(name="cr_2", size=qubits)         #Create a classical register for Eve
    bbCir_1 = qc(qr_1, cr_1, name="BB84")       #Connect the Circuit for Alice, Bob
    bbCir_2 = qc(qr_2, cr_2, name="BB84_Eve")   #Connect the circuit for Eve

    #Initialize
    alice = Alice()
    bob = Bob()
    eve = Eve()

    (a,b)=alice.prepare(n)

    for i in range(qubits):
        if a[i] == 1:
            bbCir_1.x(qr_1[i])                 
            bbCir_2.x(qr_2[i])                 

    for i in range(qubits):
        if b[i] == 1:
            bbCir_1.h(qr_1[i])                  
            bbCir_2.h(qr_2[i])                  

    b_bob = bob.genRandomBasis(n)               
    b_eve = eve.genRandomBasis(n)               

    for i in range(qubits):
        if b_eve[i] == 1:
            bbCir_2.h(qr_2[i])                  

    for i in range(qubits):
        if b_bob[i] == 1:
            bbCir_1.h(qr_1[i])                  
            bbCir_2.h(qr_2[i])                  

    for i in range(qubits):
        bbCir_1.measure(qr_1[i], cr_1[i])       #Measure 
        bbCir_2.measure(qr_2[i], cr_2[i])

    return(bbCir_1, bbCir_2, a, b, b_bob)

def detKey(a, bob_measure, b, b_bob, n):
    x = [b_i == b_bob_i for (b_i, b_bob_i) in zip(b, b_bob)]

    if np.sum(x) < n/2:
        return False
    else:
        alice_bits = a[x]
        bob_bits = bob_measure[x]

        check_alice = alice_bits[:n]
        check_bob = bob_bits[:n]
        key_alice = alice_bits[-n:]
        key_bob = bob_bits[-n:]

        mismatch = [k_a != k_b for (k_a, k_b) in zip(key_alice, key_bob)]
        error = (np.sum(mismatch)) / n

        epsilon = 0.05
        if error > epsilon:
            return False
        else:
            return True

def testCir(shots, keylen):
    global provider
    backend = provider.backend.ibmq_qasm_simulator
    bbCir_1, bbCir_2, a, b, b_bob = BB84(keylen)

    job_bob = execute(bbCir_1, backend = backend, timeout = 600, shots = shots)
    results_bob = job_bob.result()
    answer_bob = results_bob.get_counts()

    job_eve = execute(bbCir_2, backend = backend, timeout = 600, shots = shots)
    results_eve = job_eve.result()
    answer_eve = results_eve.get_counts()

    return (answer_bob, answer_eve, a, b, b_bob)

def getMeasurement(answer, answer_eve):
    bob_measure = list(rd.choice(list(answer.keys())))
    bob_measure = list(map(int, bob_measure))
    bob_measure = np.array(bob_measure)
    bob_measure = bob_measure[::-1]
    
    bob_measure_eve = list(rd.choice(list(answer_eve.keys())))
    bob_measure_eve = list(map(int, bob_measure_eve))
    bob_measure_eve = np.array(bob_measure_eve)
    bob_measure_eve = bob_measure_eve[::-1]
    
    return (bob_measure, bob_measure_eve)      

keylen = 2

for n in [2, 4, 8]:
    succ = 0
    succ_eve = 0
    int_range = 20

    for i in range(int_range):
        (answer, answer_eve, a, b, b_bob) = testCir(1, n)
        bob_measure, bob_measure_eve = getMeasurement(answer, answer_eve)
        res = detKey(a, bob_measure, b, b_bob, keylen)
        # successful
        if res: 
            succ += 1
        res = detKey(a, bob_measure_eve, b, b_bob, keylen)
        # successful
        if res:                                     
            succ_eve += 1
        
    p = (succ_eve) / (int_range)
    print(f"Key Length: {n}, Total Round: {int_range}, Percent of detecting eavesdropping: {p},\n")
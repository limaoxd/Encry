import numpy as np
import random as rd
import sys, os
import matplotlib.pyplot as plt
import qiskit.result.result as result

from qiskit import QuantumCircuit as qc
from qiskit import QuantumRegister as qr
from qiskit import ClassicalRegister as cr
from qiskit import execute, BasicAer, IBMQ

class Alice:
    def prepare(self, n):
        qubits = int(4*n,)
        a = np.random.randint(2, size=qubits,) #Generate a random data bits
        b = np.random.randint(2, size=qubits,) #Generate a random data bits
        print("Alice has generated random secret data bits, a: ", a)
        print("She will arbitrarily encode these bits using random basis, b: ", b)
        return (a, b)

class Bob:
    def genRandomBasis(self, n):
        qubits = int(4*n)
        b = np.random.randint(2, size=qubits,)
        print("Bob has chosen random basis, b': ", b)
        return b

class Eve:
    def genRandomBasis(self, n):
        qubits = int(4*n)
        b = np.random.randint(2, size=qubits,)
        print("Eve has chosen random basis: ", b)
        return b

def BB84(keylen):
    qubits = int(4*keylen)

    n = keylen

    qr_1 = qr(name="qr_1", size=qubits)
    qr_2 = qr(name="qr_2", size=qubits)
    cr_1 = cr(name="cr_1", size=qubits)
    cr_2 = cr(name="cr_2", size=qubits)
    bbCir_1 = qc(qr_1, cr_1, name="BB84")
    bbCir_2 = qc(qr_2, cr_2, name="BB84_Eve")

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
        bbCir_1.measure(qr_1[i], cr_1[i])
        bbCir_2.measure(qr_2[i], cr_2[i])

    return(bbCir_1, bbCir_2, a, b, b_bob)

def detKey(a, bob_measure, b, b_bob, n):
    x = [b_i == b_bob_i for (b_i, b_bob_i) in zip(b, b_bob)]

    if np.sum(x) < n/2:
        print("Aborting protocol. Less than 2n bits match between b and b'")
        return False
    else:
        alice_bits = a[x]
        bob_bits = bob_measure[x]

        check_alice = alice_bits[:n]
        check_bob = bob_bits[:n]
        key_alice = alice_bits[-n:]
        key_bob = bob_bits[-n:]

        print("Alice has check bits: ", check_alice)
        print("Bob has check bits: ", check_bob)

        mismatch = [k_a != k_b for (k_a, k_b) in zip(key_alice, key_bob)]
        error = (np.sum(mismatch)) / n

        epsilon = 0.05
        if error > epsilon:
            print("Interference detected... Aborting!")
            return False
        else:
            print("Successfully exchanged private key: ", key_alice)
            return True

def testCir(shots, keylen):
    IBMQ.delete_account
    IBMQ.save_account('c81d9c6500e2c272304612ae6d7c87be2686aaecb71a8e9bdfc94d2ae39d6acce3be3ea7842776f7f0dee71555f89f0ccb4f77f4ec087e08ac719ce720d9386a', overwrite=True)
    provider = IBMQ.load_account()
    backend = provider.backend.ibmq_qasm_simulator
    bbCir_1, bbCir_2, a, b, b_bob = BB84(keylen)

    print("Simulating...")

    job_bob = execute(bbCir_1, backend = backend, timeout = 600, shots = shots)
    results_bob = job_bob.result()
    answer_bob = results_bob.get_counts()

    job_eve = execute(bbCir_2, backend = backend, timeout = 600, shots = shots)
    results_eve = job_eve.result()
    answer_eve = results_eve.get_counts()

    return (answer_bob, answer_eve, a, b, b_bob)

keylen = 4
(answer_bob, answer_eve, a, b, b_bob) = testCir(2**10, keylen)
print("Discrete distribution of potential measurements by Bob: ", answer_bob)
print("Discrete distribution of potential measurements by Bob with eavesdropping: ", answer_eve)

def getMeasurement(answer, answer_eve):
    bob_measure = list(rd.choice(list(answer.keys())))
    bob_measure = list(map(int, bob_measure))
    bob_measure = np.array(bob_measure)
    bob_measure = bob_measure[::-1]
    print("Bob's measurement result: ", bob_measure)
    
    bob_measure_eve = list(rd.choice(list(answer_eve.keys())))
    bob_measure_eve = list(map(int, bob_measure_eve))
    bob_measure_eve = np.array(bob_measure_eve)
    bob_measure_eve = bob_measure_eve[::-1]
    print("Bob's measurement result with eavesdropping: ", bob_measure_eve)
    
    return (bob_measure, bob_measure_eve)
    
bob_measure, bob_measure_eve = getMeasurement(answer_bob, answer_eve)

print("Key Exchange without eavesdropping: ")
result = detKey(a, bob_measure, b, b_bob, keylen)
print("Key Exchange with eavesdropping: ")
result = detKey(a, bob_measure_eve, b, b_bob, keylen)        

def plot_eve_undetected(probs):
    
    objects = ('2', '4', '8')
    y_pos = np.arange(len(objects))
    
    plt.bar(y_pos, probs, align='center', alpha=0.5)
    plt.xticks(y_pos, objects)
    
    plt.xlabel("Key Length")
    plt.ylabel('Probability')
    plt.title('Probability of Undetected Eavesdropping')
    
    plt.show()

probs = []

for n in [2, 4, 8]:
    succ = 0
    succ_eve = 0
    for i in range(2**7):
        print("Simulation round:", i)
        # Simulate one measurement with key length n = 4
        (answer, answer_eve, a, b, b_bob) = testCir(1, n)
        bob_measure, bob_measure_eve = getMeasurement(answer, answer_eve)
        print("Key Exchange without eavesdropping: ")
        res = detKey(a, bob_measure, b, b_bob, keylen)
        # successful
        if res: 
            succ += 1
        print("Key Exchange with eavesdropping: ")
        res = detKey(a, bob_measure_eve, b, b_bob, keylen)
        # successful
        if res: 
            succ_eve += 1
        
    print("n=", n)
    print("Successful key exchanges without Eve present: ", succ)
    print("Successful key exchanges with Eve present: ", succ_eve)
    p = (succ_eve) / (2**7)
    probs.append(p)
    
plot_eve()
plot_eve_undetected(probs)


















    

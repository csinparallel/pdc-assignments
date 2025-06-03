# circuitSolver-16.py solves the Circuit Satisfiability
#  Problem using a brute-force sequential solution.
#
# The particular circuit being tested is "wired" into the
# logic of function 'checkCircuit'. All combinations of
# inputs that satisfy the circuit are printed.
#
# 16-bit version by Michael J. Quinn, Sept 2002.
# Extended to use mpi4py by Anwesha Pradhananga, July 2024.
#
# Usage: mpirun -np N [-machinefile hosts] python circuitSolver-16.py

import time  # timing functions
from mpi4py import MPI

# Define the size of the bit vector
SIZE = 16

def extract_bit(n, i):
    return (n & (1 << i)) >> i

# Function to check the circuit
def check_circuit(id, bits):
    v = [extract_bit(bits, i) for i in range(SIZE)]

    if (
        (v[0] or v[1]) and (not v[1] or not v[3]) and (v[2] or v[3])
        and (not v[3] or not v[4]) and (v[4] or not v[5])
        and (v[5] or not v[6]) and (v[5] or v[6])
        and (v[6] or not v[15]) and (v[7] or not v[8])
        and (not v[7] or not v[13]) and (v[8] or v[9])
        and (v[8] or not v[9]) and (not v[9] or not v[10])
        and (v[9] or v[11]) and (v[10] or v[11])
        and (v[12] or v[13]) and (v[13] or not v[14])
        and (v[14] or v[15])
   ):
      print(
            f"{id}) {''.join(str(bit) for bit in reversed(v))}"
         , flush=True
        )
      return 1
    else:
        return 0

def main():
   id = 0   # process id
   num_threads = 1  # number of processes
   totalCount = 0  # total count of solutions

   search_space = 2**32

   print(f"\nChecking the circuit with {num_threads} processes using inputs 0..{search_space - 1}...\n", flush = True)
   
   start_time = time.time()

   # Each process checks a portion of the input space
   for i in range(id, 2**SIZE, num_threads):
      totalCount += check_circuit(id, i)

   totalTime = time.time() - start_time
   print(f"\n{num_threads} processes found {totalCount} solutions in {totalTime} secs.\n")

if __name__ == "__main__":
    main()


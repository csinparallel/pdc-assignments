# circuitSolver-32.py solves the Circuit Satisfiability
#  Problem using a brute-force sequential solution.
#
# The particular circuit being tested is "wired" into the
# logic of function 'checkCircuit'. All combinations of
# inputs that satisfy the circuit are printed.
#
# 16-bit version by Michael J. Quinn, Sept 2002.
# Extended to 32 bits by Joel C. Adams, Sept 2013.
# Extended to use mpi4py by Anwesha Pradhananga, July 2024.
#
# NOTE: This python program will take an unreasonable amount of time unless it is run with many processes.
#       If you would like to see how the program runs sequentially, I would recommend using the 16-bit version. 
#
# Usage: mpirun -np N [-machinefile hosts] python circuitSolver-32.py

import time  # timing functions
from mpi4py import MPI

# Define the size of the bit vector
SIZE = 32

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
    ) and (
        (v[16] or v[17]) and (not v[17] or not v[19]) and (v[18] or v[19])
        and (not v[19] or not v[20]) and (v[20] or not v[21])
        and (v[21] or not v[22]) and (v[21] or v[22])
        and (v[22] or not v[31]) and (v[23] or not v[24])
        and (not v[23] or not v[29]) and (v[24] or v[25])
        and (v[24] or not v[25]) and (not v[25] or not v[26])
        and (v[25] or v[27]) and (v[26] or v[27])
        and (v[28] or v[29]) and (v[29] or not v[30])
        and (v[30] or v[31])
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


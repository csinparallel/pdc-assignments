/** circuitSolverCUDA.c solves the Circuit Satisfiability
 *  Problem using a CUDA solution in GPU.
 *
 *   The particular circuit being tested is "wired" into the
 *   logic of function 'checkCircuit'. All combinations of
 *   inputs that satisfy the circuit are printed.
 *
 *   16-bit version by Michael J. Quinn, Sept 2002.
 *   Extended to 32 bits by Joel C. Adams, Sept 2013.
 *
 * Usage: ./circuitSolver
 *
 * HINT: for this problem, it may help to use CUDA's atomic operations.
 *          There are limitations on this based on your CUDA version and GPU compute capability (https://docs.nvidia.com/cuda/cuda-c-programming-guide/)
 *          - Devices with compute capability less than 6.0 only support device-wide atomic operations,
 *          - Tegra devices with compute capability less then 7.2 do not support system-wide atomic operations.
 *          - CUDA 12.8 and later support CUDA compiler builtin functions for atomic operations with memory order
 *              and thread scope.
 */

#include <iostream>
#include <limits.h>
#include <cuda.h>
#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

/* checkCircuit() checks the circuit for a given input.
 * parameters: id, the id of the process checking;
 *             bits, the (long) rep. of the input being checked.
 *
 * output: the binary rep. of bits if the circuit outputs 1
 * return: 1 if the circuit outputs 1; 0 otherwise.
 */
int checkCircuit(int id, long bits);


// Function to check for CUDA errors
__host__ void checkErr(cudaError_t err, const char* msg) {
    if (err != cudaSuccess) {
        fprintf(stderr, "\n*** %s (error code %d: '%s')!\n\n",
                         msg, err, cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {

    printf("\nChecking the circuit using inputs 0..%u...\n", UINT_MAX);

    unsigned int count = 0;
    int id = 0;

    TimePoint startTime = std::chrono::high_resolution_clock::now();

    for (unsigned long i = 0; i <= UINT_MAX; ++i) {
        count += checkCircuit(id, i);
    }

    TimePoint endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = endTime - startTime;

    printf("\n%u threads found %d solutions in %f secs.\n\n",
           1, count, totalTime.count());

    return 0;
}


/* EXTRACT_BIT is a macro that extracts the ith bit of number n.
 *
 * parameters: n, a number;
 *             i, the position of the bit we want to know.
 *
 * return: 1 if 'i'th bit of 'n' is 1; 0 otherwise 
 */

#define EXTRACT_BIT(n,i) ( (n & (1<<i) ) ? 1 : 0)

/* checkCircuit() checks the circuit for a given input.
 * parameters: id, the id of the process checking;
 *             bits, the (long) rep. of the input being checked.
 *
 * output: the binary rep. of bits if the circuit outputs 1
 * return: 1 if the circuit outputs 1; 0 otherwise.
 */

#define SIZE 32

int checkCircuit(int id, long bits) {
    int v[SIZE];        /* Each element is one of the 32 bits */

    for (int i = 0; i < SIZE; i++) {
      v[i] = EXTRACT_BIT(bits,i);
    }
 
    if (((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
        && (!v[3] || !v[4]) && (v[4] || !v[5])
        && (v[5] || !v[6]) && (v[5] || v[6])
        && (v[6] || !v[15]) && (v[7] || !v[8])
        && (!v[7] || !v[13]) && (v[8] || v[9])
        && (v[8] || !v[9]) && (!v[9] || !v[10])
        && (v[9] || v[11]) && (v[10] || v[11])
        && (v[12] || v[13]) && (v[13] || !v[14])
        && (v[14] || v[15]) )
        &&
       ((v[16] || v[17]) && (!v[17] || !v[19]) && (v[18] || v[19])
        && (!v[19] || !v[20]) && (v[20] || !v[21])
        && (v[21] || !v[22]) && (v[21] || v[22])
        && (v[22] || !v[31]) && (v[23] || !v[24])
        && (!v[23] || !v[29]) && (v[24] || v[25])
        && (v[24] || !v[25]) && (!v[25] || !v[26])
        && (v[25] || v[27]) && (v[26] || v[27])
        && (v[28] || v[29]) && (v[29] || !v[30])
        && (v[30] || v[31]) ) )
    {
       printf ("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n",
          v[31],v[30],v[29],v[28],v[27],v[26],v[25],v[24],v[23],v[22],
          v[21],v[20],v[19],v[18],v[17],v[16],v[15],v[14],v[13],v[12],
          v[11],v[10],v[9],v[8],v[7],v[6],v[5],v[4],v[3],v[2],v[1],v[0]);
       return 1;
    } else {
       return 0;
    }
}
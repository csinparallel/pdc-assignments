/* circuitSolverCpp11.c solves the Circuit Satisfiability
 *  Problem using a C++11 solution using the threads approach.
 *
 *   The particular circuit being tested is "wired" into the
 *   logic of function 'checkCircuit'. All combinations of
 *   inputs that satisfy the circuit are printed.
 *
 *   16-bit version by Michael J. Quinn, Sept 2002.
 *   Extended to 32 bits by Joel C. Adams, Sept 2013.
 *
 * Usage: ./circuitSolverCpp11
 *
 */

#include <iostream>
#include <limits.h>
#include <thread>
#include <vector> // used to make a vector of threads
#include <chrono>

using std::thread; 
using std::vector;
using std::cout;

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

/* checkCircuit() checks the circuit for a given input.
 * parameters: id, the id of the process checking;
 *             bits, the (long) rep. of the input being checked.
 *
 * output: the binary rep. of bits if the circuit outputs 1
 * return: 1 if the circuit outputs 1; 0 otherwise.
 */
int checkCircuit(int id, long bits);

int main(int argc, char* argv[]) {
    int numThreads = 1;
    int id = 0;
    unsigned int count = 0;

    cout << "\nChecking the circuit with " << numThreads << " thread" 
              << (numThreads == 1 ? "" : "s") << " using inputs 0.."
              << UINT_MAX << "...\n";

    TimePoint startTime = std::chrono::high_resolution_clock::now();

    for (long i = 0; i <= UINT_MAX; ++i) {
        count += checkCircuit (id, i);
    }

    TimePoint endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = endTime - startTime;

    cout << "\n" << numThreads << " thread" << (numThreads == 1 ? "" : "s") 
              << " found " << total_count << " solutions in " 
              << totalTime.count() << " secs.\n\n";

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
    int v[SIZE];  /* Each element is one of the 32 bits */

    for (long i = 0; i < SIZE; i++) {
        v[i] = EXTRACT_BIT(bits, i);
    }

    if (((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
        && (!v[3] || !v[4]) && (v[4] || !v[5])
        && (v[5] || !v[6]) && (v[5] || v[6])
        && (v[6] || !v[15]) && (v[7] || !v[8])
        && (!v[7] || !v[13]) && (v[8] || v[9])
        && (v[8] || !v[9]) && (!v[9] || !v[10])
        && (v[9] || v[11]) && (v[10] || v[11])
        && (v[12] || v[13]) && (v[13] || !v[14])
        && (v[14] || v[15]))
        &&
        ((v[16] || v[17]) && (!v[17] || !v[19]) && (v[18] || v[19])
        && (!v[19] || !v[20]) && (v[20] || !v[21])
        && (v[21] || !v[22]) && (v[21] || v[22])
        && (v[22] || !v[31]) && (v[23] || !v[24])
        && (!v[23] || !v[29]) && (v[24] || v[25])
        && (v[24] || !v[25]) && (!v[25] || !v[26])
        && (v[25] || v[27]) && (v[26] || v[27])
        && (v[28] || v[29]) && (v[29] || !v[30])
        && (v[30] || v[31]))) {
        std::cout << id << ") ";
        for (int i = SIZE - 1; i >= 0; i--) {
            std::cout << v[i];
        }
        std::cout << std::endl;
        return 1;
    } else {
        return 0;
    }
}

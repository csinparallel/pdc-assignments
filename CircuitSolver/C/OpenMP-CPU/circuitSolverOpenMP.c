/* circuitSolverOpenMP.c solves the Circuit Satisfiability
 *  Problem using an OpenMP+C solution.
 *
 *   The particular circuit being tested is "wired" into the
 *   logic of function 'checkCircuit'. All combinations of
 *   inputs that satisfy the circuit are printed.
 *
 *   16-bit version by Michael J. Quinn, Sept 2002.
 *   Extended to 32 bits by Joel C. Adams, Sept 2013.
 *
 * Usage: ./circuitSolverOpenMP
 *
 */

#include <stdio.h>    
#include <limits.h> 
#include <omp.h>       // OpenMP directives
#include <ctype.h>     // isdigit
#include <stdlib.h>   // atoi

int checkCircuit (int id, unsigned long bits);
int processCommandLine(int argc, char** argv);

int main(int argc, char *argv[]) {
   int numThreads    = processCommandLine(argc, argv);  // number of threads (defaults to 1 if no arguments provided)
   int total_count = 0;  // total number of solutions

   printf("\nChecking the circuit with %d thread%s using inputs 0..%u...\n",
          numThreads, (numThreads == 1) ? "" : "s", UINT_MAX);

   double startTime = omp_get_wtime();

   #pragma omp parallel for reduction(+:total_count) num_threads(numThreads) schedule(dynamic)
   for (unsigned long i = 0; i < UINT_MAX; i++) {
      total_count += checkCircuit(omp_get_thread_num(), i);
   }

   double totalTime = omp_get_wtime() - startTime;

   printf("\n%d thread%s found %d solutions in %f secs.\n\n",
          numThreads, (numThreads == 1) ? "" : "s", total_count, totalTime);

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

int checkCircuit (int id, unsigned long bits) {
   int v[SIZE];        /* Each element is one of the 32 bits */

   for (unsigned long i = 0; i < SIZE; i++) {
     v[i] = EXTRACT_BIT(bits,i);
   }

   if ( ( (v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
       && (!v[3] || !v[4]) && (v[4] || !v[5])
       && (v[5] || !v[6]) && (v[5] || v[6])
       && (v[6] || !v[15]) && (v[7] || !v[8])
       && (!v[7] || !v[13]) && (v[8] || v[9])
       && (v[8] || !v[9]) && (!v[9] || !v[10])
       && (v[9] || v[11]) && (v[10] || v[11])
       && (v[12] || v[13]) && (v[13] || !v[14])
       && (v[14] || v[15]) )
       &&
          ( (v[16] || v[17]) && (!v[17] || !v[19]) && (v[18] || v[19])
       && (!v[19] || !v[20]) && (v[20] || !v[21])
       && (v[21] || !v[22]) && (v[21] || v[22])
       && (v[22] || !v[31]) && (v[23] || !v[24])
       && (!v[23] || !v[29]) && (v[24] || v[25])
       && (v[24] || !v[25]) && (!v[25] || !v[26])
       && (v[25] || v[27]) && (v[26] || v[27])
       && (v[28] || v[29]) && (v[29] || !v[30])
       && (v[30] || v[31]) ) )
   {
      printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
         v[31],v[30],v[29],v[28],v[27],v[26],v[25],v[24],v[23],v[22],
         v[21],v[20],v[19],v[18],v[17],v[16],v[15],v[14],v[13],v[12],
         v[11],v[10],v[9],v[8],v[7],v[6],v[5],v[4],v[3],v[2],v[1],v[0]);
      fflush (stdout);
      return 1;
   } else {
      return 0;
   }
}


/* processCommandLine() checks for command line arguments
 */
int processCommandLine(int argc, char** argv) {
      if (argc > 1 && isdigit(argv[1][0])) {
         return atoi(argv[1]);
      } else {
         fprintf(stderr, "\n*** Usage: %s [numThreads], defaulting to 1 thread\n\n", argv[0]);
         return 1;
      }
}
/** circuitSolver.c solves the Circuit Satisfiability
  * Problem using a brute-force sequential solution.
  *
  * The particular circuit being tested is "wired" into the
  * logic of function 'checkCircuit'. All combinations of
  * inputs that satisfy the circuit are printed.
  * 
  * 16-bit version by Michael J. Quinn, Sept 2002.
  * Extended to 32 bits by Joel C. Adams, Sept 2013.
  * Extended to use OpenMP with the GPU to paralleize 
  * the solution by Anwesha Pradhananga July 2024.
  *
  * Usage: ./circuitSolver 
  */ 

#include <stdlib.h>   // Include for atoi
#include <stdio.h>  
#include <ctype.h>
#include <omp.h>       // OpenMP directives
#include <inttypes.h>

int checkCircuit (uintmax_t);

int main() {
   int total_count = 0;  // total number of solutions
   uint64_t upper_limit = UINT32_MAX;
   printf("\nChecking the circuit using inputs 0..%lu...\n",
         upper_limit-1);

   double startTime = omp_get_wtime();

   for (uint64_t i = 0; i < upper_limit; i++) {
      total_count += checkCircuit(i);
   }

   double totalTime = omp_get_wtime() - startTime;

   printf("Found %d solutions in %f secs.\n\n",
           total_count, totalTime);
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

int checkCircuit (uint64_t bits) {
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

/* parallelLoopChunks.h
 *
 *  ... defines getChunkStartStopValues() to set up a parallel loop.
 *  A process/thread can call it to compute the start and stop values
 *  of a contiguous 'chunk' of the loop's iterations,
 *  and any two processes'/threads' chunk-sizes will differ by at most 1.
 *
 *  This header file makes the function easier to reuse.
 *
 * Joel Adams, Calvin University, Fall 2023.
 *
 */

#include <stdio.h>  // printf()
#include <stdlib.h> // exit()
#include <math.h>   // ceil()

// set to 1 if this file is being used for MPI -- to run finalize when encountering errors
#define IS_MPI 0 

/* Calculate the start and stop values for this process's/thread's 
 *  contiguous chunk of a set of loop-iterations, 0..REPS-1,
 *  so that each process/thread chunk-sizes are equal (or nearly so).
 *
 * @param: id, an int containing this process's/thread's rank
 * @param: numProcs, an int containing the number of processes/threads
 * @param: REPS, a const unsigned containing the for loop's iteration total
 * Precondition: id == this process's/thread's rank
 *            && numProcs == the number of processes/threads
 *            && REPS == the total number of 0-based loop iterations needed
 *            && numProcs <= REPS 
 *            && REPS < 2^32
 * @param: start, the address of the unsigned variable through which the 
 *          starting value of this process's/thread's chunk should be returned
 * @param: stop, the address of the unsigned variable through which the
 *          stopping value of this process's/thread's chunk should be returned
 * Postcondition: *start = this process's/thread's first iteration value 
 *             && *stop = this process's/thread's last iteration value + 1.
 */
void getChunkStartStopValues(int id, int numProcs, const unsigned REPS,
                              unsigned &start, unsigned &stop)
{
   // check precondition before proceeding
   if ((unsigned)numProcs > REPS) {
      if (id == 0) {
         printf("\n*** Number of processes/threads (%u) exceeds REPS (%u)\n",
                 numProcs, REPS);
         printf("*** Please run with -np (number of processes/threads) less than or equal to %u\n\n", REPS);
      }
      #if IS_MPI
         MPI_Finalize();
      #endif
      exit(1);
   }

   // compute the chunk size that works in many cases
   unsigned chunkSize1 = (long)ceil(((double)REPS) / numProcs);
   unsigned begin = id * chunkSize1;
   unsigned end = begin + chunkSize1;
   // see if there are any leftover iterations
   unsigned remainder = REPS % numProcs;
   // If remainder == 0, chunkSize1 = chunk-size for all processes/threads;
   // If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
   //   but for processes/threads p_remainder..p_numProcs-1
   //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
   if (remainder > 0 && (unsigned)id >= remainder) {
     unsigned chunkSize2 = chunkSize1 - 1;
     unsigned remainderBase = remainder * chunkSize1;
     unsigned processOffset = (id-remainder) * chunkSize2;
     begin = remainderBase + processOffset;
     end = begin + chunkSize2;
   } 
   // pass back this proc's/thread's begin and end values via start and stop
   start = begin;
   stop = end;
}


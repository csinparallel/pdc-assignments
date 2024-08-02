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
                              unsigned* start, unsigned* stop);


/* CircuitSolver.java solves the Circuit Satisfiability
 *  Problem using a brute-force sequential solution.
 *
 *   The particular circuit being tested is "wired" into the
 *   logic of function 'checkCircuit'. All combinations of
 *   inputs that satisfy the circuit are printed.
 *
 *   16-bit version by Michael J. Quinn, Sept 2002.
 *   Extended to 32 bits by Joel C. Adams, Sept 2013.
 *
 * Usage: mpirun -np N [-machinefile hosts] java CircuitSolver
 */

import mpi.*;
import java.nio.IntBuffer;

public class CircuitSolver extends Object {

 public static void main (String args[]) throws MPIException {

   MPI.Init(args);

   int id = 0;
   int numProcs = 1;
   int  totalCount = 0;
   long UINT_MAX   = (Long.valueOf(Integer.MAX_VALUE) + 1) * 2 - 1;  // 2^32 - 1

   System.out.printf("\nChecking the circuit with %d process%s using inputs 0..%d...\n", 
                        numProcs, (numProcs == 1) ? "" : "es", UINT_MAX);

   double startTime = 0.0, totalTime = 0.0;
   startTime = MPI.wtime();

   for (long i = 0; i < UINT_MAX; ++i) {
      totalCount += checkCircuit (id, i);
   }

   totalTime = MPI.wtime() - startTime;
   System.out.printf("\n%d process%s found %d solutions in time %f secs.\n\n",
                        numProcs, (numProcs == 1) ? "" : "es",
                        totalCount, totalTime);

   MPI.Finalize();
 }

/* StartAndStop
 * An object that contains a "start" and "stop" long. 
 * Designed for use with getChunkStartStopValues().
 * Includes a constructor along with getters/setters for each variable.
*/
 private static class StartAndStop {
   private long start;
   private long stop;

   public StartAndStop()   { start = 0; stop = 0; }
   public long getStart()  { return start; }
   public long getStop()   { return stop; }
   
   public void setStart(long val)   { start = val; }
   public void setStop(long val)    { stop = val; }
 }

/* checkCircuit() checks the circuit for a given input.
 * parameters: id, the id of the process checking;
 *             bits, the (long) rep. of the input being checked.
 *
 * output: the binary rep. of bits if the circuit outputs 1
 * return: 1 if the circuit outputs 1; 0 otherwise.
 */

 private final static int SIZE = 32;

 private static int checkCircuit (int id, long bits) {
   boolean v[] = new boolean[SIZE];    /* Each element is one of the 32 bits */

   for (int i = 0; i < SIZE; ++i) {
     v[i] = ( ((bits & (1 << i)) > 0 ) ? true : false );
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
      System.out.printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n",
         id,
         (v[31] ? 1 : 0),
         (v[30] ? 1 : 0),
         (v[29] ? 1 : 0),
         (v[28] ? 1 : 0),
         (v[27] ? 1 : 0),
         (v[26] ? 1 : 0),
         (v[25] ? 1 : 0),
         (v[24] ? 1 : 0),
         (v[23] ? 1 : 0),
         (v[22] ? 1 : 0),
         (v[21] ? 1 : 0),
         (v[20] ? 1 : 0),
         (v[19] ? 1 : 0),
         (v[18] ? 1 : 0),
         (v[17] ? 1 : 0),
         (v[16] ? 1 : 0),
         (v[15] ? 1 : 0),
         (v[14] ? 1 : 0),
         (v[13] ? 1 : 0),
         (v[12] ? 1 : 0),
         (v[11] ? 1 : 0),
         (v[10] ? 1 : 0),
         (v[9] ? 1 : 0),
         (v[8] ? 1 : 0),
         (v[7] ? 1 : 0),
         (v[6] ? 1 : 0),
         (v[5] ? 1 : 0),
         (v[4] ? 1 : 0),
         (v[3] ? 1 : 0),
         (v[2] ? 1 : 0),
         (v[1] ? 1 : 0),
         (v[0] ? 1 : 0) );
      System.out.flush();
      return 1;
   } else {
      return 0;
   }
 }

 /* Calculate the start and stop values for this MPI process's
 *  contiguous chunk of a set of loop-iterations, 0..REPS-1,
 *  so that each process chunk-sizes are equal (or nearly so).
 *
 * @param: id, an int containing this process's MPI rank
 * @param: numProcs, an int containing the number of processes
 * @param: REPS, a const unsigned containing the for loop's iteration total
 * Precondition: id == this process's MPI rank
 *            && numProcs == the number of MPI processes
 *            && REPS == the total number of 0-based loop iterations needed
 *            && numProcs <= REPS
 *            && REPS < 2^32
 * @param: start, the address of the unsigned variable through which the
 *          starting value of this process's chunk should be returned
 * @param: stop, the address of the unsigned variable through which the
 *          stopping value of this process's chunk should be returned
 * Postcondition: *start = this process's first iteration value
 *             && *stop = this process's last iteration value + 1.
 */
private static void getChunkStartStopValues(int id, int numProcs,
		final long REPS, StartAndStop result) throws MPIException
{
   // check precondition before proceeding
   if (numProcs > REPS) {
      if (id == 0) {
         System.out.printf("\n*** Number of MPI processes (%u) exceeds REPS (%lu)\n",
                 numProcs, REPS);
         System.out.printf("*** Please run with -np less than or equal to %lu\n\n", REPS);
      }
      System.exit(1);
   }

   // compute the chunk size that works in many cases
   long chunkSize1 = (long)Math.ceil(((double)REPS) / numProcs);
   long begin = id * chunkSize1;
   long end = begin + chunkSize1;
   // see if there are any leftover iterations
   long remainder = REPS % numProcs;
   // If remainder == 0, chunkSize1 = chunk-size for all processes;
   // If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
   //   but for processes p_remainder..p_numProcs-1
   //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
   if (remainder > 0 && id >= remainder) {
     long chunkSize2 = chunkSize1 - 1;
     long remainderBase = remainder * chunkSize1;
     long processOffset = (id-remainder) * chunkSize2;
     begin = remainderBase + processOffset;
     end = begin + chunkSize2;
   }
   // pass back this proc's begin and end values via start and stop
   result.setStart(begin);
   result.setStop(end);
}
}


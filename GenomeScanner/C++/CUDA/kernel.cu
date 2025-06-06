/** kernel.cpp defines the kernel method for scanning the genome
 * 
 * It includes functions for:
 * - compare_substrings(): a function for comparing two substrings on the device 
 * - launch_scan(): Defines a function for interfacing with a CUDA kernel
 *      (to be defined in this file) from files compiled with a non-CUDA compiler
 *
 * @author: Steven McKelvey, Calvin University, January 2025
 * 
 * HINT: the GPU's memory may not be able to hold the entire genome and an array of indices.
 *          Keep this memory constraint in mind when writing code.
 */

 #include <cuda.h>
 #include <stdio.h>
 #include "indicesVecType.h" 
 
 __host__ void checkErr(cudaError_t err, const char *msg);
 __device__ bool compare_substrings(const char* str1, const char* str2, int length);
 extern "C" void launch_scan(const char* genome, indicesVecType* indices, const char* subSequence, long subSeqSize, long numCharsInFile);
 
 __host__ void checkErr(cudaError_t err, const char *msg)
 {
     if (err != cudaSuccess)
     {
         fprintf(stderr, "\n*** %s (error code %d: '%s')!\n\n",
                 msg, err, cudaGetErrorString(err));
         exit(EXIT_FAILURE);
     }
 }
 

void getChunkStartStopValues(long& start, long& stop, int chunkNum, int totalChunks, long numCharsInFile){
   long chunkSize1 = numCharsInFile / totalChunks;
   long begin = chunkNum * chunkSize1;
   long end = begin + chunkSize1;
   unsigned remainder = numCharsInFile % totalChunks;
   if (remainder > 0 && (unsigned) chunkNum >= remainder) {
     unsigned chunkSize2 = chunkSize1 - 1;
     unsigned remainderBase = remainder * chunkSize1;
     unsigned processOffset = (chunkNum-remainder) * chunkSize2;
     begin = remainderBase + processOffset;
     end = begin + chunkSize2;
   } 
   
   start = begin;
   stop = end;
}

extern "C" void launch_scan(const char* genome, indicesVecType* indices, const char* subSequence, long subSeqSize, long numCharsInFile) {    
    // ADD YOUR CODE HERE FOR LAUNCHING THE KERNEL
}
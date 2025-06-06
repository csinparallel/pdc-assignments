/** kernel.cpp defines the kernel method for scanning the genome
 * 
 * It includes functions for:
 * - compare_substrings(): a function for comparing two substrings on the device 
 * - launch_kernel(): launch_kernel(): Defines a function for scanning the genome for
 *   matching subsequences using OpenACC
 *
 * @author: Steven McKelvey, Calvin University, January 2025
 * 
 * HINT: the GPU's memory may not be able to hold the entire genome and an array of indices.
 *          Keep this memory constraint in mind when writing code.
 */

#include "GenomeScanner.h"
#include <cstring>
#include <cuda_runtime.h> // for getting device memory (cudaMemGetInfo)
#include <cmath>

#pragma acc routine(compare_substrings)

bool compare_substrings(const char* str1, const char* str2, int length) {
    for (int i = 0; i < length; ++i) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
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

/**
 * @param subSequence: the subsequence to scan for
 * @param genomeStart: a pointer to the genome string
 * @param indices: the return array for the indices where subsequence matches were found
 * @param numCharsInFile: the length of the genome
 */
extern void launch_kernel(string subSequence, char* genomeStart, vector<long>& indices, long numCharsInFile) {
    // ADD YOUR CODE HERE
}
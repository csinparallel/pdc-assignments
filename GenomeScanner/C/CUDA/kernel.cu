/** kernel.cu defines the kernel method for scanning the genome
 * 
 * It includes functions for:
 * - compare_substrings(): a function for comparing two substrings on the device 
 * - launch_kernel(): Defines a function for interfacing with a CUDA kernel
 *      (to be defined in this file) from files compiled with a non-CUDA compiler
 *
 * @author: Steven McKelvey, Calvin University, January 2025
 * 
 * HINT: the GPU's memory may not be able to hold the entire genome and an array of indices.
 *          Keep this memory constrant in mind when writing code.
 */
#include <cuda.h>
#include <stdio.h>
#include "indicesArrayType.h" 
#include "parallelLoopChunks.h"

__host__ void checkErr(cudaError_t err, const char *msg);
__device__ bool compare_substrings(const char* str1, const char* str2, int length);
extern "C" void launch_scan(const char* genome, indicesArrayType* indices, size_t* resultSize, const char* subSequence, long subSeqSize, long numCharsInFile);

__host__ void checkErr(cudaError_t err, const char *msg)
{
    if (err != cudaSuccess)
    {
        fprintf(stderr, "\n*** %s (error code %d: '%s')!\n\n",
                msg, err, cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
}

__device__ bool compare_substrings(const char* str1, const char* str2, int length) {
    for (int i = 0; i < length; ++i) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

extern "C" void launch_scan(const char* genome, indicesArrayType* indices, size_t* resultSize, const char* subSequence, long subSeqSize, long numCharsInFile) {    
    // ADD YOUR CODE HERE FOR LAUNCHING THE KERNEL
}
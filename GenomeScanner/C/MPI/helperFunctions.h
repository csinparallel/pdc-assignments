/* helperFunctions.h declares the functions used by genScanner.c
 *  that need to be tested.
 *
 * @author: Joel Adams, Calvin University, May 2024
 *
 * See: helperFunctions.c, genScanner.c.
 */

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <stdio.h>              // printf(), etc.
#include <stdlib.h>             // exit()
#include <string.h>             // strncpy()
#include <mpi.h>

#define FILE_NAME_MAX 64
#define SUBSEQUENCE_MAX 64

FILE* openFile(char* fileName);
long  fileLength(FILE* fPtr);
void  readSequence(FILE* fPtr, long size, char* seq);
long  scan(char* seq, long seqSize, char* subSeq, long subSeqSize,
            long* indexArray, long numIndices);

#endif // HELPER_FUNCTIONS_H

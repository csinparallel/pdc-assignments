/* OO_MPI_IO.h declares C++ templates that use  MPI_IO:
 *  - ParallelReader to read data from a binary file in parallel.
 *  - ParallelWriter to write data to a binary file in parallel.
 *
 * The template allows you to pass a type-parameter indicating
 *   the type of the data in the file.
 *
 * @author: Joel C. Adams, for CS 374 at Calvin University, Fall 2023.
 * Added the getChunkStartStopValues override with subsequence parameter
 * By Anwesha Pradhananga and Aishwarya Joshi, Calvin University, August 2024
 * Expanded to include a struct-based version of OO_MPI_IO_Base (renamed as ParallelReader)
 * by Steven McKelvey, Calvin University, January 2025
 */
#ifndef OO_MPI_IO_H
#define OO_MPI_IO_H

#include <mpi.h>                     // C MPI
#include <stdio.h>                   // C I/O
#include <stdlib.h>                  // C standard library
#include <string.h>                  // C string manipulation
#include <math.h>                    // ceil()
#include <limits.h>

typedef char ItemType; 

// Struct to replace the OO_MPI_IO_Base/ParallelReader class
typedef struct {
    int rank;                         // MPI process ID
    int numProcs;                     // number of MPI processes
    long itemSize;                    // size of 1 Item
    char* fileName;                   // file being opened
    MPI_Datatype mpiType;             // the MPI equiv of ItemType
    MPI_File fileHandle;              // MPI handle for file 

    // Attributes initialized later
    long numItemsInFile;              // total Items to be read
    MPI_Offset fileSize;              // size of file in bytes
    long chunkSize;                   // size of my chunk to read
    long firstItemOffset;             // offset of my chunk (Item #)
    long firstByteOffset;             // offset of my chunk (byte #)
} ParallelReader;

// Utility to check the return-values of MPI-IO function calls
void checkResult(int id, int result);

// ParallelReader "constructor" and "destructor"
void ParallelReader_init(ParallelReader* self, const char* fileName, int openMode,
                         MPI_Datatype mpiType, int rank, int numProcs);
void ParallelReader_destroy(ParallelReader* self);

// Function to get the chunk start and stop values
void getChunkStartStopValues(int id, int numProcs, const unsigned REPS,
                             long* start, long* stop);

// Function to read a chunk from the file (replaces the ParallelReader class)
char* readChunk(ParallelReader* self);

#endif // OO_MPI_IO_H

/** Utility to check the return-values of MPI-IO function calls
 * @param: id, an int
 * @param: result, an int
 * Precondition: id is the rank of the MPI process invoking this function 
 *           &&  result is the return-value from the last MPI-IO call.
 * Postcondition: If result indicates there was an error:
 *                 the string associated with result has been printed to stderr
 *                 && the program has been terminated abnormally.
 */
void checkResult(int id, int result) {
    if (result != MPI_SUCCESS) {
        char errorString[1024] = {'\0'};
        int errorStringLength = -1;
        int errorClass = -1;

        MPI_Error_class(result, &errorClass);
        MPI_Error_string(errorClass, errorString, &errorStringLength);
        fprintf(stderr, "Process %d: %s\n", id, errorString);

        MPI_Abort(MPI_COMM_WORLD, result);
    }
}

void ParallelReader_init(ParallelReader* self, const char* fileName, int openMode,
                         MPI_Datatype mpiType, int rank, int numProcs) {
    self->fileName = strdup(fileName);
    self->mpiType = mpiType;
    self->rank = rank;
    self->numProcs = numProcs;
    self->itemSize = sizeof(ItemType);  // Assuming the size of the MPI datatype is correct

    // Set other attributes to defaults
    self->numItemsInFile = 0;
    self->fileSize = 0;
    self->chunkSize = 0;
    self->firstItemOffset = 0;
    self->firstByteOffset = 0;

    int openResult = MPI_File_open(MPI_COMM_WORLD, fileName, openMode, MPI_INFO_NULL, &self->fileHandle);
    checkResult(rank, openResult);
}

void ParallelReader_destroy(ParallelReader* self) {
    MPI_File_close(&self->fileHandle);
    free(self->fileName);
}

void getChunkStartStopValues(int id, int numProcs, const unsigned REPS, long* start, long* stop) {
    if ((unsigned)numProcs > REPS) {
        if (id == 0) {
            printf("\n*** Number of MPI processes (%u) exceeds REPS (%u)\n", numProcs, REPS);
            printf("*** Please run with -np less than or equal to %u\n\n", REPS);
        }
        MPI_Finalize();
        exit(1);
    }

    unsigned chunkSize1 = (long)ceil(((double)REPS) / numProcs);
    unsigned begin = id * chunkSize1;
    unsigned end = begin + chunkSize1;
    unsigned remainder = REPS % numProcs;

    if (remainder > 0 && (unsigned)id >= remainder) {
        unsigned chunkSize2 = chunkSize1 - 1;
        unsigned remainderBase = remainder * chunkSize1;
        unsigned processOffset = (id - remainder) * chunkSize2;
        begin = remainderBase + processOffset;
        end = begin + chunkSize2;
    }

    *start = begin;
    *stop = end;
}

char* readChunk(ParallelReader* self) {
    MPI_Offset fileSize;
    MPI_File_get_size(self->fileHandle, &fileSize);
    self->fileSize = fileSize;
    self->numItemsInFile = fileSize / self->itemSize;

    long start = 0, stop = 0;
    getChunkStartStopValues(self->rank, self->numProcs, self->numItemsInFile, &start, &stop);
    self->chunkSize = stop - start;
    self->firstItemOffset = start;
    self->firstByteOffset = start * self->itemSize;

    MPI_Status status;
    unsigned long itemsRead = 0;
    unsigned long itemsToRead = self->chunkSize;
    char* returnChunk = (char*)malloc(sizeof(char) * itemsToRead);
    int readResult = 0;

    while (itemsToRead > INT_MAX) {
        readResult = MPI_File_read_at(self->fileHandle, self->firstByteOffset + itemsRead * self->itemSize,
            returnChunk + itemsRead * self->itemSize, INT_MAX, self->mpiType, &status);
        checkResult(self->rank, readResult);
        itemsRead += INT_MAX;
        itemsToRead -= INT_MAX;
    }

    if (itemsToRead > 0) {
        readResult = MPI_File_read_at(self->fileHandle, self->firstByteOffset + itemsRead * self->itemSize,
            returnChunk + itemsRead * self->itemSize, itemsToRead, self->mpiType, &status);
        checkResult(self->rank, readResult);
    }

    return returnChunk;
}

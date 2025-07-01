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
#include <omp.h>                     // C OpenMP
#include <stdio.h>                   // C I/O
#include <stdlib.h>                  // C standard library
#include <string.h>                  // C string manipulation
#include <math.h>                    // ceil()
#include <limits.h>
#include <stdbool.h>

typedef char ItemType; 

// Struct to replace the OO_MPI_IO_Base/ParallelReader class
typedef struct {
    int id;                         // MPI process ID
    int numPEs;                     // number of MPI processes
    long itemSize;                    // size of 1 Item
    char* fileName;                   // file being opened
    MPI_Datatype mpiType;             // the MPI equiv of ItemType
    MPI_File fileHandle;              // MPI handle for file 
    bool finalizeFlag;                // true iff MPI_Init not called

    // Attributes initialized later
    long numItemsInFile;              // total Items to be read
    MPI_Offset fileSize;              // size of file in bytes
    long chunkSize;                   // size of my chunk to read
    long firstItemOffset;             // offset of my chunk (Item #)
    long firstByteOffset;             // offset of my chunk (byte #)
} ParallelReader;

// Utility to check the return-values of MPI-IO function calls
void checkResult(int result);
void checkResultId(int id, int result);

// ParallelReader "constructor" and "destructor"
void ParallelReader_init(ParallelReader* self, const char* fileName, int openMode,
                         MPI_Datatype mpiType, int id, int numProcs);
void ParallelReader_destroy(ParallelReader* self);

// Function to get the chunk start and stop values
void getChunkStartStopValues(int id, int numProcs, const unsigned REPS,
                             long* start, long* stop);

// Function to read a chunk from the file (replaces the ParallelReader class)
char* readChunk(ParallelReader* self);

#endif // OO_MPI_IO_H


/* Utility to check the return-values of MPI-IO function calls
 * @param: result, an int
 * Precondition:  result is the return-value from the last MPI-IO call.
 * Postcondition: If result is anything other than MPI_SUCCESS::
 *                 the string associated with result has been printed to stderr
 *                 && the program has been terminated abnormally.
 */
void checkResult(int result) {
    if (result != MPI_SUCCESS) {
        char errorString[1024] = {'\0'};
        int  errorStringLength = -1;
        int  errorClass = -1;

        MPI_Error_class(result, &errorClass);
        MPI_Error_string(errorClass, errorString, &errorStringLength);
        fprintf(stderr, "\nMPI Error: '%s'\n\n", errorString);

        MPI_Abort(MPI_COMM_WORLD, result);
    }
}

/** Utility to check the return-values of MPI-IO function calls
 * @param: id, an int
 * @param: result, an int
 * Precondition: id is the rank of the MPI process invoking this function 
 *           &&  result is the return-value from the last MPI-IO call.
 * Postcondition: If result indicates there was an error:
 *                 the string associated with result has been printed to stderr
 *                 && the program has been terminated abnormally.
 */
void checkResultId(int id, int result) {
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
                         MPI_Datatype mpiType, int id, int numProcs) {
    self->fileName = strdup(fileName);
    self->mpiType = mpiType;
    self->itemSize = sizeof(ItemType);  // Assuming the size of the MPI datatype is correct
    if (id >= 0) { 
        self->id = id; 
     } else { 
        fprintf(stderr, "\nOO_MPI_IO_Base(): id must be non-negative\n\n");
        exit(1);
     } 
     if (numProcs > 0) { 
        self->numPEs = numProcs;
     } else { 
        fprintf(stderr, "\nOO_MPI_IO_Base(): numPEs must be positive\n\n");
        exit(1);
     } 

    // Set other attributes to defaults
    self->numItemsInFile = 0;
    self->fileSize = 0;
    self->chunkSize = 0;
    self->firstItemOffset = 0;
    self->firstByteOffset = 0;
    self->finalizeFlag = false;

    // for OpenMP: the main thread needs to call MPI_Init_thread()
    int mpiInitFlag = 0;
    MPI_Initialized(&mpiInitFlag);
    if (!mpiInitFlag && omp_get_thread_num() == 0) {    // if not:
        int modeProvided = 0;                           //  assume OpenMP prog and
        int initResult = MPI_Init_thread(0, 0,          //  call MPI_Init_thread()
                                  MPI_THREAD_MULTIPLE,  //  so any thread can make
                                  &modeProvided);       //  MPI calls
        checkResult(initResult);
        self->finalizeFlag = true;                      // set flag for destructor
    }
                                                        // make faster threads
#pragma omp barrier                                     // wait until
                                                        // MPI_Init_thread() done

    int openResult = MPI_File_open( MPI_COMM_WORLD,     // communicator
                                    self->fileName,   // name of file
                                    openMode,           // mode parameter
                                    MPI_INFO_NULL,      // skip this
                                    &(self->fileHandle) );    // MPI handle
    checkResult(openResult);
}

void ParallelReader_destroy(ParallelReader* self) {
    MPI_File_close(&self->fileHandle);
    free(self->fileName);
    #pragma omp barrier
    if (self->finalizeFlag &&                   // if MPI_Init() was not called 
        omp_get_thread_num() == 0) {        //  and we are the main thread,
       MPI_Finalize();                      //   then we need to finalize
    }    
}

/* Calculate the start and stop values for this PE's 
 *  contiguous chunk of a set of loop-iterations, 0..REPS-1,
 *  so that PEs' chunk-sizes are equal (or nearly so).
 *
 * @param: id, an int containing this PE's id (thread id or MPI rank)
 * @param: numPEs, an int containing the number of PEs
 * @param: REPS, a const unsigned containing the for loop's iteration total
 * Precondition: id == this thread's id or MPI process's rank
 *            && numPEs == the number of threads or MPI processes
 *            && REPS == the total number of 0-based loop iterations needed
 *            && numPEs <= REPS 
 *            && REPS < 2^32
 * @param: start, a long reference through which the 
 *          starting value of this PE's chunk should be returned
 * @param: stop, a long reference through which the
 *          stopping value of this PE's chunk should be returned
 * Postcondition: start == this PE's first iteration value 
 *             && stop == this PE's last iteration value + 1.
 */
void getChunkStartStopValues(int id, int numProcs, const unsigned REPS, long* start, long* stop) {
    // check precondition before proceeding
    if ((unsigned)numProcs > REPS) {
        if (id == 0) {
            printf("\n*** Number of MPI processes (%u) exceeds REPS (%u)\n", numProcs, REPS);
            printf("*** Please run with -np less than or equal to %u\n\n", REPS);
        }
        MPI_Finalize();
        exit(1);
    }
    
    // compute the chunk size that works in many cases
    unsigned chunkSize1 = (long)ceil(((double)REPS) / numProcs);
    unsigned begin = id * chunkSize1;
    unsigned end = begin + chunkSize1;
    // see if there are any leftover iterations
    unsigned remainder = REPS % numProcs;
    // If remainder == 0, chunkSize1 = chunk-size for all PEs;
    // If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
    //   but for PEs p_remainder..p_numPEs-1
    //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
    if (remainder > 0 && (unsigned)id >= remainder) {
        unsigned chunkSize2 = chunkSize1 - 1;
        unsigned remainderBase = remainder * chunkSize1;
        unsigned processOffset = (id - remainder) * chunkSize2;
        begin = remainderBase + processOffset;
        end = begin + chunkSize2;
    }
    // pass back this PE's begin and end values via start and stop
    *start = begin;
    *stop = end;
}

char* readChunk(ParallelReader* self) {
    MPI_Offset fileSize;
    MPI_File_get_size(self->fileHandle, &fileSize);
    self->fileSize = fileSize;
    if (self->mpiType == MPI_CHAR) {         // if ItemType is char
        --(self->fileSize);                            // ignore EOF char
     }
    self->numItemsInFile = fileSize / self->itemSize;

    long start = 0, stop = 0;
    getChunkStartStopValues(self->id, self->numPEs, self->numItemsInFile, &start, &stop);
    self->chunkSize = stop - start;
    self->firstItemOffset = start;
    self->firstByteOffset = start * self->itemSize;

    MPI_Status status;
    unsigned long itemsRead = 0;
    unsigned long itemsToRead = self->chunkSize;
    char* returnChunk = (char*)malloc(sizeof(char) * itemsToRead);
    int readResult = 0;
   // handle very large files where chunkSize > INT_MAX
    while (itemsToRead > INT_MAX) {
        readResult = MPI_File_read_at(self->fileHandle, self->firstByteOffset + itemsRead * self->itemSize,
            returnChunk + itemsRead * self->itemSize, INT_MAX, self->mpiType, &status);
        checkResultId(self->id, readResult);
        itemsRead += INT_MAX;
        itemsToRead -= INT_MAX;
    }

    // read in remaining Items (or if itemsToRead <= INT_MAX initially)
    if (itemsToRead > 0) {
        readResult = MPI_File_read_at(self->fileHandle, self->firstByteOffset + itemsRead * self->itemSize,
            returnChunk + itemsRead * self->itemSize, itemsToRead, self->mpiType, &status);
        checkResultId(self->id, readResult);
    }

    return returnChunk;
}

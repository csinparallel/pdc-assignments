/* helperFunctions.c defines the functions used by genScanner.c
 *                    that we want to test separately.
 *
 * @author: Joel Adams, Calvin University, May 2024
 *
 * See: helperFunctions.h, genScanner.c.
 */

#include "helperFunctions.h"
#include "OO_MPI_IO.h"

/* open the file, checking that it succeeds
 * @param: fileName, a char*
 * Precondition: fileName is the name of an accessible text file.
 */
FILE* openFile(char* fileName) {
   FILE* result = fopen(fileName, "r");
   if (result == NULL) {
      fprintf(stderr, "\n *** Unable to open input file '%s'\n\n", fileName);
      exit(1);
   } 

   return result;
}

/* find the number of bytes in the input file.
 * @param: fPtr, a FILE*
 * Precondition: fPtr is the handle for the user's input file
 * Postcondition: the function returns the number of bytes in the input file
 *                 (including the end-of-file (EOF) marker).
 */
long fileLength(FILE* fPtr) {
   if (fPtr) {
      fseek(fPtr, 0L, SEEK_END);   // find the end of the file
      long result = ftell(fPtr);   // the offset is the number of bytes
      fseek(fPtr, 0L, SEEK_SET);   // reset the file pointer
      return result;
   } else {
      return 0;
   }
}

/* read a specified number of bytes from an open file into a char array
 * @param: fPtr, a FILE*
 * @param: size, a long
 * @param: seq, a char*
 * Precondition: fPtr is a handle for the user's (open) input file
 *           && size is the number of chars to be read from the file
 *           && seq is the char buffer into which the chars should be read.
 * Postcondition: seq contains the chars from the input file.
 */
void readSequence(FILE* fPtr, long size, char* seq) {
   long numRead = fread(seq, 1, size, fPtr);
   if (numRead < size) {
      fprintf(stderr, "\n *** read error: expected to read %ld bytes"
                      " but read %ld instead\n\n", size, numRead);
      exit(1);
   }
}

/* scan a char array containing a sequence for a subsequence
 * @param: seq, a char* 
 * @param: seqSize, a long
 * @param: subSeq, a char*
 * @param: subSeqSize, a long
 * @param: indexArray, a long*
 * @param: indexArraySize, a long
 * Precondition: seq is the address of a buffer containing a sequence
 *           && seqSize is the number of chars in that buffer
 *           && subSeq is the address of a buffer containing a subsequence
 *           && subSeqSize is the number of chars in the subsequence
 *           && indexArray is the address of an array of longs
 *           && indexArraySize is the max capacity of that array. 
 * Postcondition: indexArray contains the indices of the occurrences of
 *                 subSeq within seq
 *           && the function returns the number of occurrences.
 */
long scan(char* seq, long seqSize, char* subSeq, long subSeqSize,
            long* indexArray, long indexArraySize) {

   long seqStop = seqSize - subSeqSize + 1;
   long result = 0;
   for (long i = 0; i < seqStop && result < indexArraySize; ++i) {
      if ( strncmp(seq+i, subSeq, subSeqSize) == 0 ) { // if they match
         indexArray[result] = i;
         ++result;
      }
   }   
   return result; 
}


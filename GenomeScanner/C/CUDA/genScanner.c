/* genScanner.c searches a genome G for a given subsequence SS,
 *  and outputs the number of occurrences and locations of SS within G.
 *
 * @author: Joel Adams, Calvin University, May 2024
 *
 * Usage: ./genScanner <genomeFile> <subsequence> [-i]
 *
 * Note: Use '-i' to display index values of subsequence within genome.
 *
 * Precondition: genomeFile is a text file containing 
 *                 a genetic sequence in *plain* format.
 *
 * HINT: this solution will use MPI, OpenMP, AND OpenACC to process the genome.
 *       It is recommended that you solve the OpenMP solution first, then build
 *       off of that for the OpenACC version.
 */

#include <omp.h>             // omp_get_wtime()
#include "helperFunctions.h" // declarations of helper functions
                           //  which provides most of the functionality

void processCommandLineArgs(int argc, char** argv, 
                            char** file, char** subSeq, int* indicesFlag);
void printResults(char* subSequence, long numSubSeqs, long* indexArray,
                                                      int showIndices);

int main(int argc, char** argv) { 
    char* fileName = NULL;
    char* subsequence = NULL;
    int  indicesFlag = 0;

    processCommandLineArgs(argc, argv, &fileName, &subsequence, &indicesFlag);

    // read in genome
    double startInputTime = omp_get_wtime();
    long subSeqLength = strnlen(subsequence, SUBSEQUENCE_MAX);
    FILE * fPtr = openFile(fileName);
    long numBytes = fileLength(fPtr);
    long numChars = numBytes-1;      // ignore EOF marker
    char* genome = (char*) malloc(numChars);
    readSequence(fPtr, numChars, genome);
    fclose(fPtr);
    double inputTime = omp_get_wtime() - startInputTime;

    // scan the genome for the subsequence
    double startScanTime = omp_get_wtime();
    long  numIndices = numChars / subSeqLength;
    long* indexArray = (long*) calloc(numIndices, sizeof(long));
    long  numSubSeqs = scan(genome, numChars, subsequence, subSeqLength,
                            indexArray, numIndices);
    double scanTime = omp_get_wtime() - startScanTime;

    // output results
    printResults(subsequence, numSubSeqs, indexArray, indicesFlag);
    printf("Total time: %lf secs (input: %lf; scan: %lf)\n\n",
             inputTime+scanTime, inputTime, scanTime);

    // clean up
    free(genome);
    free(indexArray);

    return 0;
}

/* retrieve inputs from command line
 * @param: argc, an int
 * @param: argv, a char**
 * @param: file, a char*
 * @param: subSeq, a char*
 * @param: indicesFlag, an int*
 * Precondition: argc and arg are the main function parameters
 *                 for retrieving command line values
 *            && the user has entered a file name and subsequence
 *                 on the command line
 *            && file is a char buffer big enough to hold the maximum file name
 *            && subSeq is a char buffer big enough to hold the max subsequence
 * Postcondition: file contains the name of the file the user entered
 *            && subSeq contains the name of the subsequence the user entered
 *                (i.e., what they want to search for)
 *            && indicesFlag = 1 iff argv[3] == "-i".
 */
void processCommandLineArgs(int argc, char** argv, 
                             char** file, char** subSeq, int* indicesFlag) {

    switch (argc) {
      case 4:
         if (strncmp(argv[3], "-i", 2) == 0) {
            *indicesFlag = 1;
         } else {
            fprintf(stderr, "\n *** Unsupported switch '%s' given\n\n",
                                                                 argv[3]);
            exit(1);
         } 
      case 3:
         *file = argv[1];
         *subSeq = argv[2];
         break;
      default:
        fprintf(stderr, "\n *** Usage: ./genomeScanner "
                        "<fileName> <subSequence> [-i]\n\n");
        exit(1);
   }
}

/* output results of scan
 * @param: subSeq, a char*
 * @param: numSubSeqs, a long
 * @param: indices, a long*
 * @param: showIndices, an int.
 * Precondition: subSeq contains the user's subsequence
 *           && numSubSeqs == the number of subSeq occurrences in the sequence
 *           && indices contains the indices of those occurrences.
 * Postcondition: if showIndices, then
 *                    the values in indices have been displayed
 *             && subSeq and numSubSeqs have been displayed.
 */   
void printResults(char* subSeq, long numSubSeqs, long* indices, int showIndices) {
   if (showIndices && numSubSeqs > 0) {
      printf("\nIndices of subsequence %s:\n ", subSeq);
      for (long i = 0; i < numSubSeqs; ++i) {
         printf(" %ld", indices[i]);
      }
      printf("\n");
   }

   printf("\nFound %ld instance%s of %s\n\n", numSubSeqs,
            ((numSubSeqs != 1) ? "s" : ""), subSeq);
}


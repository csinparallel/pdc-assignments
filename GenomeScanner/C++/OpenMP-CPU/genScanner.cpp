/* genScanner.cpp searches a genome G for a given subsequence SS,
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
 * HINT: this solution will use both MPI AND OpenMP. 
 */

#include "GenomeScanner.h" // declarations of class GenomeScanner
                           //  which provides most of the functionality 
#include <omp.h>
#include <mpi.h>   

void processCommandLineArgs(int argc, char** argv, 
                             string& file, string& subSeq, bool& indicesFlag);
void printResults(const string& subSequence, const vector<long>& results,
                   bool showResults = true);

int main(int argc, char** argv) { 
    string fileName, subsequence;
    bool indicesFlag;
    processCommandLineArgs(argc, argv, fileName, subsequence, indicesFlag);

    double startInputTime = omp_get_wtime();
    GenomeScanner genScanner(fileName);
    double inputTime = omp_get_wtime() - startInputTime;

    double startScanTime = omp_get_wtime();
    vector<long> indices = genScanner.scan(subsequence);
    double scanTime = omp_get_wtime() - startScanTime;

    printResults(subsequence, indices, indicesFlag);
    printf("Total time: %lf secs (input: %lf; scan: %lf)\n\n",
             inputTime+scanTime, inputTime, scanTime);
}

/* retrieve inputs from command line
 * @param: argc, an int
 * @param: argv, a char**
 * @param: file, a string&
 * @param: subSeq, a string&
 * @param: indicesFlag, a bool&
 * Precondition: argc and arg are the main function's parameters
 *                 for retrieving command line values
 *            && the user has entered a file name and subsequence
 *                 on the command line
 * Postcondition: file contains the name of the file the user entered
 *            && subSeq contains the subsequence the user entered
 *                (i.e., what they want to search for)
 *            && indicesFlag == true iff argv[3] == "-i".
 */
void processCommandLineArgs(int argc, char** argv, 
                             string& file, string& subSeq, bool& indicesFlag) {
   switch (argc) {
      case 4:
         if (string(argv[3]) == "-i") {
            indicesFlag = true;
         } else {
            fprintf(stderr, "\n *** Unsupported switch '%s' given\n\n",
                                                                 argv[3]);
            exit(1);
         }
      case 3:
         file = string(argv[1]);
         subSeq = string(argv[2]);
         break;
      default:
        fprintf(stderr, "\n *** Usage: ./genomeScanner "
                        "<fileName> <subSequence> [-i]\n\n");
        exit(1);
   }
}

/* output results of scanning 
 * @param: subSeq, a string
 * @param: indices, a vector<long>
 * @param: showIndices, a bool
 * Precondition: subSeq contains the user's subsequence
 *           && indices contain the indices of the occurrences
 *                of subsequence within the genome.
 * Postcondition: if showIndices, then
 *                   the values in indices have been displayed
 *           && subSeq and the number of occurrences have been displayed.
 */   
void printResults(const string& subSeq, const vector<long>& indices,
                                         bool showIndices) {
   long numSubSeqs = indices.size();
   if (showIndices && numSubSeqs > 0) {
      printf("\nIndices of subsequence %s:\n ", subSeq.c_str());
      for (long i = 0; i < numSubSeqs; ++i) {
         printf(" %ld", indices[i]);
      }
   }
 
   printf("\n\nFound %ld instance%s of %s\n\n", numSubSeqs, 
            ((numSubSeqs != 1) ? "s" : ""), subSeq.c_str());
}


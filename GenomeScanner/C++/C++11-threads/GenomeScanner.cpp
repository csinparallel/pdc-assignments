/* GenomeScanner.cpp defines the GenomeScanner methods.
 *
 * @author: Joel Adams, Calvin University, May 2024
 *
 * See: GenomeScanner.h, genScanner.cpp.
 */

#include "GenomeScanner.h"
#include "OO_MPI_IO.h"
#include <thread>

GenomeScanner::GenomeScanner(const string& fileName) {
   myFileName = fileName;
   myFilePtr  = openFile(fileName);
   numCharsInFile = fileLength(myFilePtr) - 1; // exclude EOF
   readSequence(myFilePtr);                    // into mySequence
   fclose(myFilePtr);
}

/* open the file, checking that it succeeds
 * @param: fileName, a char*
 * Precondition: fileName is the name of an accessible text file.
 */
FILE* GenomeScanner::openFile(const string& fileName) {
   FILE* result = fopen(fileName.c_str(), "r");
   if (result == NULL) {
      fprintf(stderr, "\n *** Unable to open input file '%s'\n\n", 
                fileName.c_str());
      exit(1);
   } 

   return result;
}

/* find the number of bytes in the input file.
 * @param: fPtr, a FILE*
 * Precondition: fPtr is the handle for the user's input file
 * Postcondition: the function has returned the number of bytes in the input file.
 */
long GenomeScanner::fileLength(FILE* fPtr) {
   if (fPtr) {
      fseek(fPtr, 0L, SEEK_END);   // find the end of the file
      long result = ftell(fPtr);   // the offset is the number of bytes
      fseek(fPtr, 0L, SEEK_SET);   // reset the file pointer
      return result;
   } else {
      return 0;
   }
}

/* read bytes from a file into myGenome
 * @param: fPtr, a FILE*
 * Precondition: fPtr is a handle for the user's input file
 *           && numCharsInFile is the size of the genome in that file
 * Postcondition: myGenome contains the chars from fPtr.
 */
void GenomeScanner::readSequence(FILE* fPtr) {
   myGenome.resize(numCharsInFile);
   long numRead = fread(myGenome.data(), 1, numCharsInFile, fPtr);

   if (numRead != numCharsInFile) {
      fprintf(stderr, "\n *** read error: expected to read %ld bytes"
                      " but read %ld instead\n\n", 
                        numCharsInFile, numRead);
      exit(1);
   }
}

/* scan myGenome for a subsequence
 * @param: subSequence, a string
 * Precondition: myGenome contains the user's genomic sequence
 *           && subSeq contains the users's subsequence
 * Postcondition: the function returns a vector containing
 *                 the indices of subSeq within seq.
 */
vector<long> GenomeScanner::scan(const string& subSequence) {
   long genomeSize = myGenome.size();
   long subSeqSize = subSequence.size();
   long genomeStop = genomeSize - subSeqSize + 1;
   const char* genomeStart = myGenome.data();
   const char* subSeqStart = subSequence.data();
   vector<long> indices;
   for (long i = 0; i < genomeStop; ++i) {
      if ( strncmp(genomeStart+i, subSeqStart, subSeqSize) == 0 ) {
         indices.push_back(i);
      }
   }   
   return indices; 
}



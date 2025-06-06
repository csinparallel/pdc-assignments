/* GenomeScanner.h declares the GenomeScanner class that,
 *  given a genetic sequence GS and a subsequence SS,
 *  finds all the occurrences of the SS with GS.
 *
 * @author: Joel Adams, Calvin University, May 2024
 *
 * See: genScanner.cpp, GenomeScanner.cpp.
 */

#include <cstdio>              // printf(), etc.
#include <cstdlib>             // exit()
#include <string>
#include <vector>
#include <cstring>
#include <climits>            // INT_MAX
#include "indicesVecType.h"
using namespace std;

class GenomeScanner {
public:
   GenomeScanner(const string& fileName);
   vector<long> scan(const string& subsequence);

protected:
   FILE* openFile(const string& fileName);
   long  fileLength(FILE* fPtr);
   void  readSequence(FILE* fPtr);

private:
   string myFileName;
   FILE*  myFilePtr;
   long   numCharsInFile;
   string myGenome;
};




/* testScanner.c tests the functions in ../functionDefs.c
 *
 * @author: Joel Adams, Calvin University, May 2024
 *
 * Usage: ./testScanner
 */

#include <assert.h>              // assert()
#include "../helperFunctions.h"  // declarations of the functions 
                                 //  being tested below

/* generic method to test different instances of the scanning problem
 * @param: fileName, a char*
 * @param: subSeq, a char*
 * @param: predictedIndexArray, a long*
 * @param: numPredictedIndices, a long
 *
 */
void test(char* fileName, char* subSeq, 
           long* predictedIndexArray, long numPredictedIndices) {
    long subSeqLength = strnlen(subSeq, SUBSEQUENCE_MAX);
    FILE * fPtr = openFile(fileName);
    long numBytes = fileLength(fPtr);
    long numChars = numBytes-1;      // ignore EOF marker
    char* sequence = (char*) malloc(numChars);
    readSequence(fPtr, numChars, sequence);
    fclose(fPtr);

    long  numIndices = numChars / subSeqLength;
    long* indexArray = (long*) calloc(numIndices, sizeof(long));
    long  numSubSeqs = scan(sequence, numChars, subSeq, subSeqLength,
                            indexArray, numIndices);
    assert( numSubSeqs == numPredictedIndices);
    for (long i = 0; i < numPredictedIndices; ++i) {
        assert( indexArray[i] == predictedIndexArray[i] );
    }
    free(sequence);
    free(indexArray);
}

void runTests() {
    printf("\nTesting functions in 'helperFunctions.h'...\n");
    // single match tests
    // beginning
    long indexArray0[1]; 
    indexArray0[0] = 0;
    test("52Chars.txt", "ABC", indexArray0, 1);
    printf(" 0 "); fflush(stdout);
    // end
    long indexArray1[1]; 
    indexArray1[0] = 49;
    test("52Chars.txt", "xyz", indexArray1, 1);
    printf(" 1 "); fflush(stdout);
    // middle
    long indexArray2[1]; 
    indexArray2[0] = 25;
    test("52Chars.txt", "Zab", indexArray2, 1);
    printf(" 2 "); fflush(stdout);

    // 2-match tests
    // beginning and middle
    long indexArray3[2]; 
    indexArray3[0] = 0;
    indexArray3[1] = 52;
    test("104Chars.txt", "ABC", indexArray3, 2);
    printf(" 3 "); fflush(stdout);
    // middle and end
    long indexArray4[2]; 
    indexArray4[0] = 49;
    indexArray4[1] = 101;
    test("104Chars.txt", "xyz", indexArray4, 2);
    printf(" 4 "); fflush(stdout);
    // middle and middle
    long indexArray5[2]; 
    indexArray5[0] = 25;
    indexArray5[1] = 77;
    test("104Chars.txt", "Zab", indexArray5, 2);
    printf(" 5 "); fflush(stdout);

    printf("\nAll tests passed!\n\n");
}

int main(int argc, char** argv) { 
   runTests();
}

/* testScanner.cpp tests the methods in ../GenomeScanner.cpp
 *
 * @author: Joel Adams, Calvin University, May 2024
 *
 * Usage: ./testScanner 
 */

#include <assert.h>            // assert()
#include "../GenomeScanner.h"  // declarations of the functions called below
                               //  separated out for testing purposes

void test(const string& fileName, const string& subSeq, 
           const vector<long>& predictedIndices) {
    long numPredictedIndices = predictedIndices.size();
    GenomeScanner genScanner(fileName);
    vector<long> foundIndices = genScanner.scan(subSeq);
    long numFoundIndices = foundIndices.size();
    assert( numPredictedIndices == numFoundIndices);
    for (long i = 0; i < numFoundIndices; ++i) {
        assert( predictedIndices[i] == foundIndices[i] );
    }
}

void runTests() {
    printf("\nTesting GenomeScanner methods...\n");
    // single match tests
    // beginning
    vector<long> v0(1); 
    v0[0] = 0;
    test("52Chars.txt", "ABC", v0);
    printf(" 0 "); fflush(stdout);
    // end
    vector<long> v1(1); 
    v1[0] = 49;
    test("52Chars.txt", "xyz", v1);
    printf(" 1 "); fflush(stdout);
    // middle
    vector<long> v2(1);
    v2[0] = 25;
    test("52Chars.txt", "Zab", v2);
    printf(" 2 "); fflush(stdout);

    // 2-match tests
    // beginning and middle
    vector<long> v3(2);
    v3[0] = 0;
    v3[1] = 52;
    test("104Chars.txt", "ABC", v3);
    printf(" 3 "); fflush(stdout);
    // middle and end
    vector<long> v4(2); 
    v4[0] = 49;
    v4[1] = 101;
    test("104Chars.txt", "xyz", v4);
    printf(" 4 "); fflush(stdout);
    // middle and middle
    vector<long> v5(2); 
    v5[0] = 25;
    v5[1] = 77;
    test("104Chars.txt", "Zab", v5);
    printf(" 5 "); fflush(stdout);

    printf("\nAll tests passed!\n\n");
}

int main(int argc, char** argv) { 
   runTests();
}

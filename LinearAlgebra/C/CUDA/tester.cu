/** tester.c runs the tests for the Matrix operations
 * These tests are found in MatrixTester.c and MatrixTester.h
 * Author: Steven McKelvey (Calvin University)
 * Based on a C++ solution by Joel C. Adams (Calvin University)
 * For: CSinParallel.org
 */

#include "MatrixTester.cuh"

// void generateTestFile(unsigned SIZE);
void generateBinTestFile(unsigned SIZE);

int main(int argc, char **argv)
{
	const unsigned MAX = 4096;
	// const unsigned MAX = 1024;
	// generateTestFile(MAX);
	generateBinTestFile(MAX);
	runTests();
}

/** function to generate a large Matrix datafile for testing
 * @param: SIZE, the number of rows and columns
 *              the test-matrix should have.
 * @postcondition: a SIZExSIZE matrix has been created
 *                 in a file named 'testFiles/SIZExSIZE.txt'.
 */
// void generateTestFile(unsigned SIZE) {
// 	char buf[32];
// 	sprintf(buf, "testFiles/%dx%d.txt", SIZE, SIZE);

// 	Matrix* m = (Matrix*)malloc(sizeof(Matrix));
// 	m = initializeAsc(SIZE, SIZE);
// 	dumpText(buf, m);

// 	freeMatrix(m);
// 	free(m);
// }

/** function to generate a large Matrix binary datafile for testing
 * @param: SIZE, the number of rows and columns
 *              the test-matrix should have.
 * @postcondition: a SIZExSIZE matrix has been created
 *                 in a file named 'testFiles/SIZExSIZE.bin'.
 */
void generateBinTestFile(unsigned SIZE) {
	char buf[32];
	sprintf(buf, "testFiles/%dx%d.bin", SIZE, SIZE);

	Matrix* m = (Matrix*)malloc(sizeof(Matrix));
	m = initializeAsc(SIZE, SIZE);
	dumpBin(buf, m);

	freeMatrix(m);
	free(m);
}

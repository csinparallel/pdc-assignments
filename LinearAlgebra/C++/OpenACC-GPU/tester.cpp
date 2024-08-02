/** tester.cpp runs the tests for the Matrix operations
 * These tests are found in MatrixTester.cpp and MatrixTester.h
 * Author: Joel C. Adams, Calvin University
 * For: CSinParallel.org
 */

#include "MatrixTester.h"

void generateTestFile(unsigned SIZE);
void generateBinTestFile(unsigned SIZE);

int main(int argc, char **argv)
{
	// const unsigned MAX = 4096;
	// const unsigned MAX = 1024;
	// generateTestFile(MAX);
	// generateBinTestFile(MAX);
	MatrixTester mt;
	mt.runTests();
}

/** function to generate a large Matrix datafile for testing
 * @param: SIZE, the number of rows and columns
 *              the test-matrix should have.
 * @postcondition: a SIZExSIZE matrix has been created
 *                 in a file named 'testFiles/SIZExSIZE.txt'.
 */

void generateTestFile(unsigned SIZE)
{
	char buf[32];
	sprintf(buf, "testFiles/%dx%d.txt", SIZE, SIZE);
	ofstream fout(buf);
	assert(fout.is_open());
	fout << SIZE << " " << SIZE << endl;
	for (unsigned i = 0; i < SIZE; ++i)
	{
		for (unsigned j = 0; j < SIZE; ++j)
		{
			fout << i * SIZE + j << '\t';
		}
		fout << '\n';
	}
	fout.close();
}

/** function to generate a large Matrix binary datafile for testing
 * @param: SIZE, the number of rows and columns
 *              the test-matrix should have.
 * @postcondition: a SIZExSIZE matrix has been created
 *                 in a file named 'testFiles/SIZExSIZE.bin'.
 */
void generateBinTestFile(unsigned SIZE)
{
	char buf[32];
	sprintf(buf, "testFiles/%dx%d.bin", SIZE, SIZE);
	ofstream fout(buf);
	assert(fout.is_open());
	fout.write(reinterpret_cast<char *>(&SIZE), sizeof(int));
	fout.write(reinterpret_cast<char *>(&SIZE), sizeof(int));
	for (unsigned i = 0; i < SIZE; ++i)
	{
		for (unsigned j = 0; j < SIZE; ++j)
		{
			double index = i * SIZE + j;
			fout.write(reinterpret_cast<char *>(&index), sizeof(double));
		}
	}
	fout.close();
}

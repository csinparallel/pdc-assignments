/** generateTestFiles.cpp
 * A program for generating the testfiles used by MatrixTester and MatrixTimer
 * 
 * These files will be created in the parent directory.
 * 
 * Large files are created as binary files for efficient usage while smaller files
 * are created as text files for readability.
 * 
 * By: Steven McKelvey, Calvin University, July 2024
 */
#include <iostream> // istream, ostream
#include <fstream>	// ifstream, ofstream
#include <cassert>
#include <map>
#include <string>

using namespace std;
typedef map<int, int> offset_map;

void generateTestFile(unsigned SIZE);
void generateBinTestFile(unsigned SIZE); 
// optional map for setting offsets
void generateMatFile(string filename, unsigned height, unsigned length, offset_map offsets = offset_map());

int main()
{
	generateTestFile(1024);
	generateTestFile(4096);
	generateBinTestFile(4096);
	generateMatFile("../mat1.txt", 3, 4);
	offset_map mat2;
	mat2[1] = -1; // offset first number of file by -1.
	generateMatFile("../mat2.txt", 3, 4, mat2);
	offset_map mat3;
	mat3[12] = 1;
	generateMatFile("../mat3.txt", 3, 4, mat3);
	offset_map mat4;
	mat4[7] = -1;
	generateMatFile("../mat4.txt", 3, 4, mat4);
	offset_map mat5;
	mat5[4] = 1;
	mat5[5] = 1;
	mat5[7] = 2;
	mat5[8] = 2;
	mat5[9] = 2;
	generateMatFile("../mat5.txt", 3, 3, mat5);
	generateMatFile("../mat6.txt", 4, 4);
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
	sprintf(buf, "../%dx%d.txt", SIZE, SIZE);
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
	sprintf(buf, "../%dx%d.bin", SIZE, SIZE);
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

/** generateMatFile()
 * Creates a text file that stores a matrix with ascending numbers, starting with 1.
 * @param filename: the name of the file this matrix should be written to.
 * @param length: the length of the matrix
 * @param height: the height of the matrix
 * @param offsets: a map that contains an index and the offset it should have.
 *
 * Although the function is set to create a file with numbers in ascending order by default,
 * the user can user the "offsets" map to change the value of each number that is printed.
 * The key is the number/index that would normally be printed (from 1..height*length), and the
 * value is the offset that number should have.
 * 
 * For example, having <1, -1> in the map states that the first number should have an offset of -1
 * (making the file read "0" instead of "1" at the beginning)
 */
void generateMatFile(string filename, unsigned height, unsigned length, offset_map offsets)
{
	ofstream fout(filename);
	assert(fout.is_open());
	fout << height << " " << length << endl;
	for (unsigned i = 0; i < height; ++i)
	{
		for (unsigned j = 1; j <= length; ++j)
		{
			int num = i * length + j;
			// ascending number + offset (if the offset does not exist, it adds 0.)
			fout << num + offsets[num] << '\t';
		}
		fout << '\n';
	}
	fout.close();
}
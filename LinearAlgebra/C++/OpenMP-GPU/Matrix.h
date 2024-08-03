/** Matrix.h defines a simple Matrix class, built using a 1D array.
 * Author: Joel C. Adams
 * For: OpenMP lab in CS 112 at Calvin College.
 * Copyright Joel C. Adams, 2009.  All rights reserved.
 ****************************************************************/

#ifndef MATRIX
#define MATRIX

#include <omp.h>	 // openMP
#include <iostream>	 // istream, ostream
#include <fstream>	 // ifstream, ofstream
#include <cassert>	 // assert()
#include <stdexcept> // exceptions
using namespace std;

template <class Item>
class Matrix
{
public:
	Matrix();
	Matrix(unsigned rows, unsigned columns);
	Matrix(const Matrix &original);
	Matrix(Matrix &&mat);
	~Matrix();

	Item get(unsigned row, unsigned col) const;
	void set(unsigned row, unsigned col, Item value);
	Item *operator[](unsigned index);
	const Item *operator[](unsigned index) const;

	unsigned getRows() const;
	unsigned getColumns() const;

	void print(ostream &out) const;
	void read(istream &in);

	void fillText(const string &fileName);
	void fillBin(const string &fileName);
	void dumpText(const string &fileName);
	void dumpBin(const string &fileName);

	Matrix &operator=(const Matrix &original);
	bool operator==(const Matrix &mat1) const;
	Matrix operator+(const Matrix &mat2) const;
	Matrix transpose() const;
	Matrix operator*(const Matrix &mat1) const;
	// ... additional operations omitted ...
protected:
	void allocate(unsigned rows, unsigned columns);
	void deallocate();
	void initialize(Item value);
	void makeCopyOf(const Matrix &original);

private:
	unsigned myRows,
		myColumns;
	Item *myMatrix;
	friend class MatrixTester;
};

// bool approxEqual(double d1, double d2);

/** default constructor
 * @postcondition: myRows == 0 && myColumns == 0 &&
 *                 myMatrix == NULL
 */
template <class Item>
inline Matrix<Item>::Matrix()
{
	myRows = 0;
	myColumns = 0;
	myMatrix = nullptr;
}

/** constructor
 * @param: rows, columns, two unsigned values.
 * @precondition: rows > 0 && columns > 0.
 * @postcondition: myRows == rows && myColumns == columns &&
 *                 myMatrix points to a rows x columns array.
 */
template <class Item>
inline Matrix<Item>::Matrix(unsigned rows, unsigned columns)
{
	assert(rows > 0 && columns > 0);
	myRows = rows;
	myColumns = columns;
	myMatrix = nullptr;
	allocate(rows, columns);
	initialize(0.0);
}

/** --- copy constructor
 * @param: original, a Matrix
 * @postcondition: I am a distinct copy of original.
 */
template <class Item>
inline Matrix<Item>::Matrix(const Matrix<Item> &original)
{
	makeCopyOf(original);
}

/** move constructor
 */
template <class Item>
inline Matrix<Item>::Matrix(Matrix<Item> &&mat)
{
	myRows = mat.myRows;
	myColumns = mat.myColumns;
	myMatrix = mat.myMatrix;
	mat.myMatrix = nullptr;
}

/** destructor
 * @postcondition: my memory has been reclaimed.
 */
template <class Item>
inline Matrix<Item>::~Matrix()
{
	deallocate();
}

/** access (read) a value
 * @param: row, col, two unsigned values.
 * @precondition: row >= 0 && col >= 0 &&
 *               row < myRows && col < myCols.
 * @postcondition: myMatrix[row][col] has been returned.
 */
template <class Item>
inline Item Matrix<Item>::get(unsigned row, unsigned col) const
{
	return *((myMatrix + row * myColumns) + col);
}

/** access (read) a value
 * @param: row, col, two unsigned values;
 *             value, an Item.
 * @precondition: row >= 0 && col >= 0 &&
 *               row < myRows && col < myCols.
 * @postcondition: myMatrix[row][col] == value.
 */
template <class Item>
inline void Matrix<Item>::set(unsigned row, unsigned col, Item value)
{
	*((myMatrix + row * myColumns) + col) = value;
}

template <class Item>
inline const Item *Matrix<Item>::operator[](unsigned index) const
{
	return myMatrix + index * myColumns;
}

template <class Item>
inline Item *Matrix<Item>::operator[](unsigned index)
{
	return myMatrix + index * myColumns;
}

/** Number of rows in a Matrix
 * @postcondition: myRows has been returned.
 */
template <class Item>
inline unsigned Matrix<Item>::getRows() const
{
	return myRows;
}

/** Number of columns in a Matrix
 * @postcondition: myColumns has been returned.
 */
template <class Item>
inline unsigned Matrix<Item>::getColumns() const
{
	return myColumns;
}

/** output operator
 * @param: out, an ostream;
 *             mat, a Matrix.
 * @precondition: out is an open ostream.
 * @postcondition: mat's values have been inserted into out.
 */
template <class Item>
inline ostream &operator<<(ostream &out, const Matrix<Item> &mat)
{
	mat.print(out);
	return out;
}

/** input operator
 * @param: in, an istream;
 *             mat, a Matrix.
 * @precondition: in is an open istream containing
 *                myRows x myColumns values.
 * @postcondition: myMatrix contains the input values.
 */
template <class Item>
inline istream &operator>>(istream &in, Matrix<Item> &theMatrix)
{
	theMatrix.read(in);
	return in;
}

/** --- output using *this
 * @param: out, an ostream.
 * @precondition: out is an open ostream
 * @postcondition: my values have been inserted into out,
 *                 with tab chars between each value.
 */
template <class Item>
inline void Matrix<Item>::print(ostream &out) const
{
	for (unsigned i = 0; i < myRows; ++i)
	{
		for (unsigned j = 0; j < myColumns; ++j)
		{
			out << (*this)[i][j] << '\t';
		}
		out << '\n';
	}
}

/** --- input using *this
 * @param: in, an ostream.
 * @precondition: in is an open istream
 *                containing myRows x myColumns values.
 * @postcondition: I now contain a copy of the values from in.
 */
template <class Item>
inline void Matrix<Item>::read(istream &in)
{
	for (unsigned i = 0; i < myRows; ++i)
	{
		for (unsigned j = 0; j < myColumns; ++j)
		{
			in >> (*this)[i][j];
		}
	}
}

/** --- input from text file
 * @param: fileName, a string.
 * @precondition: fileName is the name of (or path to) a file,
 *                 the first line of file is #rows #columns,
 *                 and the remainder of which is rows x columns values.
 * @postcondition: myRows == rows && myColumns == columns &&
 *                 I contain the other values from fileName.
 */
template <class Item>
inline void Matrix<Item>::fillText(const string &fileName)
{
	ifstream fin(fileName.c_str());
	assert(fin.is_open());
	unsigned rows, columns;
	fin >> rows >> columns;
	assert(rows > 0 && columns > 0);
	myRows = rows;
	myColumns = columns;
	allocate(rows, columns);
	Item value;
	for (unsigned i = 0; i < rows; ++i)
	{
		for (unsigned j = 0; j < columns; ++j)
		{
			fin >> value;
			assert(!fin.eof());
			(*this)[i][j] = value;
		}
	}
	fin.close();
}

/** --- input from binary file
 * @param: fileName, a string.
 * @precondition: fileName is the name of (or path to) a file,
 *                 the beginning of file is #rows #columns,
 *                 and the remainder of which is rows x columns values.
 * @postcondition: myRows == rows && myColumns == columns &&
 *                 I contain the other values from fileName.
 */
template <class Item>
inline void Matrix<Item>::fillBin(const string &fileName)
{
	ifstream fin(fileName.c_str(), ios::binary);
	assert(fin.is_open());
	unsigned rows, columns;
	// read rows/columns into this matrix
	// reinterpret_cast<char*> allows types other than char to be read from the binary file.
	fin.read(reinterpret_cast<char *>(&rows), sizeof(int));
	fin.read(reinterpret_cast<char *>(&columns), sizeof(int));
	assert(rows > 0 && columns > 0);
	myRows = rows;
	myColumns = columns;
	allocate(rows, columns);
	// read values int this matrix
	fin.read(reinterpret_cast<char *>(myMatrix), sizeof(Item) * myRows * myColumns);
	fin.close();
}

/** --- output to a text file
 * @param: fileName, a String.
 * @postcondition: the file named fileName contains myRows and myColumns on the first line,
 *                and each subsequent line contains one of my rows.
 */
template <class Item>
inline void Matrix<Item>::dumpText(const string &fileName)
{
	ofstream fout(fileName.c_str());
	assert(fout.is_open());
	fout << myRows << ' ' << myColumns << '\n';
	print(fout);
	fout.close();
}

/** --- output to a binary file
 * @param: fileName, a String.
 * @postcondition: the file named fileName contains myRows and myColumns at the beginning,
 *                and the subsequent data contains each of my rows.
 */
template <class Item>
inline void Matrix<Item>::dumpBin(const string &fileName)
{
	ofstream fout(fileName.c_str());
	assert(fout.is_open());
	// reinterpret_cast<char*> allows types other than char to be written to the binary file.
	fout.write(reinterpret_cast<char *>(&myRows), sizeof(int));
	fout.write(reinterpret_cast<char *>(&myColumns), sizeof(int));
	fout.write(reinterpret_cast<char *>(myMatrix), sizeof(Item) * myRows * myColumns);
	fout.close();
}

/** assignment
 * @param: original, another Matrix.
 * @postcondition: I am a copy of original &&
 *                 I have not leaked any memory.
 */
template <class Item>
inline Matrix<Item> &Matrix<Item>::operator=(const Matrix<Item> &original)
{
	if (this != &original)
	{
		if (myRows > 0)
		{
			deallocate();
		}
		makeCopyOf(original);
	}
	return *this;
}

/** addition
 * @param: mat2, another Matrix.
 * @precondition: mat2.getRows() == myRows &&
 *               mat2.getColumns() == myColumns.
 * @return: result, a Matrix with my dimensions
 *          containing the sum of myself and mat2.
 */
template <class Item>
inline Matrix<Item> Matrix<Item>::operator+(const Matrix<Item> &mat2) const
{
	if (myRows != mat2.getRows() || myColumns != mat2.getColumns())
	{
		throw invalid_argument("Matrix::operator+: matrix dimensions mismatch");
	}
	Matrix result(myRows, myColumns);
	unsigned size = myRows * myColumns;
	// needed because, if one tries to return the same matrix that is mapped to the device,
	// they get a "terminated by signal 11" error during compilation.
	Matrix returnMatrix(result);

	/* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the object AND its array member (matrix.myMatrix)
	*/
	for (unsigned rows = 0; rows < myRows; ++rows)
	{
		for (unsigned cols = 0; cols < myColumns; ++cols)
		{
			result[rows][cols] = (*this)[rows][cols] + mat2[rows][cols];
		}
	}

	// copy the results to the output matrix (see note above about "terminated by signal 11")
	copy(result.myMatrix, result.myMatrix + size, returnMatrix.myMatrix);

	return returnMatrix;
}

/** transpose
 * @return: result, containing a myColumns-by-myRows Matrix,
 *                whose column values are my row values
 */
template <class Item>
inline Matrix<Item> Matrix<Item>::transpose() const
{
	if (myRows <= 0)
	{
		throw invalid_argument("Matrix::transpose(): empty Matrix!");
	}
	Matrix result(myColumns, myRows);
	unsigned size = myRows * myColumns;
	// needed because, if one tries to return the same matrix that is mapped to the device,
	// they get a "terminated by signal 11" error during compilation.
	Matrix returnMatrix(result);

	/* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the object AND its array member (matrix.myMatrix)
	*/
	for (unsigned rows = 0; rows < myRows; ++rows)
	{
		for (unsigned cols = 0; cols < myColumns; ++cols)
		{
			result[cols][rows] = (*this)[rows][cols];
		}
	}

	// copy the results to the output matrix (see note above about "terminated by signal 11")
	copy(result.myMatrix, result.myMatrix + size, returnMatrix.myMatrix);

	return returnMatrix;
}

/** --- boolean equality
 * @param: mat2, a Matrix.
 * @postcondition: true is returned, if and only if
 *                 myRows == mat2.rows() &&
 *                 myColumns == mat2.columns() &&
 *                 my values are the same as mat2's values.
 */
template <class Item>
inline bool Matrix<Item>::operator==(const Matrix<Item> &mat2) const
{
	if (myColumns != mat2.getColumns() || myRows != mat2.getRows())
	{
		return false;
	}
	bool equal = true; // equal until proven otherwise

	/* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the object AND its array member (matrix.myMatrix)
	*/
	for (unsigned i = 0; i < mat2.getRows(); ++i)
	{
		for (unsigned j = 0; j < mat2.getColumns(); ++j)
		{
			if ((*this)[i][j] != mat2[i][j])
			{
				equal = false;
			}
		}
	}
	return equal;
}

/** approxEqual
 * Compares two doubles and returns true if they are equal
 * up to a certain level of precision (determined by THRESHOLD).
 * Useful when using a file (such as a text file) that might not
 * store the exact, original precision of a value.
 *
 * To use this function, replace
 *      "(*this)[i][j] != mat2[i][j]"
 * in the above "equal()" function with
 *      "!approxEqual((*this)[i][j], mat2[i][j])"
 *
 */
// bool approxEqual(double d1, double d2)
// {
//     double THRESHOLD = .00001;
//     return abs(d1 - d2) < THRESHOLD;
// }

/** --- multiplication
 * @param: mat2, a Matrix.
 * @precondition: myColumns == mat2.rows().
 * @postcondition: a Matrix is returned containing the product of
 *                 my values and those of mat2.
 */
template <class Item>
inline Matrix<Item> Matrix<Item>::operator*(const Matrix<Item> &mat2) const
{
	if (myColumns != mat2.getRows())
	{ // check dimensions
		throw invalid_argument("Matrix::operator*(): invalid dimensions");
	}
	Matrix mat3(myRows, mat2.getColumns()); // build result Matrix
	unsigned outputSize = myRows * mat2.getColumns();
	// needed because, if one tries to return the same matrix that is mapped to the device,
	// they get a "terminated by signal 11" error during compilation.
	Matrix returnMatrix(mat3);

	/* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the object AND its array member (matrix.myMatrix)
	*/
	for (unsigned i = 0; i < myRows; ++i)
	{ // for each of my rows:
		for (unsigned j = 0; j < mat2.getColumns(); ++j)
		{ //  for each col in mat2:

			Item sum = 0;
			for (unsigned k = 0; k < myColumns; ++k)
			{									   //   for each of my columns:
				sum += (*this)[i][k] * mat2[k][j]; //     sum the products
			}
			mat3[i][j] = sum; //     put sum in result
		}
	}
	
	// copy the results to the output matrix (see note above about "terminated by signal 11")
	copy(mat3.myMatrix, mat3.myMatrix + outputSize, returnMatrix.myMatrix);

	return returnMatrix;
}

/** --- utility to allocate space dynamically
 * @param: rows and columns, two unsigned ints.
 * @precondition: rows > 0 && columns > 0.
 * @postcondition: myMatrix points to a rows x columns block of memory.
 */
template <class Item>
inline void Matrix<Item>::allocate(unsigned rows, unsigned columns)
{
	// avoid memory leak
	if (myMatrix != nullptr)
	{
		deallocate();
	}
	// allocate block of memory
	myMatrix = new Item[rows * columns];
}

/** --- utility to deallocate my storage, refactored from
 *      destructor, operator=, and other destructive methods.
 * @postcondition: my storage has been returned to the heap &&
 *                 myMatrix == NULL && myRows == 0 && myColumns == 0.
 */
template <class Item>
inline void Matrix<Item>::deallocate()
{
	// delete the array of Items
	delete[] myMatrix;
	// clean up
	myMatrix = nullptr;
	myRows = myColumns = 0;
}

/** --- initialize the Matrix's elements to a common value
 * @param: value, an Item.
 * @postcondition: all elements in myMatrix == value.
 */
template <class Item>
inline void Matrix<Item>::initialize(Item value)
{
	unsigned size = myRows * myColumns;
	for (unsigned i = 0; i < size; ++i)
	{
		myMatrix[i] = value;
	}
}

/** --- utility method refactored from copy constructor and operator=
 * @param: original, a Matrix
 * @postcondition: I am a distinct copy of original.
 */
template <class Item>
inline void Matrix<Item>::makeCopyOf(const Matrix<Item> &original)
{
	myRows = myColumns = 0;
	myMatrix = nullptr;
	if (original.getRows() != 0)
	{
		unsigned r = original.getRows();
		unsigned c = original.getColumns();
		allocate(r, c);
		unsigned size = r * c;
		for (unsigned i = 0; i < size; ++i)
		{
			myMatrix[i] = original.myMatrix[i];
		}
		myRows = r;
		myColumns = c;
	}
}

#endif

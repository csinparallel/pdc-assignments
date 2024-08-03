/** MatrixTester.cpp tests Matrix operations.
 * Author: Joel C. Adams, for CS 112 at Calvin University.
 * Copyright 2009, Joel C. Adams.  All rights reserved.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University
 * Extended to include tests for Matrix multiplication
 */

#include "MatrixTester.h"

// these tests were designed to run using Matrices filled with doubles (Matrix<double>)
void MatrixTester::runTests()
{
	cout << "Testing class Matrix." << endl;
	testConstructor();
	testEquality();
	testCopyConstructor();
	testAssignment();
	testAddition();
	testTranspose();
	testMultiply();
	cout << "All tests passed!" << endl;
}

void MatrixTester::testConstructor()
{
	cout << "- constructor... " << flush;
	Matrix<double> m0;
	assert(m0.getRows() == 0);
	assert(m0.getColumns() == 0);
	cout << " 0 " << flush;
	Matrix<double> m1(3, 4);
	assert(m1.getRows() == 3);
	assert(m1.getColumns() == 4);
	cout << " 1a " << flush;
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			assert(m1[i][j] == 0);
		}
	}
	cout << " 1b " << flush;

	cout << " Passed!" << endl;
}

void MatrixTester::testEquality()
{
	cout << "- equality... " << flush;
	// empty case
	Matrix<double> m0, m1;
	assert(m0 == m1);
	cout << " 0 " << flush;
	// non-empty but equal case
	Matrix<double> m2, m3;
	m2.fillText("testFiles/mat1.txt");
	m3.fillText("testFiles/mat1.txt");
	assert(m2 == m3);
	cout << " 1 " << flush;
	// nonempty, same size, not equal at first item
	Matrix<double> m4;
	m4.fillText("testFiles/mat2.txt");
	assert(!(m2 == m4));
	cout << " 2a " << flush;
	// nonempty, same size, not equal at last item
	Matrix<double> m5;
	m5.fillText("testFiles/mat3.txt");
	assert(!(m2 == m5));
	cout << " 2b " << flush;
	// nonempty, same size, not equal at interior item
	Matrix<double> m6;
	m6.fillText("testFiles/mat4.txt");
	assert(!(m2 == m6));
	cout << " 2c " << flush;
	// nonempty, different rows
	Matrix<double> m7;
	m7.fillText("testFiles/mat5.txt");
	assert(!(m2 == m7));
	cout << " 3a " << flush;
	// nonempty, different columns
	Matrix<double> m8;
	m8.fillText("testFiles/mat6.txt");
	assert(!(m2 == m8));
	cout << " 3b " << flush;

	cout << "Passed!" << endl;
}

void MatrixTester::testCopyConstructor()
{
	cout << "- copy constructor... " << flush;
	// empty case
	Matrix<double> m0;
	Matrix<double> m1(m0);
	assert(m1.getRows() == 0);
	assert(m1.getColumns() == 0);
	assert(m1.myMatrix == nullptr);
	cout << " 0 " << flush;
	// nonempty
	Matrix<double> m2(3, 4);
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			m2[i][j] = i + j + 10;
		}
	}
	Matrix<double> m3(m2);
	assert(m3.getRows() == 3);
	assert(m3.getColumns() == 4);
	cout << " 1a " << flush;
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(m3[r][c] == r + c + 10);
		}
	}
	cout << " 1b " << flush;
	assert(m3.myMatrix != m2.myMatrix);
	cout << " 1c " << flush;
	cout << " Passed! " << endl;
}

void MatrixTester::testAssignment()
{
	cout << "- assignment... " << flush;
	// empty
	Matrix<double> m0;
	Matrix<double> m1;
	m1 = m0;
	assert(m1.getRows() == 0);
	assert(m1.getColumns() == 0);
	assert(m1.myMatrix == nullptr);
	cout << " 0 " << flush;
	// nonempty to empty
	Matrix<double> m2(3, 4);
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			m2[i][j] = i + j + 10;
		}
	}
	m1 = m2;
	assert(m1.getRows() == 3);
	assert(m1.getColumns() == 4);
	cout << " 1a " << flush;
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(m1[r][c] == r + c + 10);
		}
	}
	cout << " 1b " << flush;
	assert(m1.myMatrix != m2.myMatrix);
	cout << " 1c " << flush;
	// empty to nonempty
	m1 = m0;
	assert(m1.getRows() == 0);
	assert(m1.getColumns() == 0);
	assert(m1.myMatrix == nullptr);
	cout << " 2 " << flush;
	// nonempty to nonempty
	Matrix<double> m3(2, 2);
	m3 = m2;
	assert(m3.getRows() == 3);
	assert(m3.getColumns() == 4);
	cout << " 3a " << flush;
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(m3[r][c] == r + c + 10);
		}
	}
	cout << " 3b " << flush;
	assert(m3.myMatrix != m2.myMatrix);
	cout << " 3c " << flush;
	// chaining
	m3 = m0 = m2;
	assert(m3.getRows() == 3);
	assert(m3.getColumns() == 4);
	cout << " 4a " << flush;
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(m3[r][c] == r + c + 10);
		}
	}
	cout << " 4b " << flush;
	assert(m3.myMatrix != m2.myMatrix);
	assert(m3.myMatrix != m0.myMatrix);
	assert(m0.myMatrix != m2.myMatrix);
	cout << " 4c " << flush;
	// self-assignment
	m3 = m3;
	assert(m3.getRows() == 3);
	assert(m3.getColumns() == 4);
	cout << " 5a " << flush;
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(m3[r][c] == r + c + 10);
		}
	}
	cout << " 5b " << flush;

	cout << " Passed! " << endl;
}

void MatrixTester::testAddition()
{
	cout << "- testing operator+ ... " << flush;
	Matrix<double> m1, m2;
	m1.fillBin("testFiles/4096x4096.bin");
	m2.fillBin("testFiles/4096x4096.bin");
	// change based on the file loaded in above.
	assert( m1.getRows() == 4096 && m1.getColumns() == 4096); 
	assert( m2.getRows() == 4096 && m2.getColumns() == 4096);
	Matrix<double> m3 = m1 + m2;
	assert(m3.getRows() == m1.getRows());
	assert(m3.getColumns() == m1.getColumns());
	for (unsigned i = 0; i < m3.getRows(); ++i)
	{
		for (unsigned j = 0; j < m3.getColumns(); ++j)
		{
			assert(m3[i][j] == 2 * (i * m3.getRows() + j));
		}
	}
	cout << " 1 " << flush;
	Matrix<double> m4;
	try
	{
		Matrix<double> m5 = m4 + m3;
	}
	catch (invalid_argument &)
	{
		cout << " 2 " << flush;
	}
	cout << " Passed!" << endl;
}

void MatrixTester::testTranspose()
{
	cout << "- testing transpose()... " << flush;
	// empty case
	Matrix<double> m0;
	try
	{
		m0.transpose();
	}
	catch (invalid_argument &)
	{
		cout << " 0 " << flush;
	}
	// nonempty
	Matrix<double> m2(3, 4);
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			m2[i][j] = i + j + 10;
		}
	}
	Matrix<double> m1 = m2.transpose();
	assert(m1.getRows() == 4);
	assert(m1.getColumns() == 3);
	cout << " 1a " << flush;
	for (unsigned r = 0; r < 4; ++r)
	{
		for (unsigned c = 0; c < 3; ++c)
		{
			assert(m1[r][c] == r + c + 10);
		}
	}
	cout << " 1b " << flush;
	assert(m1.myMatrix != m2.myMatrix);
	cout << " 1c " << flush;

	cout << " Passed!" << endl;
}

void MatrixTester::testMultiply()
{
	cout << "- testing multiply()... " << flush;

	Matrix<double> m1(3, 2);
	Matrix<double> m2(2, 3);
	// fill with numbers 0 to rows*cols, in ascending order
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 2; ++j)
		{
			m1[i][j] = i * m1.getColumns() + j;
			m2[j][i] = j * m2.getColumns() + i;
		}
	}
	Matrix<double> m3 = m1 * m2;
	assert(m3.getRows() == m1.getRows());
	assert(m3.getColumns() == m2.getColumns());
	cout << " 1a " << flush;

	Matrix<double> m4(3, 3);
	// manually insert the values for testing the resulting matrix
	for (unsigned i = 0; i < m4.getRows(); ++i)
	{
		for (unsigned j = 0; j < m4.getColumns(); ++j)
		{
			unsigned row = i % m4.getRows();
			unsigned col = j % m4.getColumns();
			m4[i][j] = (col + 3) +
					   ((col == 2 ? 14 : col == 1 ? 10
												  : 6) *
						row);
		}
	}
	assert(m3 == m4);
	cout << " 1b " << flush;

	Matrix<double> m5(3, 4);
	Matrix<double> m6(3, 3);
	try
	{
		Matrix<double> m3 = m5 * m6; // invalid multiply, 3x4 by 3x3 (m1.cols != m2.rows)
	}
	catch (invalid_argument &)
	{
		cout << " 2 " << flush;
	}

	cout << " Passed!" << endl;
}

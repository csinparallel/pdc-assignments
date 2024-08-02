/** MatrixTester.c tests Matrix operations.
 * Author: Steven McKelvey, Calvin University
 * Based on a C++ Class written by Joel C. Adams (Calvin University).
 * Adapted for CSinParallel.org.
 */

#include "MatrixTester.h"

void runTests()
{
	printf("Testing Matrix.\n");
	testInitialize();
	testEquality();
	testCopyConstructor();
	testAssignment();
	testAddition();
	testTranspose();
	testMultiply();
	testIndex();
	printf("All tests passed!\n");
}

void testInitialize()
{
	printf("- testing initialization... ");
	fflush(stdout);
	Matrix m0;
	m0 = *initializeEmpty();
	assert(m0.rows == 0);
	assert(m0.columns == 0);
	assert(m0.values == NULL);
	printf(" 0 ");
	fflush(stdout);

	// initialize the same matrix twice
	Matrix m1;
	m1 = *initialize(3, 3);
	assert(m1.rows == 3 && m1.columns == 3);
	m1 = *initialize(2, 2);
	assert(m1.rows == 2 && m1.columns == 2);
	printf(" 1 ");
	fflush(stdout);

	Matrix m2;
	m2 = *initializeZeroes(3, 4);
	assert(m2.rows == 3);
	assert(m2.columns == 4);
	printf(" 2a ");
	fflush(stdout);
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			assert(get(&m2, i, j) == 0);
		}
	}
	printf(" 2b ");
	fflush(stdout);

	m2 = *initializeValues(5, 2, 10);
	assert(m2.rows == 5);
	assert(m2.columns == 2);
	printf(" 3a ");
	fflush(stdout);
	for (unsigned i = 0; i < m2.rows; ++i)
	{
		for (unsigned j = 0; j < m2.columns; ++j)
		{
			assert(get(&m2, i, j) == 10);
		}
	}
	printf(" 3b ");
	fflush(stdout);

	m2 = *initializeAsc(4, 3);
	assert(m2.rows == 4);
	assert(m2.columns == 3);
	printf(" 4a ");
	fflush(stdout);
	for (unsigned i = 0; i < m2.rows; ++i)
	{
		for (unsigned j = 0; j < m2.columns; ++j)
		{
			assert(get(&m2, i, j) == (i * m2.columns + j));
		}
	}
	printf(" 4b ");
	fflush(stdout);

	printf(" Passed!\n");
}

void testEquality()
{
	printf("- equality... ");
	fflush(stdout);
	// empty case
	Matrix *m0 = (Matrix*)calloc(0, sizeof(Matrix)), 
	*m1 = (Matrix*)calloc(0, sizeof(Matrix));
	assert(equal(m0, m1));
	printf(" 0 ");
	fflush(stdout);
	// non-empty but equal case
	Matrix m2, m3;
	fillText("testFiles/mat1.txt", &m2);
	fillText("testFiles/mat1.txt", &m3);
	assert(equal(&m2, &m3));
	printf(" 1 ");
	fflush(stdout);
	// nonempty, same size, not equal at first item
	Matrix m4;
	fillText("testFiles/mat2.txt", &m4);
	assert(!equal(&m2, &m4));
	printf(" 2a ");
	fflush(stdout);
	// nonempty, same size, not equal at last item
	Matrix m5;
	fillText("testFiles/mat3.txt", &m5);
	assert(!equal(&m2, &m5));
	printf(" 2b ");
	fflush(stdout);
	// nonempty, same size, not equal at interior item
	Matrix m6;
	fillText("testFiles/mat4.txt", &m6);
	assert(!equal(&m2, &m6));
	printf(" 2c ");
	fflush(stdout);
	// nonempty, different rows
	Matrix m7;
	fillText("testFiles/mat5.txt", &m7);
	assert(!equal(&m2, &m7));
	printf(" 3a ");
	fflush(stdout);
	// nonempty, different columns
	Matrix m8;
	fillText("testFiles/mat6.txt", &m8);
	assert(!equal(&m2, &m8));
	printf(" 3b ");
	fflush(stdout);

	printf("Passed!\n");

	// free dynamically allocated Matrices
	freeMatrix(m0);
	free(m0);
	freeMatrix(m1);
	free(m1);
}

void testCopyConstructor()
{
	printf("- copy constructor... ");
	fflush(stdout);
	// empty case
	Matrix m0, m1;
	m0 = *initializeEmpty();
	copy(&m1, &m0);
	assert(m1.rows == 0);
	assert(m1.columns == 0);
	assert(m1.values == NULL);
	printf(" 0 ");
	fflush(stdout);
	// nonempty
	Matrix m2;
	m2 = *initialize(3, 4);
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			set(&m2, i, j, i + j + 10);
		}
	}
	Matrix m3;
	m3 = *initializeEmpty();
	copy(&m3, &m2);
	assert(m3.rows == 3);
	assert(m3.columns == 4);
	printf(" 1a ");
	fflush(stdout);
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(get(&m3, r, c) == r + c + 10);
		}
	}
	printf(" 1b ");
	fflush(stdout);
	assert(m3.values != m2.values);
	printf(" 1c ");
	fflush(stdout);
	printf(" Passed! \n");
}

// works with copy(), not with = operator (because we are using pointers?)
void testAssignment()
{
	printf("- assignment... ");
	fflush(stdout);
	// empty
	Matrix m0, m1;
	m0 = *initializeEmpty();
	copy(&m1, &m0);
	// m1 = m0;
	assert(m1.rows == 0);
	assert(m1.columns == 0);
	assert(m1.values == NULL);
	printf(" 0 ");
	fflush(stdout);
	// nonempty to empty
	Matrix m2;
	m2 = *initialize(3, 4);
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			set(&m2, i, j, i + j + 10);
		}
	}
	copy(&m1, &m2);
	// m1 = m2;
	assert(m1.rows == 3);
	assert(m1.columns == 4);
	printf(" 1a ");
	fflush(stdout);
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(get(&m1, r, c) == r + c + 10);
		}
	}
	printf(" 1b ");
	fflush(stdout);
	assert(m1.values != m2.values);
	printf(" 1c ");
	fflush(stdout);
	// empty to nonempty
	copy(&m1, &m0);
	// m1 = m0;
	assert(m1.rows == 0);
	assert(m1.columns == 0);
	assert(m1.values == NULL);
	printf(" 2 ");
	fflush(stdout);
	// nonempty to nonempty
	Matrix m3;
	m3 = *initialize(2, 2);
	copy(&m3, &m2);
	// m3 = m2;
	assert(m3.rows == 3);
	assert(m3.columns == 4);
	printf(" 3a ");
	fflush(stdout);
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(get(&m3, r, c) == r + c + 10);
		}
	}
	printf(" 3b ");
	fflush(stdout);
	assert(m3.values != m2.values);
	printf(" 3c ");
	fflush(stdout);
	// chaining
	copy(&m0, &m2);
	copy(&m3, &m0);
	// m3 = m0 = m2;
	assert(m3.rows == 3);
	assert(m3.columns == 4);
	printf(" 4a ");
	fflush(stdout);
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(get(&m3, r, c) == r + c + 10);
		}
	}
	printf(" 4b ");
	fflush(stdout);
	assert(m3.values != m2.values);
	assert(m3.values != m0.values);
	assert(m0.values != m2.values);
	printf(" 4c ");
	fflush(stdout);
	// self-assignment
	copy(&m3, &m3);
	m3 = m3;
	assert(m3.rows == 3);
	assert(m3.columns == 4);
	printf(" 5a ");
	fflush(stdout);
	for (unsigned r = 0; r < 3; ++r)
	{
		for (unsigned c = 0; c < 4; ++c)
		{
			assert(get(&m3, r, c) == r + c + 10);
		}
	}
	printf(" 5b ");
	fflush(stdout);

	printf(" Passed! \n");
}

void testAddition()
{
	printf("- testing add() ... ");
	fflush(stdout);

	Matrix m1, m2;
	fillBin("testFiles/4096x4096.bin", &m1);
	fillBin("testFiles/4096x4096.bin", &m2);
	// change based on the file loaded in above.
	assert( m1.rows == 4096 && m1.columns == 4096); 
	assert( m2.rows == 4096 && m2.columns == 4096);
	Matrix m3;
	m3 = add(&m1, &m2);
	assert(m3.rows == m1.rows);
	assert(m3.columns == m1.columns);
	for (unsigned i = 0; i < m3.rows; ++i)
	{
		for (unsigned j = 0; j < m3.columns; ++j)
		{
			assert(get(&m3, i, j) == 2 * (i * m3.rows + j));
		}
	}
	printf(" 1 ");
	fflush(stdout);

	Matrix m4, m5;
	m5 = add(&m4, &m3);
	assert(m5.status == 1); // previous add should have failed.
	printf(" 2 ");
	fflush(stdout);
	printf(" Passed!\n");
}

void testTranspose()
{
	printf("- testing transpose()... ");
	fflush(stdout);
	// empty case
	Matrix m0, m1;
	m0 = *initializeEmpty();
	m1 = transpose(&m0);
	assert(m1.status == 1); // previous transpose should have failed
	printf(" 0 ");
	fflush(stdout);
	// nonempty
	Matrix m2;
	m2 = *initialize(3, 4);
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 4; ++j)
		{
			set(&m2, i, j, i + j + 10);
		}
	}
	Matrix m3;
	m3 = transpose(&m2);
	assert(m3.rows == 4);
	assert(m3.columns == 3);
	printf(" 1a ");
	fflush(stdout);
	for (unsigned r = 0; r < 4; ++r)
	{
		for (unsigned c = 0; c < 3; ++c)
		{
			assert(get(&m3, r, c) == r + c + 10);
		}
	}
	printf(" 1b ");
	fflush(stdout);
	assert(m3.values != m2.values);
	printf(" 1c ");
	fflush(stdout);

	printf(" Passed!\n");
}

void testMultiply()
{
	printf("- testing multiply()... ");
	fflush(stdout);

	Matrix m1, m2, m3, m4;

	m1 = *initializeAsc(3, 2);
	m2 = *initializeAsc(2, 3);
	m3 = multiply(&m1, &m2);
	assert(m3.rows == m1.rows);
	assert(m3.columns == m2.columns);
	printf(" 1a ");
	fflush(stdout);

	m4 = *initialize(3, 3);
	// manually insert the values for testing the resulting matrix
	for (int i = 0; i < m4.rows; ++i)
	{
		for (int j = 0; j < m4.columns; ++j)
		{
			int row = i % m4.rows;
			int col = j % m4.columns;
			set(&m4, i, j, (col + 3) + 
			((col == 2 ? 14 : col == 1 ? 10 : 6) *
			row));
		}
	}
	assert(equal(&m3, &m4));
	printf(" 1b ");
	fflush(stdout);

	m1 = *initialize(3, 4);
	m2 = *initialize(3, 3);
	m3 = multiply(&m1, &m2); // invalid multiply, 3x4 by 3x3 (m1.cols != m2.rows)
	assert(m3.status == 1);
	printf(" 2 ");
	fflush(stdout);

	printf(" Passed!\n");
}

void testIndex()
{
	printf("- testing indexing... ");
	fflush(stdout);

	Matrix m0;
	m0 = *initializeAsc(3, 3);
	assert(get(&m0, 0, 0) == 0);
	assert(get(&m0, 2, 1) == 7);
	assert(get(&m0, 1, 2) == 5);
	printf(" 1 ");
	fflush(stdout);

	// essentially assert(getRow(m0, 2) == testArr) for the values within each array
	Item testArr[3] = {6, 7, 8};
	Item *returnedArr = getRow(&m0, 2);
	for (int i = 0; i < 3; ++i)
	{
		assert(testArr[i] == returnedArr[i]);
	}
	printf(" 2 ");
	fflush(stdout);

	printf(" Passed!\n");

	free(returnedArr);
}

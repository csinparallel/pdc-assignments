/** MatrixTester.h declares a class to test Matrix operations.
 * Author: Joel C. Adams, for CS 112 at Calvin University.
 * Copyright 2009, Joel C. Adams.  All rights reserved.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University
 * Extended to include tests for matrix multiplication
 */

#ifndef MATRIXTESTER_H_
#define MATRIXTESTER_H_

#include "Matrix.cuh"

class MatrixTester
{
public:
	void runTests();
	void testConstructor();
	void testEquality();
	void testCopyConstructor();
	void testAssignment();
	void testAddition();
	void testTranspose();
	void testMultiply();
};

#endif /*MATRIXTESTER_H_*/

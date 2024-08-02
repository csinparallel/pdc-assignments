/** MatrixTester.h declares functions for testing Matrix operations.
 * Author: Steven McKelvey, Calvin University
 * Based on a C++ Class written by Joel C. Adams (Calvin University).
 * Adapted for CSinParallel.org.
 */

#ifndef MATRIXTESTER_H_
#define MATRIXTESTER_H_

#include "Matrix.cuh"

void runTests();
void testInitialize();
void testEquality();
void testCopyConstructor();
void testAssignment();
void testAddition();
void testTranspose();
void testMultiply();
void testIndex();

#endif /*MATRIXTESTER_H_*/

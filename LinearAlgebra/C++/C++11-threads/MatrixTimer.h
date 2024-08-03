/** MatrixTimer.h declares a class to record Matrix operations' timing data.
 * Joel Adams, for CS 112 at Calvin University.
 * Copyright Joel C. Adams, 2009.  All Rights Reserved.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University
 * Extended to include timing for matrix multiplication and equality
 */

#ifndef MATRIX_TIMER
#define MATRIX_TIMER

#include <string>
#include <fstream>  // ifstream, ofstream
#include <iostream> // cin, cout, <<, >>
#include <iomanip>  // setw()
#include "Matrix.h" // Matrix class
#include "omp.h"    // OpenMP library
using namespace std;

// Modify the type of "Item" to change the datatype stored in a Matrix (during the time trials, not the tests)
typedef double Item;

class MatrixTimer
{
public:
    MatrixTimer(unsigned numTrials, const string &logFile);
    ~MatrixTimer();
    void run();
    void timeAddition();
    void timeTranspose();
    void timeEquality();
    void timeMultiplication();

protected:
    double average() const;

private:
    unsigned myNumTrials;
    double *myTrialTimes;
    ofstream myLogStream;
    Matrix<Item> myMatrix, myMatrix2;
};

#endif

/** MatrixTimer.h declares a struct to record Matrix operations' timing data.
 * Author: Steven McKelvey, Calvin University
 * Based on a C++ Class written by Joel C. Adams (Calvin University).
 * Adapted for CSinParallel.org.
 */

#ifndef MATRIX_TIMER
#define MATRIX_TIMER

#include "Matrix.cuh" // Matrix class
#include "omp.h"    // OpenMP library

typedef struct MatrixTimer
{
    unsigned myNumTrials;
    double *myTrialTimes;
    FILE *myLogStream;
    Matrix *myMatrix, *myMatrix2;
} MatrixTimer;

void initializeMatrixTimer(MatrixTimer *matrixTimer, unsigned numTrials, const char *logFile);
void freeMatrixTimer(MatrixTimer *matrixTimer);
void matrixTimerRun(MatrixTimer *matrixTimer);
void timeAddition(MatrixTimer *matrixTimer);
void timeTranspose(MatrixTimer *matrixTimer);
void timeEquality(MatrixTimer *matrixTimer);
void timeMultiplication(MatrixTimer *matrixTimer);
double average(MatrixTimer *matrixTimer);

#endif

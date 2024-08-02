/** MatrixTimer.c defines operations that can be performed on MatrixTimer structs.
 * Author: Steven McKelvey, Calvin University
 * Based on a C++ Class written by Joel C. Adams (Calvin University).
 * Adapted for CSinParallel.org.
 */

#include "MatrixTimer.h"

/** initializes a MatrixTimer struct
 * @param matrixTimer: the MatrixTimer to be initialized
 * @param numTrials: the number of time-trials for each operation
 * @param logFile: a file where time-trial data are logged.
 * @postcondition: all member variables have all been initialized.
 */
void initializeMatrixTimer(MatrixTimer *matrixTimer, unsigned numTrials, const char *logFile)
{
    matrixTimer->myNumTrials = numTrials;
    matrixTimer->myTrialTimes = (double *)malloc(sizeof(double) * numTrials);
    if (strcmp(logFile, "") && logFile != NULL)
    { // if the logfile name is not "" and is not NULL
        matrixTimer->myLogStream = fopen(logFile, "w");
    }
    printf("\nLoading matrices...");
    matrixTimer->myMatrix = (Matrix *)malloc(sizeof(Matrix));
    matrixTimer->myMatrix2 = (Matrix *)malloc(sizeof(Matrix));

    fillText("testFiles/1024x1024.txt", matrixTimer->myMatrix);
    fillText("testFiles/1024x1024.txt", matrixTimer->myMatrix2);
}

/** destructor replacement. For closing the file and freeing memory.
 * @postcondition: my dynamic array has been returned to the heap &&
 *                 my log file has been closed.
 */
void freeMatrixTimer(MatrixTimer *matrixTimer)
{
    free(matrixTimer->myTrialTimes);
    // free the members of each matrix
    freeMatrix(matrixTimer->myMatrix);
    freeMatrix(matrixTimer->myMatrix2);
    // free the matrices themselves
    free(matrixTimer->myMatrix);
    free(matrixTimer->myMatrix2);
    if (matrixTimer->myLogStream != NULL)
    {
        fclose(matrixTimer->myLogStream);
    }
}

/** method to run time-trials
 * @postcondition: my time-trials have been run.
 */
void matrixTimerRun(MatrixTimer *matrixTimer)
{
    printf("\n");
    timeAddition(matrixTimer);
    timeTranspose(matrixTimer);
    timeEquality(matrixTimer);
    timeMultiplication(matrixTimer); // using matrices of size 1024x1024 still takes a while (12 seconds on average per sequential iteration)
    printf("\n");
}

/** time the addition operation
 * @postcondition: the addition operation has been performed
 *                      myNumTrials times, &&
 *                 myLogStream contains the time-trial times and
 *                      their average &&
 *                 an asterisk has been display on the screen
 *                      for each time-trial &&
 *                 their average has been displayed on the screen.
 */
void timeAddition(MatrixTimer *matrixTimer)
{
    printf("Timing Matrix addition: \t");
    fflush(stdout);
    fprintf(matrixTimer->myLogStream, "\nTiming Matrix addition ...\n");
    double start = 0, stop = 0;
    for (unsigned i = 0; i < matrixTimer->myNumTrials; ++i)
    {
        printf(" * ");
        fflush(stdout);
        start = omp_get_wtime();
        add(matrixTimer->myMatrix, matrixTimer->myMatrix2);
        stop = omp_get_wtime();
        matrixTimer->myTrialTimes[i] = stop - start;
        fprintf(matrixTimer->myLogStream, "\nTrial %d: %lf", i, matrixTimer->myTrialTimes[i]);
    }
    double avgTime = average(matrixTimer);
    printf("\t%7.5lf\n", avgTime);
    fprintf(matrixTimer->myLogStream, "\nAverage: %7.5lf\n", avgTime);
}

/** utility method to average the time-trials in myTrialTimes.
 * @precondition: myNumTrials > 0 &&
 *                myTrialTimes contains that many trial-times
 * @return: the average of the trial-times in myTrialTimes.
 */
double average(MatrixTimer *matrixTimer)
{
    double sum = 0;
    for (unsigned i = 0; i < matrixTimer->myNumTrials; ++i)
    {
        sum += matrixTimer->myTrialTimes[i];
    }
    return sum / matrixTimer->myNumTrials;
}

/** time the transpose operation
 * @postcondition: the transpose operation has been performed
 *                      myNumTrials times, &&
 *                 myLogStream contains the time-trial times and
 *                      their average &&
 *                 an asterisk has been display on the screen
 *                      for each time-trial &&
 *                 their average has been displayed on the screen.
 */
void timeTranspose(MatrixTimer *matrixTimer)
{
    printf("Timing Matrix transpose: \t");
    fflush(stdout);
    fprintf(matrixTimer->myLogStream, "\nTiming Matrix transpose ...\n");
    double start = 0, stop = 0;
    for (unsigned i = 0; i < matrixTimer->myNumTrials; ++i)
    {
        printf(" * ");
        fflush(stdout);
        start = omp_get_wtime();
        transpose(matrixTimer->myMatrix);
        stop = omp_get_wtime();
        matrixTimer->myTrialTimes[i] = stop - start;
        fprintf(matrixTimer->myLogStream, "\nTrial %d: %lf", i, matrixTimer->myTrialTimes[i]);
    }
    double avgTime = average(matrixTimer);
    printf("\t%7.5lf\n", avgTime);
    fprintf(matrixTimer->myLogStream, "\nAverage: %7.5lf\n", avgTime);
}

/** time the boolean equality operation
 * @postcondition: the boolean equality operation has been performed
 *                      myNumTrials times, &&
 *                 myLogStream contains the time-trial times and
 *                      their average &&
 *                 an asterisk has been display on the screen
 *                      for each time-trial &&
 *                 their average has been displayed on the screen.
 */
void timeEquality(MatrixTimer *matrixTimer)
{
    printf("Timing Matrix equality: \t");
    fflush(stdout);
    fprintf(matrixTimer->myLogStream, "\nTiming Matrix equality ...\n");
    double start = 0, stop = 0;
    for (unsigned i = 0; i < matrixTimer->myNumTrials; ++i)
    {
        printf(" * ");
        fflush(stdout);
        start = omp_get_wtime();
        equal(matrixTimer->myMatrix, matrixTimer->myMatrix2);
        stop = omp_get_wtime();
        matrixTimer->myTrialTimes[i] = stop - start;
        fprintf(matrixTimer->myLogStream, "\nTrial %d: %lf", i, matrixTimer->myTrialTimes[i]);
    }
    double avgTime = average(matrixTimer);
    printf("\t%7.5lf\n", avgTime);
    fprintf(matrixTimer->myLogStream, "\nAverage: %7.5lf\n", avgTime);
}

/** time the multiplication operation
 * @postcondition: the multiplication operation has been performed
 *                      myNumTrials times, &&
 *                 myLogStream contains the time-trial times and
 *                      their average &&
 *                 an asterisk has been display on the screen
 *                      for each time-trial &&
 *                 their average has been displayed on the screen.
 */
void timeMultiplication(MatrixTimer *matrixTimer)
{
    printf("Timing Matrix multiplication: \t");
    fflush(stdout);
    fprintf(matrixTimer->myLogStream, "\nTiming Matrix multiplication ...\n");
    double start = 0, stop = 0;
    for (unsigned i = 0; i < matrixTimer->myNumTrials; ++i)
    {
        printf(" * ");
        fflush(stdout);
        start = omp_get_wtime();
        multiply(matrixTimer->myMatrix, matrixTimer->myMatrix2);
        stop = omp_get_wtime();
        matrixTimer->myTrialTimes[i] = stop - start;
        fprintf(matrixTimer->myLogStream, "\nTrial %d: %lf", i, matrixTimer->myTrialTimes[i]);
    }
    double avgTime = average(matrixTimer);
    printf("\t%7.5lf\n", avgTime);
    fprintf(matrixTimer->myLogStream, "\nAverage: %7.5lf\n", avgTime);
}
/** MatrixTimer.cpp defines operations that can be performed on instances of MatrixTimer.
 * Joel Adams, for CS 112 at Calvin University.
 * Copyright Joel C. Adams, 2009.  All Rights Reserved.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University
 * Extended to include timing for matrix multiplication and equality
 */

#include "MatrixTimer.cuh"

/** explicit-value constructor
 * @param: numTrials, the number of time-trials for each operation
 *             logFile, a file where time-trial data are logged.
 * @postcondition: my instance variables have all been initialized.
 */

MatrixTimer::MatrixTimer(unsigned numTrials, const string &logFile)
{
    myNumTrials = numTrials;
    myTrialTimes = new double[numTrials];
    if (logFile != "")
    {
        myLogStream.open(logFile.c_str());
    }
    cout << "\nLoading matrices..." << flush;
    // the datatype of the values stored in these testFiles must be the same as the Item type in MatrixTimer.h
    myMatrix.fillText("testFiles/1024x1024.txt");
    myMatrix2.fillText("testFiles/1024x1024.txt");
}

/** destructor
 * @postcondition: my dynamic array has been returned to the heap &&
 *                 my log file has been closed.
 */
MatrixTimer::~MatrixTimer()
{
    delete[] myTrialTimes;
    if (myLogStream.is_open())
    {
        myLogStream.close();
    }
}

/** method to run time-trials
 * @postcondition: my time-trials have been run.
 */
void MatrixTimer::run()
{
    cout << endl;
    timeAddition();
    timeTranspose();
    timeEquality();
    timeMultiplication();
    cout << endl;
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
void MatrixTimer::timeAddition()
{
    cout << "Timing Matrix addition: \t" << flush;
    myLogStream << fixed << "\nTiming Matrix addition ...\n";
    double start = 0, stop = 0;
    for (unsigned i = 0; i < myNumTrials; ++i)
    {
        cout << " * " << flush;
        start = omp_get_wtime();
        myMatrix + myMatrix2;
        stop = omp_get_wtime();
        myTrialTimes[i] = stop - start;
        myLogStream << "\nTrial " << i << ": " << myTrialTimes[i];
    }
    double avgTime = average();
    cout << '\t' << fixed << setw(7) << setprecision(5) << avgTime << endl;
    myLogStream << "\nAverage: " << fixed << setw(7) << setprecision(5)
                << avgTime << endl;
}

/** utility method to average the time-trials in myTrialTimes.
 * @precondition: myNumTrials > 0 &&
 *                myTrialTimes contains that many trial-times
 * @return: the average of the trial-times in myTrialTimes.
 */
double MatrixTimer::average() const
{
    double sum = 0;
    for (unsigned i = 0; i < myNumTrials; ++i)
    {
        sum += myTrialTimes[i];
    }
    return sum / myNumTrials;
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
void MatrixTimer::timeTranspose()
{
    cout << "Timing Matrix transpose: \t" << flush;
    myLogStream << "\nTiming Matrix transpose ...\n";
    double start = 0, stop = 0;
    for (unsigned i = 0; i < myNumTrials; ++i)
    {
        cout << " * " << flush;
        start = omp_get_wtime();
        myMatrix.transpose();
        stop = omp_get_wtime();
        myTrialTimes[i] = stop - start;
        myLogStream << "\nTrial " << i << ": " << myTrialTimes[i];
    }
    double avgTime = average();
    cout << '\t' << fixed << setw(7) << setprecision(5) << avgTime << endl;
    myLogStream << "\nAverage: " << fixed << setw(7) << setprecision(5)
                << avgTime << endl;
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
void MatrixTimer::timeEquality()
{
    cout << "Timing Matrix equality: \t" << flush;
    myLogStream << fixed << "\nTiming Matrix equality ...\n";
    double start = 0, stop = 0;
    for (unsigned i = 0; i < myNumTrials; ++i)
    {
        cout << " * " << flush;
        start = omp_get_wtime();
        myMatrix == myMatrix2;
        stop = omp_get_wtime();
        myTrialTimes[i] = stop - start;
        myLogStream << "\nTrial " << i << ": " << myTrialTimes[i];
    }
    double avgTime = average();
    cout << '\t' << fixed << setw(7) << setprecision(5) << avgTime << endl;
    myLogStream << "\nAverage: " << fixed << setw(7) << setprecision(5)
                << avgTime << endl;
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
void MatrixTimer::timeMultiplication()
{
    cout << "Timing Matrix multiplication: \t" << flush;
    myLogStream << fixed << "\nTiming Matrix multiplication ...\n";
    double start = 0, stop = 0;
    for (unsigned i = 0; i < myNumTrials; ++i)
    {
        cout << " * " << flush;
        start = omp_get_wtime();
        myMatrix *myMatrix2;
        stop = omp_get_wtime();
        myTrialTimes[i] = stop - start;
        myLogStream << "\nTrial " << i << ": " << myTrialTimes[i];
    }
    double avgTime = average();
    cout << '\t' << fixed << setw(7) << setprecision(5) << avgTime << endl;
    myLogStream << "\nAverage: " << fixed << setw(7) << setprecision(5)
                << avgTime << endl;
}

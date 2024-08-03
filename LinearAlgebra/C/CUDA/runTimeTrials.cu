/** runTimeTrials.cpp uses MatrixTimer to time different Matrix operations.
 * Joel C. Adams, for CS 112 at Calvin University
 * Converted from C++ to C by Steven McKelvey, Calvin University
 * Adapted for CSinParallel.org.
 */

#include "MatrixTimer.cuh"
#include "Matrix.cuh"
#include <unistd.h>

char *buildLogFileName(unsigned numTrials);

int main()
{
	unsigned numTrials = 10; // number of trials to average
	char *logFileName = buildLogFileName(numTrials);

	MatrixTimer *matrixTimer = (MatrixTimer *)malloc(sizeof(MatrixTimer));
	initializeMatrixTimer(matrixTimer, numTrials, logFileName);
	matrixTimerRun(matrixTimer);

	freeMatrixTimer(matrixTimer); // free the members of matrixTimer
	free(matrixTimer);
	free(logFileName);
}

/** utility function to build unique log-file names
 * @param: numTrials, the number of trials to be performed
 * @return: a unique file name incorporating numTrials (returnString)
 */
char *buildLogFileName(unsigned numTrials)
{
	char emptyString[1] = "";
	char buf[16];
	sprintf(buf, "%02d", numTrials);
	char prefix[24];
	sprintf(prefix, "%strials-%s", emptyString, buf);
	char *result = (char *)malloc(sizeof(char) * 40);
	FILE *fin;
	unsigned count = 0;
	while (true)
	{
		++count;
		sprintf(buf, "-%03d.txt", count);
		sprintf(result, "%s%s", prefix, buf);
		if (access(result, F_OK) == -1)
		{ // if the file (log) does not already exist
			// create a new logfile for this set of trials
			fin = fopen(result, "w");
			break;
		}
		// otherwise, keep looping until you get to the next available log file name
	}
	fclose(fin);
	return result;
}
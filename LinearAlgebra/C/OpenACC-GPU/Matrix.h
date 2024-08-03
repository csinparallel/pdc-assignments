/** Matrix.h defines a simple Matrix struct, built using a 1D array.
 * Functions that operate on these structs are also declared (and defined in Matrix.c)
 * Author: Steven McKelvey (Calvin University)
 * Based on a C++ solution by Joel C. Adams (Calvin University)
 * For: CSinParallel.org
 ****************************************************************/

#ifndef MATRIX
#define MATRIX

#include <omp.h>     // openMP
#include <stdio.h>   // printf
#include <string.h>  // rsize_t
#include <stdlib.h>  // malloc
#include <stdbool.h> // bool type
#include <assert.h>  // assert()

/* As long as you create a new file for the 4096x4096 file in tester.c,
    the tests will still pass for most basic, built-in types.
    Exceptions to this: signed/unsigned short, char */
typedef double Item; // User-defined type in Matrix.h
#define ItemFormat "%lf" // Adjust based on the type of "Item"

typedef struct Matrix
{
    int rows;
    int columns;
    Item *values;
    int status; // for returning errors
} Matrix;

// function declarations
Matrix *initialize(int rows, int columns);
Matrix *initializeZeroes(int rows, int columns);
Matrix *initializeValues(int rows, int columns, Item value);
Matrix *initializeAsc(int rows, int columns);
Matrix *initializeEmpty();
void freeMatrix(Matrix *matrix);
void copy(Matrix *newMatrix, Matrix *original);
void print(Matrix *matrix, FILE *stream);
Matrix add(const Matrix *matrix1, const Matrix *matrix2);
Matrix transpose(Matrix *inputMatrix);
bool equal(const Matrix *matrix1, const Matrix *matrix2);
// bool approxEqual(double d1, double d2);
Matrix multiply(const Matrix *matrix1, const Matrix *matrix2);

void fillText(const char *filename, Matrix *matrix);
void dumpText(const char *filename, Matrix *matrix);
void fillBin(const char *filename, Matrix *matrix);
void dumpBin(const char *filename, Matrix *matrix);

Item *getRow(const Matrix *m, int rowIndex);
Item get(const Matrix *m, int rowIndex, int colIndex);
void set(Matrix *m, int rowIndex, int colIndex, Item value);

#endif

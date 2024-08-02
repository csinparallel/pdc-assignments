/** Matrix.c defines several operations that can
 * be performed on the Matrix struct defined in Matrix.h,
 * Author: Steven McKelvey (Calvin University)
 * Based on a C++ solution by Joel C. Adams (Calvin University)
 * For: CSinParallel.org
 ****************************************************************/

#include "Matrix.h"

#define VERBOSE_ERRORS 1 // set to 0 to remove error messages. Makes ./tester output cleaner

/** Initialize a matrix of size rows x columns. A factory method for creating new matrices
 *  @param rows, @param columns: the size of the matrix
 *  @return: a newly created Matrix*
 */
Matrix *initialize(int rows, int columns)
{
    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));

    matrix->rows = rows;
    matrix->columns = columns;
    matrix->values = (Item *)malloc(rows * columns * sizeof(Item));
    matrix->status = 0;

    return matrix;
}

/** Initialize a matrix of size rows x columns to the number 0
 *  @param rows, @param columns: the size of the matrix
 *  @return: a newly created Matrix*
 */
Matrix *initializeZeroes(int rows, int columns)
{
    Matrix *matrix;
    matrix = initializeValues(rows, columns, 0);

    return matrix;
}

/** Initialize a matrix of size rows x columns to the number <value>
 *  @param rows, @param columns: the size of the matrix
 *  @param value: the value that each element of the matrix will be initialized to
 *  @return: a newly created Matrix*
 */
Matrix *initializeValues(int rows, int columns, Item value)
{
    Matrix *matrix;
    matrix = initialize(rows, columns);

    // assign values
    for (unsigned i = 0; i < rows; ++i)
    {
        for (unsigned j = 0; j < columns; ++j)
        {
            set(matrix, i, j, value);
        }
    }

    return matrix;
}

/** Initialize a matrix of size rows x columns
 *  to the numbers 0 through (rows * columns) -- in ascending order
 *  @param rows, @param columns: the size of the matrix
 *  @return: a newly created Matrix*
 */
Matrix *initializeAsc(int rows, int columns)
{
    Matrix *matrix;
    matrix = initialize(rows, columns);

    // assign values
    for (unsigned i = 0; i < rows; ++i)
    {
        for (unsigned j = 0; j < columns; ++j)
        {
            set(matrix, i, j, (i * columns + j));
        }
    }

    return matrix;
}

/** Initialize a matrix of size 0 x 0 with no values
 *  @return: a newly created Matrix*
 */
Matrix *initializeEmpty()
{
    Matrix *matrix;
    matrix = initialize(0, 0);
    // set the values to null
    free(matrix->values);
    matrix->values = NULL;
    assert(matrix->values == NULL);

    return matrix;
}

/** destructor replacement. For freeing memory of the Matrix struct's members.
 * @postcondition: my dynamic array has been returned to the heap
 */
void freeMatrix(Matrix *matrix)
{
    free(matrix->values);
}

/** copy
 * @param newMatrix, the matrix to copy into
 * @param original: the matrix to be copied from
 * @precondition: matrix1->rows == matrix1->rows &&
 *               matrix1->columns == matrix1->columns.
 * @return: result, a Matrix with the same data as original
 *         is contained within newMatrix.
 */
void copy(Matrix *newMatrix, Matrix *original)
{
    if (newMatrix == original)
        return; // if you are copying to self.
    newMatrix->rows = original->rows;
    newMatrix->columns = original->columns;
    newMatrix->values = (Item *)malloc(newMatrix->rows * newMatrix->columns * sizeof(Item));

    if (original->values == NULL)
    {
        newMatrix->values = NULL;
    }
    else
    {
        // we want our own copy of the values, not a pointer to the same memory used by original
        int index;
        for (int matrixRows = 0; matrixRows < newMatrix->rows; ++matrixRows)
        {
            for (int matrixCols = 0; matrixCols < newMatrix->columns; ++matrixCols)
            {
                index = matrixRows * newMatrix->columns + matrixCols;
                newMatrix->values[index] = original->values[index];
            }
        }
    }
}

/** Prints a given matrix to stdout
 *  @param matrix: the matrix to be printed
 */
void print(Matrix *matrix, FILE *stream)
{
    // print an error if the matrix has not been initialized correctly
    if (matrix->values == NULL)
        fprintf(stderr, "print(): matrix has not been initialized");
    int rows = matrix->rows;
    int cols = matrix->columns;
    for (unsigned i = 0; i < rows; ++i)
    {
        for (unsigned j = 0; j < cols; ++j)
        {
            fprintf(stream, ItemFormat "\t", get(matrix, i, j));
        }
        printf("\n");
    }
}

/** addition
 * @param matrix1, @param matrix2: matrices to be added together
 * @precondition: matrix1->rows == matrix1->rows &&
 *               matrix1->columns == matrix1->columns.
 * @return: result, a Matrix with the same dimensions as matrix1
 *          and containing the sum of matrix1 and matrix2.
 */
Matrix add(const Matrix *matrix1, const Matrix *matrix2)
{
    int matrixRows = matrix1->rows;
    int matrixColumns = matrix2->columns;

    Matrix outputMatrix;
    outputMatrix = *initialize(matrixRows, matrixColumns);

    // return an error if the matrix dimensions are invalid
    if (matrix1->rows != matrix2->rows || matrix1->columns != matrix2->columns)
    {
        if (VERBOSE_ERRORS != 0)
            fprintf(stderr, "add(): matrix dimensions mismatch");
        outputMatrix.status = 1;
        return outputMatrix;
    }

    /* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the struct AND its array member (matrix.values)
	*/
    // matrix addition
    for (unsigned rows = 0; rows < matrixRows; ++rows)
    {
        for (unsigned cols = 0; cols < matrixColumns; ++cols)
        {
            Item sum = get(matrix1, rows, cols) + get(matrix2, rows, cols);
            set(&outputMatrix, rows, cols, sum);
        }
    }
    return outputMatrix;
}

/** transpose
 * @param inputMatrix: the matrix to be transposed
 * @return: result, containing a inputMatrix->columns by inputMatrix->rows Matrix,
 *                whose column values are the input matrix's row values
 */
Matrix transpose(Matrix *inputMatrix)
{
    int matrixRows = inputMatrix->rows;
    int matrixColumns = inputMatrix->columns;

    Matrix outputMatrix;
    outputMatrix = *initialize(matrixColumns, matrixRows);

    // return an error if the matrix has not been initialized correctly
    if (inputMatrix->rows <= 0)
    {
        if (VERBOSE_ERRORS != 0)
            fprintf(stderr, "transpose(): empty Matrix!");
        outputMatrix.status = 1;
        return outputMatrix;
    }

    /* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the struct AND its array member (matrix.values)
	*/
    // transpose matrix
    for (unsigned rows = 0; rows < matrixRows; ++rows)
    {
        for (unsigned cols = 0; cols < matrixColumns; ++cols)
        {
            set(&outputMatrix, cols, rows, get(inputMatrix, rows, cols));
        }
    }

    return outputMatrix;
}

/** --- boolean equality
 * @param matrix1, @param matrix2, matrices to be compared.
 * @postcondition: true is returned, if and only if
 *                 matrix1->rows == matrix2->rows &&
 *                 matrix1->columns == matrix2->columns &&
 *                 matrix1's values are the same as matrix2's values.
 */
bool equal(const Matrix *matrix1, const Matrix *matrix2)
{
    // check if the size of each matrix is equal
    if (matrix1->rows != matrix2->rows || matrix1->columns != matrix2->columns)
        return false;
    if (matrix1->columns == 0 && matrix1->rows == 0)
        return true; // empty matrices

    int rows = matrix1->rows;
    int cols = matrix1->columns;

    /* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the struct AND its array member (matrix.values)
	*/
    // check equality
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (get(matrix1, i, j) != get(matrix2, i, j))
            {
                return false;
            }
        }
    }

    return true;
}

/** approxEqual
 * Compares two doubles and returns true if they are equal
 * up to a certain level of precision (determined by THRESHOLD).
 * Useful when using a file (such as a text file) that might not
 * store the exact, original precision of a value.
 *
 * To use this function, replace
 *      "get(matrix1, i, j) != get(matrix2, i, j)"
 * in the above "equal()" function with
 *      "!approxEqual(get(matrix1, i, j), get(matrix2, i, j))"
 *
 */
// bool approxEqual(double d1, double d2)
// {
//     double THRESHOLD = .00001;
//     return abs(d1 - d2) < THRESHOLD;
// }

/** --- multiplication
 * @param matrix1, @param matrix2, matrices to be multiplied.
 * @precondition: matrix1->columns == matrix2->rows
 * @postcondition: a Matrix is returned containing the product of
 *                 the values from matrix1 and matrix2.
 */
Matrix multiply(const Matrix *matrix1, const Matrix *matrix2)
{
    int matrixRows = matrix1->rows;
    int matrixColumns = matrix2->columns;

    // build result Matrix
    Matrix outputMatrix;
    outputMatrix = *initialize(matrixRows, matrixColumns);

    // return an error if the matrix dimensions are invalid
    if (matrix1->columns != matrix2->rows)
    {
        if (VERBOSE_ERRORS != 0)
            fprintf(stderr, "multiply(): invalid dimensions");
        outputMatrix.status = 1;
        return outputMatrix;
    }

    /* HINT: to transfer a Matrix's data to the device, you need to:
		- copy over the struct AND its array member (matrix.values)
	*/
    // matrix multiply
    for (unsigned i = 0; i < matrix1->rows; ++i)
    { // for each row in mat1:
        for (unsigned j = 0; j < matrix2->columns; ++j)
        { //  for each col in mat2:
            Item sum = 0;
            //   for each of my columns...
            for (unsigned k = 0; k < matrix1->columns; ++k)
            {
                sum += get(matrix1, i, k) * get(matrix2, k, j); //     sum the products...
            }
            set(&outputMatrix, i, j, sum); //     and put the sum in result
        }
    }

    return outputMatrix; // return result matrix
}

/** --- input from a text file
 * @param filename: a char* that is the name of (or path to) a file.
 * @param matrix: the matrix to be filled.
 * @precondition: filename is the name of (or path to) a file,
 *                 the first line of file is #rows #columns,
 *                 and the remainder of which is rows x columns values.
 * @postcondition: matrix->rows == rows && matrix->columns == columns &&
 *                 matrix contains the other values from filename.
 */
void fillText(const char *filename, Matrix *matrix)
{
    FILE *fin;
    fin = fopen(filename, "r");
    assert(fin != NULL);

    // process the information from the file
    int numRows, numCols = 0;

    // get the file header (size of matrix)
    fscanf(fin, " %d %d", &numRows, &numCols);

    // resize the matrix so that it can hold what is in the file
    *matrix = *initialize(numRows, numCols); // and dereference each side so that the original pointer (passed in as arg) is not overwritten

    // fill the matrix with the contents of the file.
    for (int i = 0; i < matrix->rows * matrix->columns; ++i)
    {
        fscanf(fin, " " ItemFormat, &matrix->values[i]);
    }

    fclose(fin);
}

/** --- input from a binary file
 * @param filename: a char* that is the name of (or path to) a file.
 * @param matrix: the matrix to be filled.
 * @precondition: filename is the name of (or path to) a file,
 *                 the beginning of file is #rows #columns,
 *                 and the remainder of which is rows x columns values.
 * @postcondition: matrix->rows == rows && matrix->columns == columns &&
 *                 matrix contains the other values from filename.
 */
void fillBin(const char *filename, Matrix *matrix)
{
    FILE *fin;
    fin = fopen(filename, "rb");
    assert(fin != NULL);

    // get the file header (size of matrix)
    int numRows = 0, numCols = 0;
    fread(&numRows, sizeof(int), 1, fin);
    fread(&numCols, sizeof(int), 1, fin);

    // resize the matrix so that it can hold what is in the file
    *matrix = *initialize(numRows, numCols); // and dereference each side so that the original pointer (passed in as arg) is not overwritten

    // fill the matrix with the contents of the file.
    fread(matrix->values, sizeof(Item), matrix->rows * matrix->columns, fin);

    fclose(fin);
    return;
}

/** --- output to a text file
 * @param filename: a char* that is the name of (or path to) a file.
 * @param matrix: the matrix to be dumped.
 * @postcondition: the file named fileName contains matrix->rows and matrix->columns on the first line,
 *                and each subsequent line contains one of the rows from matrix.
 */
void dumpText(const char *filename, Matrix *matrix)
{
    FILE *fout;
    fout = fopen(filename, "w");
    assert(fout != NULL);

    int rows = matrix->rows;
    int columns = matrix->columns;
    fprintf(fout, "%d %d\n", rows, columns); // the file header: the width/height of the matrix.

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            fprintf(fout, ItemFormat, matrix->values[(i * columns) + j]); // print the value to the file

            // only print a newline if we aren't on the last number of the last line.
            if (i != rows - 1 || j != columns - 1)
            {
                fprintf(fout, "%s", (j == (columns - 1) ? "\n" : "\t")); // print a tab/newline to the file
            }
        }
    }
    fclose(fout);
}

/** --- output to a binary file
 * @param filename: a char* that is the name of (or path to) a file.
 * @param matrix: the matrix to be dumped.
 * @postcondition: the file named fileName contains matrix->rows and matrix->columns at the beginning,
 *                and the subsequent data contains each of the rows from matrix.
 */
void dumpBin(const char *filename, Matrix *matrix)
{
    FILE *fout;
    fout = fopen(filename, "wb");
    assert(fout != NULL);

    // write the size (rows/columns) of the matrix
    assert(fwrite(&(matrix->rows), sizeof(int), 1, fout) == 1);
    assert(fwrite(&(matrix->columns), sizeof(int), 1, fout) == 1);

    // write the values of the matrix
    fwrite(matrix->values, sizeof(Item), matrix->rows*matrix->columns, fout);

    fclose(fout);
}

/* Get a row of values at a given row index at [row,] */
Item *getRow(const Matrix *m, int rowIndex)
{
    int numCols = m->columns;
    Item *subsection = (Item *)malloc(numCols * sizeof(Item));
    for (int i = 0; i < numCols; ++i)
    {
        subsection[i] = m->values[rowIndex * numCols + i]; // get the value of each col in the row
    }
    return subsection;
}

/* Get a value at a given index at [row, column] */
Item get(const Matrix *m, int rowIndex, int colIndex)
{
    return m->values[rowIndex * m->columns + colIndex];
}

/* Set the value at index [row, column] to "value" */
void set(Matrix *m, int rowIndex, int colIndex, Item value)
{
    m->values[rowIndex * m->columns + colIndex] = value;
}
#pragma once
#define MATRIX
#include <stdbool.h>
#include <stdio.h>

typedef double MatrixType;
typedef struct Matrix
{
    MatrixType** matrix;
    int row;
    int col;
}   Matrix;

typedef int* Sequence;

/****************************
        Generate/Free
****************************/

/*  Generates matrix. Returned matrix is not initialized. Need to be 'free' after use.
* @param row - Row of matrix
* @prama col - Coloum of matrix
*/
Matrix getMatrix(const int row, const int col);

/*  Free matrix memory.
* @param m - Matrix "adress" which you want to free.
*/
void freeMatrix(Matrix* m);

/****************************
            I/O
****************************/
/*  Input matrix.
* @param input_m - matrix object that want to input.
*/
void inputMatrix(Matrix* input_m);

/*  Generate new matrix and get input.
* @param row - row of matrix
* @param col - column of matrix
*/
Matrix inputNewMatrix(const int row, const int col);

/*  Prints matrix. Print "The matrix is empty." when the matrix is empty.
* @param m - matrix that you want to print
*/
void printMatrix(const Matrix m);

/*  Prints matrix with message. Print "The matrix is empty." when the matrix is empty.
* @param message - Massage that you want to print before printing matrix
* @param m - Matrix that you want to print
*/
void printMessageMatrix(const char* message, const Matrix m);

void fprintMatrix(const FILE* file, const Matrix m);

void fprintMessageMatrix(const FILE* file, const char* message, const Matrix m);

/****************************
            Utility
****************************/

/*  null matrix
* @return - null matrix
*/
Matrix nullMatrix();

/* Check whether the matrix is null matrix or not.
* @return - 1: null matrix, 0: not null matrix
*/
bool isNullMatrix(Matrix m);

/*  Move matrix from m_source to m_target
* @param m_target - target of moving
* @param m_source - source of moving
*/
void moveMatrix(Matrix* m_target, Matrix* m_source);

/*  Swap between two matrixes.
* @param m1 - first Matrix pointer for swap
* @param m2 - second Matrix pointer for swap
*/
void swapMatrix(Matrix* m1, Matrix* m2);

Matrix deepCopyMatrix(Matrix m);

/*	Put a random numbers(1 ~ n) in given matrix. Use time(NULL) as a seed.
* @param m - Empty matrix
* @param n - End Range of random number
*/
void randomMatrix(const Matrix m, const int n);

/*	Generates matrix initialized with random numbers. Random range is 1~ n. Use time(NULL) as a seed.
* @param row - Row of matrix
* @prama col - Coloum of matrix
* @param n - End Range of random number
*/
Matrix getRandomMatrix(const int row, const int col, const int n);

/****************************
          Operators
****************************/

/*  Executes matrix product. Returns NULL matrix when matrix product is not available.
* @param m1 - First matrix
* @param m2 - Second matrix
*/
Matrix matrixProduct(const Matrix m1, const Matrix m2);

/*  Recursivly executes product of multiple matrixes according to sequence.
* @param m_array - Array of matrix that want to product
* @param array_size - Size of array
* @param sequence - Sequence of product
* @return - Result of multiple production of matrix. Null matrix if sequence is not safe.
*/
Matrix multipleProduct(const Matrix* m_array, const int array_size, Sequence sequence);

/*  Check whether 2 matrixes are equal.
* @param m1 - First matrix
* @param m2 - Second matrix
*/
bool matrixEqual(Matrix m1, Matrix m2);

/****************************
          Solution
****************************/

/* Print sequence
*/
void printSequence(Sequence sequence, const int seq_size);

int sumProductCost(Matrix* m_array, const int array_size, Sequence sequence, Matrix* result_p);
// Base Version: Exhaustive search
/*  
* Premise: Every multiple operations has same cost regardless of elements' size.
* @param array_size - Size of array
* @param m_array - Array of matrix that want to product
* @return - product sequence that has smallest costs by using recursion
*/
Sequence findLowestCost_BaseVersion(
    const Matrix* m_array, 
    const int array_size, 
    Matrix* result_p, 
    int* cost
);

// Advenced version
Sequence findLowestCost_AdvencedVersion(
    const Matrix* m_array,
    const int array_size,
    Matrix* result_p,
    int* cost
);
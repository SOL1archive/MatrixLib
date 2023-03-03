#define _CRT_SECURE_NO_WARNINGS
#include "Matrix.h"

#include <limits.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

/****************************
        Generate/Free
****************************/

Matrix getMatrix(const int row, const int col)
{
    Matrix m;
    m.matrix = (MatrixType**)malloc(sizeof(MatrixType*) * row);
    for (int i = 0; i < row; i++)
    {
        m.matrix[i] = (Matrix*)malloc(sizeof(MatrixType) * col);
    }
    m.col = col;
    m.row = row;

    return m;
}

void freeMatrix(Matrix* m)
{
    if (m != NULL && m->matrix != NULL)
    {
        for (int i = 0; i < m->row; i++)
        {
            free(m->matrix[i]);
        }
        free(m->matrix);

        m->col = 0;
        m->row = 0;
        m->matrix = NULL;
    }
}

/****************************
            I/O
****************************/

void inputMatrix(Matrix* input_m)
{
    for (int i = 0; i < input_m->row; i++)
    {
        for (int j = 0; j < input_m->col; j++)
        {
            scanf("%lf", *(input_m->matrix + i) + j);
        }
    }
}

Matrix inputNewMatrix(const int row, const int col)
{
    Matrix m = getMatrix(row, col);
    inputMatrix(&m);
    return m;
}

void printMatrix(const Matrix m)
{
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            printf("%lf ", m.matrix[i][j]);
        }
        printf("\n");
    }
}

void printMessageMatrix(const char* message, const Matrix m)
{
    printf("%s\n", message);
    printMatrix(m);
}

void fprintMatrix(const FILE* file, const Matrix m)
{
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            fprintf(file, "%d ", m.matrix[i][j]);
        }
        fprintf(file, "\n");
    }
}

void fprintMessageMatrix(const FILE* file, const char* message, const Matrix m)
{
    fprintf(file, "%s\n", message);
    fprintMatrix(file, m);
}

/****************************
            Utility
****************************/

Matrix nullMatrix()
{
    Matrix m;
    m.col = 0;
    m.row = 0;
    m.matrix = NULL;
    return m;
}

bool isNullMatrix(Matrix m)
{
    if (m.col == 0 &&
        m.row == 0 &&
        m.matrix == NULL)
    {
        return 1;
    }
    else
        return 0;
}

void moveMatrix(Matrix* m_target, Matrix* m_source)
{
    *m_target = *m_source;
    freeMatrix(m_source);
}

void swapMatrix(Matrix* m1, Matrix* m2)
{
    Matrix temp = *m1;
    *m1 = *m2;
    *m2 = temp;
}

Matrix deepCopyMatrix(Matrix m)
{
    Matrix m_copy = getMatrix(m.row, m.col);
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
            m_copy.matrix[i][j] = m.matrix[i][j];
    }
    return m_copy;
}

void randomMatrix(const Matrix m, const int n)
{
    srand(time(NULL));
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            m.matrix[i][j] = rand() % n + 1;
        }
    }
}

Matrix getRandomMatrix(const int row, const int col, const int n)
{
    Matrix m = getMatrix(row, col);
    randomMatrix(m, n);

    return m;
}

/****************************
          Operators
****************************/

Matrix matrixProduct(const Matrix m1, const Matrix m2)
{
    Matrix m;
    m.col = 0;
    m.row = 0;
    m.matrix = NULL;
    if (m1.col != m2.row)
        return nullMatrix();

    m = getMatrix(m1.row, m2.col);
    const int count = m1.col;
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            m.matrix[i][j] = 0;
            for (int idx = 0; idx < count; idx++)
            {
                m.matrix[i][j] += m1.matrix[i][idx] * m2.matrix[idx][j];
            }
        }
    }

    return m;
}

bool matrixEqual(Matrix m1, Matrix m2)
{
    if (m1.row != m2.row || m1.col != m2.col)
    {
        return 0;
    }
    else
    {
        for (int i = 0; i < m1.row; i++)
        {
            for (int j = 0; j < m1.col; j++)
            {
                if (m1.matrix[i][j] != m2.matrix[i][j])
                    return 0;
            }
        }
        return 1;
    }
}

Matrix subMatrix(
    Matrix m, 
    unsigned int row1,
    unsigned int row2,
    unsigned int col1,
    unsigned int col2
) 
{
    int row_len;
    int col_len;
    int tmp;
    Matrix result_m;

    if (row1 > row2)
    {   
        tmp = row1;
        row1 = row2;
        row2 = tmp;
    }
    if (col1 > col2)
    {   
        tmp = col1;
        col1 = col2;
        col2 = tmp;
    }

    if (m.row <= row2 || 
        m.col <= col2 ||
        row1 < 0 ||
        col1 < 0
    ) 
    {
        result_m = nullMatrix();
    }
    else
    {
        row_len = row2 - row1;
        col_len = col2 - col1;
        result_m = getMatrix(row_len, col_len);

        for (int row = row1; row < row2; row++)
        {
            for (int col = col1; col < col2; col++)
            {
                result_m.matrix[row - row1][col - col1] = m.matrix[row][col];
            }   
        }
    }
    
    return result_m;
}

int findLeading1(Matrix m, unsigned int row)
{
    if (m.row <= row)
    {
        return -1;
    }
    else
    {
        for (int col = 0; col < m.col; col++)
        {
            if (m.matrix[row][col] == 0)
            {
                continue;
            }
            else if (m.matrix[row][col] == 1)
            {
                return col;
            }
            else
            {
                return -1;
            }
        }
    }
}

bool isEchelonForm(Matrix m)
{
    for (int row = 0; row < m.row; row++)
    {
        if (findLeading1(m, row) == -1)
        {
            return false;
        }
    }

    return true;
    
}

int type1(Matrix m, unsigned int row, MatrixType num)
{
    if (m.row <= row)
    {
        return -1;
    }

    for (int col = 0; col < m.col; col++)
    {
        m.matrix[row][col] /= num;
    }
    
    return 1;
}

int type2(Matrix m, unsigned int row1, unsigned int row2)
{
    if (m.row <= row1 || m.row <= row2)
    {
        return -1;
    }
    else
    {
        MatrixType* tmp = m.matrix[row1];
        m.matrix[row1] = m.matrix[row2];
        m.matrix[row2] = tmp;
    }
    
    return 1;
}

int type3(Matrix m, unsigned int row, unsigned int target_row, MatrixType num)
{
    if (m.row <= row || m.row <= target_row)
    {
        return -1;
    }
    else
    {
        for (int col = 0; col < m.col; col++)
        {
            m.matrix[target_row][col] += num * m.matrix[row][col];
        }
    }

    return 1;
}

Matrix gaussElim(Matrix m)
{
    Matrix result_m = deepCopyMatrix(m);

    int leading_index;
    int leading_var;

    for (int row = 0; row < result_m.row; row++)
    {
        for (int col = 0; col < result_m.col; col++)
        {
            if (result_m.matrix[row][col] == 0)
            {
                continue;
            }
            else
            {
                leading_index = col;
                leading_var = result_m.matrix[row][col];
                break;
            }
        }

        type1(result_m, row, 1.0 / leading_var);
        for (int op_row = row + 1; op_row < result_m.row; op_row++)
        {
            type3(result_m, row, op_row, result_m.matrix[op_row][leading_index]);
        }    
    }
    
    return result_m;
}

/****************************
          Solutions
****************************/

typedef struct CostSize
{
    int row;
    int col;
} CostSize;


Matrix multipleProduct(const Matrix* m_array, const int array_size)
{
    // Find A Lowest Cost Sequence
    CostSize* memo = (int*)malloc(sizeof(CostSize) * array_size);
    int* seq = (int*)malloc(sizeof(int) * (array_size - 1));

    int lowest_cost = INT_MAX;
    int lowest_pos = 0;
    int array_size_i = array_size;
    int cost;

    for (int i = 0; i < array_size; i++)
    {
        memo[i].col = m_array[i].col;
        memo[i].row = m_array[i].row;
    }

    for (int i = 0; i < array_size - 1; i++)
    {
        // Get Each Cost and save it
        for (int j = 0; j < array_size_i - 1; j++)
        {
            cost = memo[j].col * memo[j + 1].row * memo[j].row * memo[j + 1].col;
            if (cost < lowest_cost)
            {
                cost = lowest_cost;
                lowest_pos = j;
            }
        }
        seq[i] = lowest_pos;

        // Resetting
        memo[lowest_pos].col = memo[lowest_pos + 1].col;
        for (int j = lowest_pos + 1; j < array_size_i - 1; j++)
        {
            memo[j] = memo[j + 1];
        }
        
        array_size_i = array_size_i - 1;
        lowest_cost = INT_MAX;
    }
    
    free(memo);

    // Multiply
    Matrix* copy_m_array = (Matrix*)malloc(sizeof(Matrix) * array_size);
    for (int i = 0; i < array_size; i++)
    {
        copy_m_array[i] = deepCopyMatrix(m_array[i]);
    }

    array_size_i = array_size;
    Matrix tmp_result;
    for (int i = 0; i < array_size - 1; i++)
    {
        tmp_result = matrixProduct(
            copy_m_array[seq[i]], copy_m_array[seq[i] + 1]
        );

        freeMatrix(&(copy_m_array[seq[i]]));
        freeMatrix(&(copy_m_array[seq[i] + 1]));

        copy_m_array[seq[i]] = tmp_result;
        for (int j = seq[i] + 1; j < array_size_i - 1; j++)
        {
            copy_m_array[j] = copy_m_array[j + 1];
        }

        array_size_i = array_size_i - 1;
    }
    
    return copy_m_array[0];
}

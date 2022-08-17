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

Matrix loop_multipleProduct(
    const Matrix* m_array,
    const int array_size,
    const Sequence sequence,
    const int start,
    const int end
)
{
    if (start == end)
    {
        return matrixProduct(m_array[start], m_array[end + 1]);
    }

    int i = 0;
    while (i < array_size - 1 && (start > sequence[i] || sequence[i] > end))
    {
        i++;
    }

    if (start == sequence[i])
    {
        Matrix temp;
        Matrix result;
        temp = loop_multipleProduct(
            m_array,
            array_size,
            sequence,
            start + 1,
            end
        );
        result = matrixProduct(
            m_array[start],
            temp
        );
        freeMatrix(&temp);
        return result;
    }
    else if (end == sequence[i])
    {
        Matrix temp;
        Matrix result;
        temp = loop_multipleProduct(
            m_array,
            array_size,
            sequence,
            start,
            end - 1
        );
        result = matrixProduct(
            temp,
            m_array[end + 1]
        );
        freeMatrix(&temp);
        return result;
    }
    else
    {
        Matrix temp1 = loop_multipleProduct(
            m_array,
            array_size,
            sequence,
            start,
            sequence[i] - 1
        );
        Matrix temp2 = loop_multipleProduct(
            m_array,
            array_size,
            sequence,
            sequence[i] + 1,
            end
        );
        Matrix result = matrixProduct(
            temp1,
            temp2
        );
        freeMatrix(&temp1);
        freeMatrix(&temp2);
        return result;
    }
}

bool isSafe(Sequence sequence, const int arry_size)
{
    if (sequence == NULL)
    {
        return 1;
    }
    else
    {
        bool* range = (bool*)malloc(sizeof(bool) * (arry_size - 1));
        for (int i = 0; i < arry_size - 1; i++)
        {
            range[i] = 0;
        }

        for (int i = 0; i < arry_size - 1; i++)
        {
            if (!range[sequence[i]])
                range[sequence[i]] = 1;
            else
                return 0;
        }

        for (int i = 0; i < arry_size - 1; i++)
        {
            if (!(range[i]))
                return 0;
        }

        free(range);
        return 1;
    }
}

Matrix multipleProduct(const Matrix* m_array, const int array_size, Sequence sequence)
{
    if (!isSafe(sequence, array_size))
    {
        return nullMatrix();
    }

    bool in_func_gen_seq = 0;
    if (sequence == NULL)
    {
        in_func_gen_seq = 1;
        sequence = (int*)malloc(sizeof(int) * (array_size - 1));
        for (int i = 0; i < array_size - 1; i++)
        {
            //sequence[i] = array_size - 2 - i;
            sequence[i] = i;
        }
    }

    Matrix result = loop_multipleProduct(
        m_array,
        array_size,
        sequence,
        0,
        array_size - 2
    );
    if (in_func_gen_seq)
    {
        free(sequence);
    }
    return result;
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

void printSequence(Sequence sequence, const int seq_size)
{
    for (int i = 0; i < seq_size; i++)
    {
        printf("%d ", sequence[i]);
    }
    printf("\n");
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

// Base Version
/////////////////////////////////////////////////////////////////////////////////////

int productCost(Matrix m1, Matrix m2)
{
    return m1.col * m1.row * m2.col;
}

Matrix loop_sumProductCost(
    const Matrix* m_array,
    const int array_size,
    const Sequence sequence,
    const int start,
    const int end,
    int* cost
)
{
    if (start == end)
    {
        *cost += productCost(m_array[start], m_array[end + 1]);
        Matrix result = matrixProduct(m_array[start], m_array[end + 1]);
        return result;
    }

    int i = 0;
    while (i < array_size - 1 && (start > sequence[i] || sequence[i] > end))
    {
        i++;
    }

    if (start == sequence[i])
    {
        Matrix temp;
        Matrix result;
        temp = loop_sumProductCost(
            m_array,
            array_size,
            sequence,
            start + 1,
            end,
            cost
        );
        result = matrixProduct(
            m_array[start],
            temp
        );
        *cost += productCost(m_array[start], temp);
        freeMatrix(&temp);
        return result;
    }
    else if (end == sequence[i])
    {
        Matrix temp;
        Matrix result;
        temp = loop_sumProductCost(
            m_array,
            array_size,
            sequence,
            start,
            end - 1,
            cost
        );
        result = matrixProduct(
            temp,
            m_array[end + 1]
        );
        *cost += productCost(temp, m_array[end + 1]);
        freeMatrix(&temp);
        return result;
    }
    else
    {
        Matrix temp1 = loop_sumProductCost(
            m_array,
            array_size,
            sequence,
            start,
            sequence[i] - 1,
            cost
        );
        Matrix temp2 = loop_sumProductCost(
            m_array,
            array_size,
            sequence,
            sequence[i] + 1,
            end,
            cost
        );
        Matrix result = matrixProduct(
            temp1,
            temp2
        );
        *cost += productCost(temp1, temp2);
        freeMatrix(&temp1);
        freeMatrix(&temp2);
        return result;
    }
}

int sumProductCost(Matrix* m_array, const int array_size, Sequence sequence, Matrix* result_p)
{
    if (!isSafe(sequence, array_size))
    {
        return 0;
    }

    int cost = 0;

    *result_p = loop_sumProductCost(
        m_array,
        array_size,
        sequence,
        0,
        array_size - 2,
        &cost
    );

    if (isNullMatrix(*result_p))
    {
        return 0;
    }
    else
        return cost;
}

Sequence generatePermutation(const int seq_size, int tries, int total_tries) 
{
    Sequence seq = (int*)malloc(sizeof(int) * seq_size);
    bool* num_list = (bool*)malloc(sizeof(bool) * seq_size);
    int end;
    int cnt;

    for (int i = 0; i < seq_size; i++)
    {
        num_list[i] = 1;
    }

    for (int i = 0; i < seq_size; i++)
    {
        total_tries /= seq_size - i;
        end = tries / total_tries + 1;
        tries %= total_tries;
        cnt = 0;
        for (int j = 0; j < seq_size; j++)
        {
            if (num_list[j])
                cnt++;
            if (cnt == end)
            {
                seq[i] = j;
                num_list[j] = 0;
                break;
            }
        }
    }

    free(num_list);
    return seq;
}

int totalTries(const int seq_size) 
{
    int total_tries = 1;
    for (int i = 0; i < seq_size; i++)
    {
        total_tries *= i + 1;
    }

    return total_tries;
}

Sequence findLowestCost_BaseVersion(
    const Matrix* m_array, 
    const int array_size, 
    Matrix* result_p, 
    int* cost
)
{
    const int seq_size = array_size - 1;
    int lowest_cost = INT_MAX;
    Sequence lowest_cost_seq = NULL;
    int temp_cost;
    Sequence seq = NULL;
    Matrix result;
    const int total_tries = totalTries(seq_size);

    for (int tries = 0; tries < total_tries; tries++)
    {
        seq = generatePermutation(seq_size, tries, total_tries);
        temp_cost = sumProductCost(m_array, array_size, seq, &result);
        if (temp_cost != 0 && temp_cost < lowest_cost)
        {
            freeMatrix(result_p);
            *result_p = result;
            if (lowest_cost_seq != NULL)
                free(lowest_cost_seq);
            lowest_cost_seq = seq;
            lowest_cost = temp_cost;
        }
        else
        {
            free(seq);
            freeMatrix(&result);
        }
    }

    *cost = lowest_cost;
    return lowest_cost_seq;
}

// Advenced Version
/////////////////////////////////////////////////////////////////////////////////////

void resverseSeq(Sequence seq, const int seq_size)
{
    int temp;
    for (int i = 0; i < seq_size / 2; i++)
    {
        temp = seq[i];
        seq[i] = seq[seq_size - 1 - i];
        seq[seq_size - 1 - i] = temp;
    }
}

void freeMatrixArray(Matrix* m_array, const int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        freeMatrix(&m_array[i]);
    }
    free(m_array);
}

void copyMatrixArray(Matrix* destination, Matrix* source, int source_length)
{
    for (int i = 0; i < source_length; i++)
    {
        destination[i] = deepCopyMatrix(source[i]);
    }
}

void arrangeMatrixArray(Matrix* m_array, int empty_start, int empty_end, int array_size)
{
    for (int i = 0; i < array_size - empty_end; i++)
    {
        m_array[empty_start + i] = m_array[empty_end + i];
    }
}

void shiftMatrixArray(Matrix* m_array, int empty_pos, int array_size)
{
    arrangeMatrixArray(m_array, empty_pos, empty_pos + 1, array_size);
}

void shiftIntArray(int* array, int empty_pos, int array_size)
{
    for (int i = 0; i < array_size - empty_pos - 1; i++)
    {
        array[empty_pos + i] = array[empty_pos + i + 1];
    }
}

/*
Matrix loop_sumProductCost_AdvencedVersion(
    Matrix* m_array,
    const int array_size,
    Sequence seq,
    int* cost_array,
    const int i,
    int* total_cost
) 
{
    if (array_size == 1)
    {
        return m_array[0];
    }
    else
    {
        int lowest_cost = cost_array[0];
        int lowest_cost_pos = 0;
        for (int j = 1; j < array_size - 1; j++)
        {
            if (lowest_cost > cost_array[j])
            {
                lowest_cost_pos = j;
                lowest_cost = cost_array[j];
            }
        }

        Matrix temp = m_array[lowest_cost_pos];
        m_array[lowest_cost_pos] = matrixProduct(temp, m_array[lowest_cost_pos + 1]);
        freeMatrix(&temp);
        freeMatrix(&m_array[lowest_cost_pos + 1]);
        *total_cost += lowest_cost;
        shiftMatrixArray(m_array, lowest_cost_pos + 1, array_size);
        int restore_num = 0;

        seq[i] = lowest_cost_pos;
        
        if (lowest_cost_pos == 0)
        {
            
        }
        else if (lowest_cost_pos == array_size - 2)
        {

        }
        else
        {

        }

        return loop_sumProductCost_AdvencedVersion(m_array, array_size - 1, seq, cost_array, i + 1, total_cost);
    }
}
*/

Sequence findLowestCost_AdvencedVersion(
    const Matrix* m_array, 
    const int array_size, 
    Matrix* result_p, 
    int* cost
)
{
    Sequence seq = (int*)malloc(sizeof(int) * (array_size - 1));
    int* cost_array = (int*)malloc(sizeof(int) * (array_size - 1));
    for (int i = 0; i < array_size - 1; i++)
    {
        seq[i] = -1;
    }

    Matrix* m_array_cp = (Matrix*)malloc(sizeof(int) * array_size);
    copyMatrixArray(m_array_cp, m_array, array_size);
    int* included_matrix = (int*)malloc(sizeof(int) * array_size);
    for (int i = 0; i < array_size; i++)
    {
        included_matrix[i] = 0;
    }

    for (int i = 0; i < array_size - 1; i++)
    {
        cost_array[i] = productCost(m_array_cp[i], m_array_cp[i + 1]);
    }

    int seq_size = array_size - 1;
    int smallest_cost;
    int smallest_cost_pos;
    int total_matrix_num;
    Matrix temp_m;
    for (int seq_idx = 0; seq_idx < array_size - 1; seq_idx++)
    {
        smallest_cost = INT_MAX;
        for (int i = 0; i < seq_size; i++)
        {
            if (cost_array[i] <= smallest_cost)
            {
                smallest_cost_pos = i;
                smallest_cost = cost_array[i];
            }
        }
        total_matrix_num = 0;

        for (int i = 0; i <= smallest_cost_pos; i++)
        {
            total_matrix_num += included_matrix[i];
        }
        seq[seq_idx] = total_matrix_num + smallest_cost_pos;

        printf("Smallest Pos: %d  Real Pos: %d  cost: %d\n", smallest_cost_pos, seq[seq_idx], smallest_cost);
        temp_m = matrixProduct(m_array_cp[smallest_cost_pos], m_array_cp[smallest_cost_pos + 1]);
        //freeMatrix(&m_array_cp[smallest_cost_pos]);
        //freeMatrix(&m_array_cp[smallest_cost_pos + 1]);
        m_array_cp[smallest_cost_pos] = temp_m;

        *cost += smallest_cost;
      
        included_matrix[smallest_cost_pos] += included_matrix[smallest_cost_pos + 1] + 1;
        shiftIntArray(cost_array, smallest_cost_pos, seq_size);
        shiftIntArray(included_matrix, smallest_cost_pos + 1, seq_size);
        shiftMatrixArray(m_array_cp, smallest_cost_pos + 1, seq_size);
        if(smallest_cost_pos != 0)
            cost_array[smallest_cost_pos - 1] = productCost(
                m_array_cp[smallest_cost_pos - 1], 
                m_array_cp[smallest_cost_pos]
            );
        if(smallest_cost_pos != seq_size)
            cost_array[smallest_cost_pos] = productCost(
                m_array_cp[smallest_cost_pos], 
                m_array_cp[smallest_cost_pos + 1]
            );
        
        seq_size -= 1;
    }

    printf("total cost: %d\n", *cost);

    free(cost_array);
    free(included_matrix);
    resverseSeq(seq, array_size - 1);
    return seq;
}
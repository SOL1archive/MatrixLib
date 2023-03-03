#include <malloc.h>
#include <stdio.h>
#include "Matrix.h"

void multipleMatrixProductTest()
{
    Matrix m = getRandomMatrix(3, 3, 4);
    Matrix* case1 = &m;

    printf(
        "%d\n",
        matrixEqual(
            *case1,
            multipleProduct(case1, 1)
        )
    );

    Matrix* case2 = (Matrix*)malloc(sizeof(Matrix) * 2);
    case2[0] = getRandomMatrix(3, 5, 3);
    case2[1] = getRandomMatrix(5, 3, 3);

    printf(
        "%d\n",
        matrixEqual(
            matrixProduct(case2[0], case2[1]),
            multipleProduct(case2, 2)
        )
    );

    Matrix* case3 = (Matrix*)malloc(sizeof(Matrix) * 5);
    case3[0] = getRandomMatrix(3, 5, 3);
    case3[1] = getRandomMatrix(5, 4, 3);
    case3[2] = getRandomMatrix(4, 6, 3);
    case3[3] = getRandomMatrix(6, 2, 3);
    case3[4] = getRandomMatrix(2, 7, 3);

    Matrix case3_ans = matrixProduct(
        case3[0],
        matrixProduct(
            case3[1],
            matrixProduct(
                case3[2],
                matrixProduct(
                    case3[3],
                    case3[4]
                )
            )
        )
    );

    printf(
        "%d\n",
        matrixEqual(
            case3_ans,
            multipleProduct(case3, 5)
        )
    );
}
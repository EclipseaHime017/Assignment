#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 结构体定义矩阵
typedef struct Matrix{
    int rows;
    int cols;
    float *data;
}Matrix;


// 创建一个空的矩阵
Matrix* createMatrix(int size) {
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix));
    mat->size = size;
    mat->data = (float*)malloc(size * size * sizeof(float));
    return mat;
}

// 释放矩阵内存
void freeMatrix(Matrix* mat) {
    free(mat->data);
    free(mat);
}

// 乘法运算的一个子块
void multiplyBlock(Matrix* A, Matrix* B, Matrix* C, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            for (int k = 0; k < size; k++) 
                C->data[i+j*size] += A->data[j*size+k] * B->data[k*size+i];
        }
    }
}

// 矩阵乘法主函数
Matrix* matrixMultiply(Matrix* A, Matrix* B,int block_size) {
    int size = A->size;
    Matrix* C = createMatrix(size);

    double time;
    clock_t start,end;
    start = clock();
    for (int i = 0; i < size; i += block_size) 
    {
        for (int j = 0; j < size; j += block_size) 
        {
            for (int k = 0; k < size; k += block_size) 
            {
                for (int ii = 0; ii < block_size; ii++) 
                {
                    for (int jj = 0; jj < block_size; jj++) 
                    {
                        for (int kk = 0; kk < block_size; kk++) 
                            multiplyBlock(A, B, C, block_size);
                    }
                }
            }
        }
    }
    end = clock();
    time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("It costs %lf second.\n",time);
    return C;
}

// 打印矩阵
// void printMatrix(Matrix* mat) {
//     for (int i = 0; i < mat->size; i++) {
//         for (int j = 0; j < mat->size; j++) {
//             printf("%d ", mat->data[i][j]);
//         }
//         printf("\n");
//     }
// }

int main() {
    int n = 8; // 2^n = 256
    int size = 1000;

    // 创建两个矩阵并初始化
    Matrix* A = createMatrix(size);
    Matrix* B = createMatrix(size);
    // 计算乘积矩阵
    Matrix* C = matrixMultiply(A, B, n);

    // 打印结果
    // printf("Matrix A:\n");
    // printMatrix(A);
    // printf("\nMatrix B:\n");
    // printMatrix(B);
    // printf("\nMatrix C (Result of A*B):\n");
    // printMatrix(C);

    // 释放内存
    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);

    return 0;
}
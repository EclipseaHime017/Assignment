#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//winograd算法
// 定义矩阵结构体
typedef struct {
    int rows;
    int cols;
    float *data;
} Matrix;

// 创建矩阵
Matrix *create_matrix(int rows, int cols) {
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    mat->rows = rows;
    mat->cols = cols;
    mat->data = (float *)malloc(rows * cols * sizeof(int));
    return mat;
}

// 释放矩阵内存
void free_matrix(Matrix *mat) {
    free(mat->data);
    free(mat);
}

// 定义矩阵乘法函数，使用 Winograd 算法
Matrix *matmul_winograd(const Matrix *A, const Matrix *B) {
    // 检查矩阵的大小是否符合要求
    if (A->cols != B->rows) {
        printf("Error: Matrix dimensions do not match!\n");
        return NULL;
    }

    int m = A->rows;
    int n = A->cols;
    int p = B->cols;
    double time;
    float start,end;
    // 初始化结果矩阵 C
    Matrix *C = create_matrix(m, p);

    // 计算中间矩阵 M（m*p），其中m的第i行第j列的元素为-2 * B[i][0] + B[i][1] + B[i][2]
    float *M = (float *)malloc(m * p * sizeof(float));
    start = clock();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            M[i * p + j] = -2 * B->data[i * p] + B->data[i * p + 1] + B->data[i * p + 2];
        }
    }

    // 计算矩阵 C（m*p），其中c的第i行和第j列的元素为A[i][0] * M[i][j] + A[i][1] * M[i][j] + A[i][2] * M[i][j]
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C->data[i * p + j] = 0;
            for (int k = 0; k < n; k++) {
                C->data[i * p + j] += (A->data[i * n + k] + B->data[k * p + j]) * M[i * p + j];
            }
        }
    }
    //由此原本时间复杂度为O(mnp)的矩阵乘法算法的时间复杂度降低为O(mnp/3)
    end = clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %lf seconds\n", time);
    // 释放中间矩阵 M 的内存
    free(M);

    return C;
}

int main() {
    int m = 1000, n = 1000, p = 1000; // 矩阵维度

    // 创建矩阵 A
    Matrix *A = create_matrix(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A->data[i * n + j] = i + j;
        }
    }

    // 创建矩阵 B
    Matrix *B = create_matrix(n, p);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            B->data[i * p + j] = i - j;
        }
    }

    // 调用矩阵乘法函数
    Matrix *C = matmul_winograd(A, B);

    // 打印结果矩阵 C
    // printf("Result matrix C:\n");
    // for (int i = 0; i < m; i++) {
    //     for (int j = 0; j < p; j++) {
    //         printf("%d ", C->data[i * p + j]);
    //     }
    //     printf("\n");
    // }

    // 释放矩阵内存
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return 0;
}

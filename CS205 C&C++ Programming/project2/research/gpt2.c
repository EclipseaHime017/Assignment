#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000

void matmul_row_order(double *A, double *B, double *C) {
    for (int i = 0; i < N; i++) { // row index
        for (int j = 0; j < N; j++) { // column index
            double sum = 0;
            for (int k = 0; k < N; k++) { // inner product
                sum += A[i*N + k] * B[k*N + j];
            }
            C[i*N + j] = sum;
        }
    }
}

void matmul_col_order(double *A, double *B, double *C) {
    for (int j = 0; j < N; j++) { // column index
        for (int i = 0; i < N; i++) { // row index
            double sum = 0;
            for (int k = 0; k < N; k++) { // inner product
                sum += A[i*N + k] * B[k*N + j];
            }
            C[i*N + j] = sum;
        }
    }
}

int main() {
    double *A = (double *)malloc(N * N * sizeof(double));
    double *B = (double *)malloc(N * N * sizeof(double));
    double *C = (double *)malloc(N * N * sizeof(double));

    // Initialize matrices A and B with random values
    srand(time(NULL));
    for (int i = 0; i < N * N; i++) {
        A[i] = (double)rand() / RAND_MAX;
        B[i] = (double)rand() / RAND_MAX;
    }

    clock_t start, end;
    double time_taken;

    // Test matrix multiplication with row order access
    start = clock();
    matmul_row_order(A, B, C);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication with row order access: %lf seconds\n", time_taken);

    // Test matrix multiplication with column order access
    start = clock();
    matmul_col_order(A, B, C);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication with column order access: %lf seconds\n", time_taken);

    free(A);
    free(B);
    free(C);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
//AVX2准确度测试器
// Matrix structure definition
typedef struct {
    int rows;
    int cols;
    float *data;
} Matrix;

// Function to create a new matrix and initialize its data
Matrix* create_matrix(int rows, int cols) {
    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL; // Memory allocation failed
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = (float*)malloc(rows * cols * sizeof(float));
    if (matrix->data == NULL) {
        free(matrix);
        return NULL; // Memory allocation failed
    }

    return matrix;
}

// Function to initialize a matrix with random values
void initialize_random(Matrix* matrix) {
    for (int i = 0; i < matrix->rows * matrix->cols; i++) {
        matrix->data[i] = (float)rand() / RAND_MAX; // Random value between 0 and 1
    }
}

// Function to print a matrix
void print_matrix(Matrix* matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%.2f ", matrix->data[i * matrix->cols + j]);
        }
        printf("\n");
    }
}

// Function to perform matrix multiplication using AVX2
Matrix* matmul_avx2(const Matrix* A, const Matrix* B) {
    if (A->cols != B->rows) {
        printf("Error: Number of columns of A must be equal to number of rows of B.\n");
        return NULL;
    }

    // Create result matrix
    Matrix* C = create_matrix(A->rows, B->cols);

    Matrix* transposed_B = create_matrix(B->cols, B->rows);
    for (int i = 0; i < B->rows; i++)
    {
        for (int j = 0; j < B->cols; j++)
            transposed_B->data[j * B->rows + i] = B->data[i * B->cols + j];
    }

    // Perform matrix multiplication using AVX2
    __m256 a,b,c;
    float c_result = 0.0f;
    float sum[8] = {0};//once initialize, later be covered
    for(int i = 0; i < C->cols; i++)
    {
        for(int j = 0; j < C->rows; j++)
        {
            c = _mm256_setzero_ps();//similar with sum array
            c_result = 0.0f;
            //setzero
            for(int k = 0; k < A->cols; k+=8)
            {
                a = _mm256_loadu_ps(A->data + k + j*A->cols);
                b = _mm256_loadu_ps(transposed_B->data + i*transposed_B->cols + k);//must permute!!!
                c = _mm256_mul_ps(a,b);                        
                _mm256_storeu_ps(sum,c);
                for (int l = 0; l<8;l++)
                    c_result += sum[l];
            }
            C->data[j * C->cols + i] = c_result;
            //k loop calculates an entry of c[i][j]
        }
        //map all rows in A to multiply a column in B
    }

    return C;
}

int main() {
    // Initialize a random 8x8 matrix
    Matrix* A = create_matrix(8, 8);
    initialize_random(A);
    printf("Matrix A:\n");
    print_matrix(A);

    // Create an identity matrix
    Matrix* I = create_matrix(8, 8);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            I->data[i * 8 + j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // Perform matrix multiplication A * I
    Matrix* result = matmul_avx2(A, I);
    if (result != NULL) {
        printf("\nResult of A * I:\n");
        print_matrix(result);
    }

    // Free memory
    free(A->data);
    free(A);
    free(I->data);
    free(I);
    if (result != NULL) {
        free(result->data);
        free(result);
    }

    return 0;
}
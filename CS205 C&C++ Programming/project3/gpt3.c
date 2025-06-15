#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
//对拍器
#ifdef WITH_AVX2
#include <immintrin.h>
#endif 

#ifdef _OPENMP
#include <omp.h>
#endif

// Your struct definition and function declarations
typedef struct Matrix{
    int rows;
    int cols;
    float *data;
}Matrix;

Matrix * create(int rows, int cols);

Matrix * matmul_improved(const Matrix *A, const Matrix *B)
{
    #ifdef WITH_AVX2
        clock_t start,end;
        double time;
        if (A->cols != B->rows) 
        {
            printf("Error: Number of columns of A must be equal to number of rows of B.\n");
            return NULL;
        }
        // Create result matrix
        Matrix* C = create(A->rows, B->cols);
        //transpose factor matrix B
        Matrix* transposed_B = create(B->cols, B->rows);
        start = clock();
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
                    b = _mm256_loadu_ps(transposed_B->data + i*transposed_B->cols + k);//must transpose!!!
                    c = _mm256_mul_ps(a,b);                        
                    _mm256_storeu_ps(sum,c);
                    for (int l = 0; l<8;l++)
                        c_result += sum[l];
                }
                C->data[j * C->cols + i] = c_result;//k loop calculates an entry of c[i][j]
            }//map all rows in A to multiply a column in B
        }
        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("It costs %lf second.\n",time);
        return C;
    #else
        printf("Error!AVX2 is not supported.\n");
        return NULL;
    #endif 
}

Matrix * create(int rows, int cols)
{
    if(cols>0&&cols<100000&&rows>0&&rows<100000)
    {
        Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
        matrix->cols = cols;
        matrix->rows = rows;
        matrix->data = (float*)malloc(matrix->rows * matrix->cols * sizeof(float));
        return matrix;
    }
    else
    {
        printf("The size of matrix is invalid.\n");
        printf("Please make sure that it is positive and cannot be too big.\n");
        printf("The numbers of columns and rows should be from 1 to 10000\n");
        return NULL;
    }
}

Matrix * matmul_divided(Matrix * A, Matrix * B)
{
    if (A->cols!=B->rows||A->cols%8!=0||A->rows%8!=0||B->cols%8!=0||B->rows%8!=0)
    {
        printf("You have to make the number of columns of A equal the number of rows of B\n");
        printf("The lateral length must be multiples of eight\n");
        return NULL;
    }

    Matrix * C = create(A->rows,B->cols);
    for (int i = 0;i<C->rows;i++)
    {
        for(int j = 0;j<C->cols;j++)
            C->data[i*C->cols + j] = 0;
    }//initialize C

    int i = A->rows/8;
    int k = A->cols/8;
    int j = B->cols/8;
    //A有i*k块，B有k*j块
    //分块计算C，C有i*j块，C的第i行第j块等于A的i行与B的j列相乘相加
    double time,start,end;
    start =  omp_get_wtime();
    #pragma omp parallel for
    // for (int cj = 0; cj < C->cols;cj+=8)//C的第j列分块
    // {
    //     for (int ci = 0; ci < C->rows;ci+=8)//C的第i行分块
    //     {
    //         for(int ak = 0; ak < A->cols;ak+=8)//A的第k列分块
    //         {
    //             for(int a = cj; a < cj+8; a++)//B分块第a列
    //             {
    //                 for(int b = ci; b < ci+8; b++)//B分块第b行
    //                 {
    //                     for(int c = ak; c < ak+8; c++)//A分块第c列
    //                         C->data[a*C->cols + b] += A->data[b*A->cols + c] * B->data[b*B->cols + a];
    //                 }   
    //             }//Aik和Bkj相乘相加，赋值给Cij块      
    //         }           
    //     }
    // }
    for (int ci = 0; ci < C->rows; ci += 8) {  // 对 C 的每个 8x8 子块的行进行迭代
        for (int cj = 0; cj < C->cols; cj += 8) {  // 对 C 的每个 8x8 子块的列进行迭代
            for (int ak = 0; ak < A->cols; ak += 8) {  // 遍历 A 的列和 B 的行的块
                for (int a = 0; a < 8; a++) {  // 遍历 C 子块中的每一行
                    for (int b = 0; b < 8; b++) {  // 遍历 C 子块中的每一列
                        float sum = 0;
                        for (int c = 0; c < 8; c++) {  // 点乘 A 的行和 B 的列
                            sum += A->data[(ci + a) * A->cols + ak + c] * B->data[(ak + c) * B->cols + cj + b];
                        }
                        C->data[(ci + a) * C->cols + cj + b] += sum;
                    }
                }
            }
        }
    }
    end =  omp_get_wtime();
    time = end - start;
    printf("It costs %lf second.\n",time);
    return C;
}

Matrix * matmul_part(Matrix *A, Matrix *B)
{
    if (A == NULL||B == NULL||A->cols != B->rows||A->cols%8!=0||A->rows%8!=0||B->cols%8!=0||B->rows%8!=0)
    {
        printf("You have to make the number of columns of A equal the number of rows of B\n");
        printf("The lateral length must be multiples of eight\n");
        printf("You have to make sure your calculation is valid\n");
        return NULL;
    }

    Matrix * C = create(A->rows,B->cols);
    for (int i = 0;i<C->rows;i++)
    {
        for(int j = 0;j<C->cols;j++)
            C->data[i*C->cols + j] = 0;
    }//initialize C
    #ifdef WITH_AVX2
        Matrix* transposed_B = create(B->cols, B->rows);
            for (int i = 0; i < B->rows; i++)
            {
                for (int j = 0; j < B->cols; j++)
                    transposed_B->data[j * B->rows + i] = B->data[i * B->cols + j];
            }
        //A has i*k blocks, while B has k*j blocks
        //calculate C in blocks, while consequently C has i*j blocks，The ij block equals dot product ith row of A and jth column of B
        double start,end,time;
        start =  omp_get_wtime();
        #pragma omp parallel for
        for (int ci = 0; ci < C->rows; ci += 8) //iterate with C rows
        {
            for (int cj = 0; cj < C->cols; cj += 8) //iterate with C columns
            { 
                for (int ak = 0; ak < A->cols ; ak += 8)   //iterate with A columns, doing dot product
                {
                    for (int a = 0; a < 8; a++)  //for each block, we apply similar algorithm as normal 3-loop matrix multiplication
                    { 
                        for (int b = 0; b < 8; b++)
                        { 
                            float sum = 0;
                            __m256 i,j,k;
                            float temp[8] = {0};
                            for (int c = 0; c < 8; c++)
                            {
                                i = _mm256_loadu_ps(A->data + (ci + a) * A->cols + ak + c);
                                j = _mm256_loadu_ps(transposed_B->data + ak + c + (cj + b)*transposed_B->rows);
                                k = _mm256_mul_ps(i,j);
                                _mm256_storeu_ps(temp,k);
                                for (int l = 0; l < 8; l++)
                                    sum += temp[l];
                                C->data[(ci + a) * C->cols + cj + b] += sum;
                            }
                        }
                    }
                }
            }
        }
        end =  omp_get_wtime();
        time = end - start;
        printf("It costs %lf second.\n",time);
        free(transposed_B->data);
        free(transposed_B);
        return C;//Be careful of low accuracy
    #else
        printf("Error!AVX2 is not supported.\n");
        return NULL;
    #endif
}

Matrix * matmul_plain(const Matrix *A, const Matrix *B)
{
    clock_t start,end;
    double time;
    if (A == NULL&&B == NULL&&A->cols != B->rows)
    {
        //check if A and B are valid
        printf("You have to make the number of columns of A equal the number of rows of B\n");
        return NULL;
    }
    else
    {
        Matrix *C = (Matrix *)malloc(sizeof(Matrix));
        //define a returned matrix C and allocate memory for it, in which there are two 4-byte integers, a 8-byte pointer
        C->rows = A->rows;
        C->cols = B->cols;
        C->data = (float *)malloc(C->rows * C->cols * sizeof(float));
        //start counting after preparation
        start = clock();
        for(int i = 0; i < C->cols; i++)
        {
            for(int j = 0; j < C->rows; j++)
            {
                C->data[j * C->cols + i] = 0;
                //initialize elements of C
                for(int k = 0; k < A->cols; k++)
                    C->data[j * C->cols + i] += (A->data[j * A->cols + k]) * (B->data[k * B->cols + i]);
            }
            //map all rows in C to multiply a column in B
        }
        // for(int i = 0; i < C->cols; i++)
        // {
        //     for(int k = 0; k < A->cols; k++)
        //     {
        //         //initialize elements of C
        //         for(int j = 0; j < C->rows; j++)
        //             C->data[j * C->cols + i] += (A->data[j * A->cols + k]) * (B->data[k * B->cols + i]);
        //     }
        //     //map all rows in C to multiply a column in B
        // }
        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("It costs %lf seconds.\n",time);
        return C;
    }
}

// Function to generate a random float between min and max
float random_float(float min, float max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}

// Function to generate a random matrix with specified rows and columns
Matrix* generate_random_matrix(int rows, int cols) {
    Matrix* matrix = create(rows, cols);
    if (matrix == NULL) {
        return NULL; // Error in matrix creation
    }

    // Fill matrix with random values
    for (int i = 0; i < rows * cols; i++) {
        matrix->data[i] = random_float(-10.0f, 10.0f); // Generate random float between -10 and 10
    }

    return matrix;
}

// Function to compare two matrices for equality
bool compare_matrices(Matrix* A, Matrix* B) {
    if (A->rows != B->rows || A->cols != B->cols) {
        printf("size!\n");
        return false; // Matrices have different dimensions
    }
    float epsilon =  A->cols * 1e-5f; 
    for (int i = 0; i < A->rows * A->cols; i++) {
        if (fabs(A->data[i] - B->data[i]) > epsilon) {
            printf("Entry! the %d\n",i);
            printf("A:%f\n",A->data[i]);
            printf("B:%f\n",B->data[i]);
            return false; // Matrices are not equal
        }
    }

    return true; // Matrices are equal
}

int main() {
    srand(time(NULL)); // Initialize random seed

    // Generate random matricesw
    int size = 1000;
    Matrix* matrix1 = generate_random_matrix(size,size);
    Matrix* matrix2 = generate_random_matrix(size,size);

    // free(matrix1->data);
    // free(matrix2->data);
    // float a[4] = {1,2,3,4};
    // float b[4] = {1,0,0,1};
    // matrix1->data = a;
    // matrix2->data = b;

    Matrix* your_result = matmul_divided(matrix1, matrix2);

    Matrix* reference_result = matmul_plain(matrix1, matrix2);

    // Compare results
    bool passed = compare_matrices(your_result, reference_result);
    if (passed) {
        printf("Test passed: Results match.\n");
    } else {
        printf("Test failed: Results do not match.\n");
    }

    
    // for (int i = 0; i < your_result->rows; i++) {
    //     for (int j = 0; j < your_result->cols; j++) 
    //         printf("%f ", your_result->data[i*your_result->rows+j]);
    //     printf("\n");
    // }
    // for (int i = 0; i <  reference_result->rows; i++){ 
    //     for (int j = 0; j <  reference_result->cols; j++) 
    //             printf("%f ",  reference_result->data[i* reference_result->rows+j]);
    //     printf("\n");
    // } 

    // Free memory
    free(matrix1->data);
    free(matrix1);
    free(matrix2->data);
    free(matrix2);
    free(your_result->data);
    free(your_result);
    free(reference_result->data);
    free(reference_result);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cblas.h>

#ifdef WITH_AVX2
#include <immintrin.h>
#endif 

#ifdef _OPENMP
#include <omp.h>
#endif

typedef struct Matrix{
    size_t rows;
    size_t  cols;
    float *data;
}Matrix;

Matrix * create(size_t cols,size_t rows);
Matrix * matmul_plain(Matrix *A, Matrix *B);
Matrix * matmul_improved(Matrix *A, Matrix *B);
Matrix * matmul_improved_OPMP(Matrix *A, Matrix *B);
Matrix * matmul_improved_divided(Matrix *A, Matrix *B);
Matrix * matmul_improved_part(Matrix *A, Matrix *B);
void matmul_openblas(Matrix *A, Matrix *B, Matrix *C);

int main()
{
    int length;
    int ret = scanf("%d",&length);
    size_t size = (size_t)length;

    Matrix * matrix1 = create(size,size);
    Matrix * matrix2 = create(size,size);
    
    Matrix * matrix_result1 = matmul_plain(matrix1,matrix2);
    // Matrix * matrix_result2 = matmul_improved(matrix1,matrix2);
    // Matrix * matrix_result2 = matmul_improved_OPMP(matrix1,matrix2);
    // Matrix * matrix_result2 = matmul_improved_divided(matrix1,matrix2);
 

    Matrix * matrix_result2 = create(size,size);
    matmul_openblas(matrix1,matrix2,matrix_result2);
    
    if (matrix_result1 != NULL&&matrix_result2 != NULL)
        printf("Matrices are successfully mutiplied.\n");
    else
        printf("Functions failed to create matrices or multiply.\n");

    Matrix *ptr[4] = {matrix1,matrix2,matrix_result1,matrix_result2};

    for(int i = 0;i < 4;i++)
    {
        if ((ptr[i]) != NULL)
        {
            if(ptr[i]->data != NULL)
                free(ptr[i]->data);
            free(ptr[i]);
            printf("Matrix info freed.\n");
        }
    }
    //make sure that memory freed
}

Matrix * create(size_t rows, size_t cols)
{
    if(cols>0&&cols<100000&&rows>0&&rows<100000)
    {
        Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
        matrix->cols = cols;
        matrix->rows = rows;
        matrix->data = (float*)malloc((matrix->rows) * (matrix->cols) * sizeof(float));
    }
    else
    {
        printf("The size of matrix is invalid.\n");
        printf("Please make sure that it is positive and cannot be too big.\n");
        printf("The numbers of columns and rows should be from 1 to 10000\n");
        return NULL;
    }
}

Matrix * matmul_plain(Matrix *A, Matrix *B)
{
    clock_t start,end;
    double time;
    if (A == NULL||B == NULL||A->cols != B->rows)
    {
        //check if A and B are valid
        printf("You have to make your calculation valid\n");
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
                    C->data[j * C->cols + i] += A->data[j * A->cols + k] * B->data[k * B->cols + i];
            }
            //map all rows in C to multiply a column in B
        }
        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("It costs %lf second.\n",time);
        return C;
    }
}

Matrix * matmul_improved(Matrix *A, Matrix *B)
{
    #ifdef WITH_AVX2
        clock_t start,end;
        double time;
        if (A == NULL||B == NULL||A->cols != B->rows)
        {
            printf("You have to make the number of columns of A equal the number of rows of B\n");
            return NULL;
        }
        else if (A->cols % 8 != 0 || B->rows % 8 != 0)
        {
            printf("The number of columns of A should be a multiple of 8\n");
            printf("The number of rows of B should also be a multiple of 8\n");
            printf("AVX2 provides register for 256 bits so please make sure your matrices fit requirements!\n");
            return NULL;
        }
        else
        {
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
            free(transposed_B->data);
            free(transposed_B);
            return C;
        } 
    #else
        printf("Error!AVX2 is not supported.\n");
        return NULL;
    #endif 
}

Matrix * matmul_improved_OPMP(Matrix *A, Matrix *B)
{
    #ifdef WITH_AVX2
        double start,end,time;//When multiple threads are used, the time is not accurate using clock()
        if (A == NULL||B == NULL||A->cols != B->rows)
        {
            printf("You have to make the number of columns of A equal the number of rows of B\n");
            printf("You have to make sure your calculation is valid\n");
            return NULL;
        }
        else if (A->cols % 8 != 0 || B->rows % 8 != 0)
        {
            printf("The number of columns of A should be a multiple of 8\n");
            printf("The number of rows of B should also be a multiple of 8\n");
            printf("AVX2 provides register for 256 bits so please make sure your matrices fit requirements!\n");
            return NULL;
        }
        else
        {
            Matrix* C = create(A->rows, B->cols);
            //transpose factor matrix B
            Matrix* transposed_B = create(B->cols, B->rows);
            for (int i = 0; i < B->rows; i++)
            {
                for (int j = 0; j < B->cols; j++)
                    transposed_B->data[j * B->rows + i] = B->data[i * B->cols + j];
            }
            __m256 a,b,c;
            float c_result = 0.0f;
            float sum[8] = {0};//once initialize, later be covered
            start = omp_get_wtime();
            #pragma omp parallel for
            for(int i = 0; i < C->cols; i++)
            {
                for(int j = 0; j < C->rows; j++)
                {
                    c = _mm256_setzero_ps();//similar with sum array
                    c_result = 0.0f;//setzero
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
            end = omp_get_wtime();
            time = end - start;
            printf("It costs %lf second.\n",time);
            free(transposed_B->data);
            free(transposed_B);
            return C;
        } 
    #else
        printf("Error!AVX2 is not supported.\n");
        return NULL;
    #endif 
}

Matrix * matmul_improved_divided(Matrix *A, Matrix *B)
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

    int i = A->rows/8;
    int k = A->cols/8;
    int j = B->cols/8;
    //A has i*k blocks, while B has k*j blocks
    //calculate C in blocks, while consequently C has i*j blocks，The ij block equals dot product ith row of A and jth column of B
    double start,end,time;
    start =  omp_get_wtime();
    #pragma omp parallel for
    for (int ci = 0; ci < C->rows; ci += 8) //iterate with C rows
    {
        for (int cj = 0; cj < C->cols; cj += 8) //iterate with C columns
        { 
            for (int ak = 0; ak < A->cols; ak += 8)   //iterate with A columns, doing dot product
            {
                for (int a = 0; a < 8; a++)  //for each block, we apply similar algorithm as normal 3-loop matrix multiplication
                { 
                    for (int b = 0; b < 8; b++)
                    { 
                        float sum = 0;
                        for (int c = 0; c < 8; c++)
                            sum += A->data[(ci + a) * A->cols + ak + c] * B->data[(ak + c) * B->cols + cj + b];
                        C->data[(ci + a) * C->cols + cj + b] += sum;
                    }
                }
            }
        }
    }

    end =  omp_get_wtime();
    time = end - start;
    printf("It costs %lf second.\n",time);
    return C;//Be careful of low accuracy
}

void matmul_openblas(Matrix *A, Matrix *B, Matrix *C)
{   
    if (A == NULL||B == NULL||C == NULL||A->cols != B->rows||C->rows!=A->rows||C->cols!=B->cols)
    {
        printf("You have regulate sizes of A, B, C\n");
        if(C != NULL)
        {
            free(C->data);
            C->data = NULL;    
        }
        return;
    }
    double time,start,end;
    start = omp_get_wtime();//openblas also uses multiple threads
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A->rows, B->cols, A->cols, 1.0, A->data, A->cols, B->data, B->cols, 0.0, C->data, C->cols);
    //The meaning of parameters refer to the following:
    //row priority, no transpose, no transpose, rows of result matrix, columns of result matrix, columns of A,
    //gain, data of A,columns of A, data of B, columns of B, extra_addition, address of C, columns of C
    end = omp_get_wtime();
    time = end - start;
    printf("It costs %lf second.\n",time);
    return;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef WITH_AVX2
#include <immintrin.h>
#endif 

#ifdef _OPENMP
#include <omp.h>
#endif

typedef struct Matrix{
    int rows;
    int cols;
    float *data;
}Matrix;


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
    if (A->cols != B->rows||A->cols%8!=0||A->rows%8!=0||B->cols%8!=0||B->rows%8!=0)
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
    }

    int i = A->rows/8;
    int k = A->cols/8;
    int j = B->cols/8;
    //A有i*k块，B有k*j块
    //分块计算C，C有i*j块，C的第i行第j块等于A的i行与B的j列相乘相加
    clock_t start,end;
    double time;
    start =  clock();
    // #pragma omp parallel for
    for (int cj = 0; cj < C->cols;cj+=8)//C的第j列分块
    {
        for (int ci = 0; ci < C->rows;ci+=8)//C的第i行分块
        {
            for(int ak = 0; ak < A->cols;ak+=8)//A的第k列分块
            {
                for(int a = 0; a < 8; a++)//B分块第a列
                {
                    for(int b = 0; b < 8; b++)//B分块第b行
                    {
                        for(int c = 0; c < 8; c++)//A分块第c列
                            C->data[(ci+a)*C->cols + cj + b] += A->data[ci*A->cols+ak + c] * B->data[ak*B->cols+cj + b];
                    }   
                }//Aik和Bkj相乘相加，赋值给Cij块      
            }           
        }
    }
    end =  clock();
    time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("It costs %lf second.\n",time);
    return C;
}

void printMatrix(Matrix* mat) {
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%f ", mat->data[j+i*mat->cols]);
        }
        printf("\n");
    }
}

Matrix * matmul_plain(Matrix *A, Matrix *B)
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
                C->data[i * C->cols + j] = 0;
                //initialize elements of C
                for(int k = 0; k < A->cols; k++)
                    C->data[i * C->cols + j] += A->data[j * A->cols + k] * B->data[k * B->cols + i];
            }
            //map all rows in C to multiply a column in B
        }
        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("It costs %lf second.\n",time);
        return C;
    }
}

int main()
{
    int size = 128;
    float ent[4] = {1,0,0,1};
    Matrix * matrix1 = create(size,size);
    for (int i = 0;i<size*size;i++)
    {
        matrix1->data[i] = 1.0f;
    }
    // matrix1->data = &ent[0];
    Matrix * matrix2 = create(size,size);
    for (int i = 0;i<size*size;i++)
    {
        matrix2->data[i] = 1.0f;
    }
    // matrix2->data = &ent[0];
    Matrix * matrix_result = matmul_divided(matrix1,matrix2);
    if (matrix_result != NULL)
    {
        printf("Matrices are successfully mutiplied.\n");
        printMatrix(matrix_result);
    }
    else
        printf("Functions failed to create matrices or multiply.\n");

    matrix_result = matmul_plain(matrix1,matrix2);
    if (matrix_result != NULL)
    {
        printf("Matrices are successfully mutiplied.\n");
        printMatrix(matrix_result);
    }
    else
        printf("Functions failed to create matrices or multiply.\n");

    Matrix *ptr[3] = {matrix1,matrix2,matrix_result};

    for(int i = 0;i < 3;i++)
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

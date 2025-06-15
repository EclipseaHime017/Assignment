#include <iostream>
#include <sys/time.h>  
#include <cblas.h>
#include <cublas_v2.h>
#include "cudamatrixclass.h"

#define TIME_START gettimeofday(&t_start, NULL);
#define TIME_END(name)    gettimeofday(&t_end, NULL); \
                    elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;   \
                    elapsedTime += (t_end.tv_usec - t_start.tv_usec) / 1000.0;  \
                    printf(#name " Time = %f ms.\n", elapsedTime);

using namespace std;

int main()
{
    struct timeval t_start, t_end;
    double elapsedTime = 0;

    int dev_count = 0;
    int dev_id = 0;
    cudaGetDeviceCount(&dev_count);
    cudaSetDevice(2);
    cudaGetDevice(&dev_id);
    printf("You have %d cuda devices.\n", dev_count);
    printf("You are using device %d.\n", dev_id);

    size_t rows  = 4096;
    size_t cols = 4096;
    Matrix matrix_add1 = Matrix(rows, cols);
    Matrix matrix_add2 = Matrix(rows, cols);

    matrix_add1.set(1);
    matrix_add2.set(2);
    float a = 5;
    float b = 2;
    Matrix matrix_addition_result1(rows, cols);
    matrix_addition_result1.set(0);
    Matrix matrix_addition_result2(rows, cols);
    matrix_addition_result2.set(0);
    try
    {
        matrix_addition_result1.scalar_addcpu(matrix_add1,a,b);
        matrix_addition_result1.print();
        matrix_addition_result2.scalar_addgpu(matrix_add2,a,b);
        matrix_addition_result2.print();
    }
    catch(const char * msg)
    {
        cerr << msg << endl;
    }
    //test multiplication
    matrix_addition_result1.set(1);
    matrix_addition_result2.set(1);
    matrix_add1.set(1);
    matrix_add2.set(1);
    try
    {
        TIME_START
        Matrix matrix_mul_result1 = matrix_add1.multiply_openblas(matrix_addition_result1);
        TIME_END(OpenBLAS)
        matrix_mul_result1.print();

        TIME_START
        Matrix matrix_mul_result2 = matrix_add2.multiply_cublas(matrix_addition_result2);
        TIME_END(CUBLAS)
        matrix_mul_result2.print();
    }
    catch(const char * msg)
    {
        cerr << msg << endl;
    }
    return 0;
}
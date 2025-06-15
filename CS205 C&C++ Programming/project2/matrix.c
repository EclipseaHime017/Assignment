#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>


typedef struct _Matrix{
    int row;
    int col;
    float * pdata;
}Matrix;

void geneM(Matrix *ptr)
{
    srand(time(NULL)); 
    for(int i = 0;i < (ptr->row);i++)
    {
        for(int j = 0;j < (ptr->col);j++)
        {
            float rand_float = ((float)rand() / RAND_MAX);
            ptr->pdata[j + i*(ptr->col)] = rand_float;
        }
    }
}

void mult(const Matrix *ptrA, const Matrix *ptrB, Matrix *ptrC)
{
    clock_t start,end;
    double time;
    if ((*ptrA).col != (*ptrB).row || ptrA == NULL || ptrB == NULL || ptrC == NULL)
        printf("Matrices are invalid! Be careful about that A column equals B row.");
    else{ 
        start = clock();
        for(int i = 0; i < (*ptrB).col; i++)//varying B column
        {
            for(int j = 0; j < (*ptrA).row; j++)//varying A row
            {
                for (int k = 0; k < (*ptrA).col; k++)//varying A column and B row
                {
                    (*ptrC).pdata[i+j*(*ptrC).col] = 0;
                    //C[j][i]
                    (*ptrC).pdata[i+j*(*ptrB).col] += (*ptrA).pdata[(*ptrA).col*j+k] * (*ptrB).pdata[(*ptrB).col*k+i];
                }
            }
        }
        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("It costs %lf second.\n",time);
    }
}

int main()
{
    int row1, row2, col1, col2;
    printf("Please input a number as the row of your first matrix\n");
    scanf("%d",&row1);
    printf("Please input a number as the column of your first matrix\n");
    scanf("%d",&col1);
    printf("Please input a number as the row of your second matrix\n");
    scanf("%d",&row2);
    printf("Please input a number as the row of your second matrix\n");
    scanf("%d",&col2);

    int size[6] = {row1,row2,col1,col2,row1,col2};

    if (col1 != row2 || row1 <= 0 || row2 <= 0 || col1 <= 0 || col2 <= 0)
    {
        printf("Invalid matrix. Please check your columns and rows.\n"); 
        return 1;
    }
    else
    {
        Matrix matrix[3];
        Matrix *ptr1 = &matrix[0];
        Matrix *ptr2 = &matrix[1];
        Matrix *ptr3 = &matrix[2];
        Matrix *ptr[3] = {ptr1,ptr2,ptr3};

        for(int i = 0;i < 3;i++)
        {
            (*ptr[i]).row = size[2*i];
            (*ptr[i]).col = size[2*i+1];
            (*ptr[i]).pdata = (float*)malloc((*ptr[i]).row * (*ptr[i]).col*sizeof(float));
            if ((*ptr[i]).pdata == NULL)
                printf("Wrong allocation; Fault Gate 1.");
        }

        geneM(ptr1);
        geneM(ptr2);
        mult(ptr1,ptr2,ptr3);

        for(int i = 0;i < 3;i++)
        {
            if ((*ptr[i]).pdata != NULL)
            {
                free((*ptr[i]).pdata);
                printf("Matrix info freed.\n");
                (*ptr[i]).pdata = NULL;
            }
        }
        //make sure that memory freed
    }
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include"mult.h"


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

        // for (int i = 0; i < (*ptr3).row; i++) 
        // {
        //     for (int j = 0; j < (*ptr3).col; j++) 
        //         printf("%f ", (*ptr3).pdata[(*ptr3).row*i+j]);
        //     printf("\n");
        // }

        for(int i = 0;i < 3;i++)
        {
            if ((*ptr[i]).pdata != NULL)
            {
                free((*ptr[i]).pdata);
                printf("Matrix info freed.\n");
                (*ptr[i]).pdata = NULL;
            }
        }
    }
    return 0;
}
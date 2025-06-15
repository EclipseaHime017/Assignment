#include <stdio.h>
#include "mult.h"
#include<time.h>

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
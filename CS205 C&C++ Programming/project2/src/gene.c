#include <stdio.h>
#include <time.h>
#include <float.h>
#include "mult.h"
#include <stdlib.h>


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
//row prior

// void geneM(Matrix *ptr)
// {
//     srand(time(NULL)); 
//     for(int j = 0; j < (ptr->col); j++)
//     {
//         for(int i = 0; i < (ptr->row); i++)
//         {
//             float rand_float = ((float)rand() / RAND_MAX);
//             ptr->pdata[j + i*(ptr->col)] = rand_float;
//         }
//     }
// }
//column prior

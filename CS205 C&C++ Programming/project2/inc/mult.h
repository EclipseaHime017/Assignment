#ifndef _MULT_H
#define _MULT_H

typedef struct _Matrix{
    int row;
    int col;
    float * pdata;
}Matrix;

void geneM(Matrix *ptr);

void mult(const Matrix *ptrA, const Matrix *ptrB, Matrix *ptrC);

#endif
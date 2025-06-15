#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

//set an empty 2d-array and fill it with random numbers

#define ROWS 10 // set rows of the matrix
#define COLS 10 // set columns of the matrix

void geneM(float matrix[ROWS][COLS]) {
    int i = 0;
    int j = 0;
    srand(time(NULL)); 
    // set random seed

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            float rand_float = ((float)rand() / RAND_MAX);
            matrix[i][j] = rand_float;
        }
    }
}

void printM(float matrix[ROWS][COLS]) {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            printf("%f ", matrix[i][j]);
            //default precision six numbers 
        }
        printf("\n");
    }
    //a compound loop give a matrix
}

int main() {
    float matrix[ROWS][COLS];

    geneM(matrix);

    printM(matrix);

    return 0;
}
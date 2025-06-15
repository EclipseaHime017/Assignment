#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    clock_t start,end1,end2,end3;
    double time1,time2,time3;
    int b = 0;
    int c = 0;
    int d = 0;
    start = clock();
    for (int i=0;i<100000;i++)
        b = b + 10 + 10 + 10 + 10 + 10 + 10 + 10 + 10 + 10 + 10;
    end1 = clock();
    for (int i = 0;i<100000;i++)
    {
        for (int j = 0;j<10;j++)
            c += 10;
    }
    end2 = clock();
    for (int i = 0;i<10;i++)
    {
        for (int j = 0;j<100000;j++)
            d += 10;
    }
    end3 = clock();
    time1 = ((double) (end1 - start)) / CLOCKS_PER_SEC;
    time2 = ((double) (end2 - end1)) / CLOCKS_PER_SEC;
    time3 = ((double) (end3 - end2)) / CLOCKS_PER_SEC;
    printf("It costs %lf second.\n",time1);
    printf("It costs %lf second.\n",time2);
    printf("It costs %lf second.\n",time3);
}

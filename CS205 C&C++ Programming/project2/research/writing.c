#include <stdio.h>
#include <time.h>

int main()
{
    double time1, time2;
    clock_t start, end1, end2;
    start = clock();
    int a = 0;
    int *p = &a;
    for (int i = 0;i<1000000;i++)
        *p = i;
    end1 = clock();
    for (int i = 0;i<10;i++)
    {
        for (int j = 0; j < 100000; j++)
            *p = j;
    }
    end2 = clock();
    time1 = ((double) (end1 - start)) / CLOCKS_PER_SEC;
    time2 = ((double) (end2 - end1)) / CLOCKS_PER_SEC;
    printf("It costs %lf second.\n",time1);
    printf("It costs %lf second.\n",time2);
}
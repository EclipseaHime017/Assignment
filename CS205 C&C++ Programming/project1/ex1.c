#include <stdio.h>

int main()
{
    int num1, num2;
    int ret = scanf("%d %d", &num1, &num2);
    printf("The numbers are %d, %d.\n",num1,num2);
    printf("%d\n",ret);
}

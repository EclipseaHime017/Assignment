#include <stdio.h>
#include <stdbool.h>
#include  <string.h>

#define max 200
//set the maximum number

void add(char num1[max],char num2[max],char result[max+1])
{
    int carry = 0;

    for (int i = max-2; i >= 0; i--) 
    {
        int digit1 = (num1[i]);
        int digit2 = (num2[i]);

        int sum = digit1 + digit2 + carry;
        if (sum == 0)
        ;
        else if(sum >= 97 && sum <= 106)
        {
            result[i+1] = sum - 48;
            carry = (sum - 96) / 10;
        }
        else
        {
            result[i+1] = sum - 58;
            carry = (sum - 96) / 10;
        }
    }
    if(carry != 0)
        result[0] = carry + '0';
    else
        for (int i = 1; i <= 198; i++)
            result[i-1] = result[i];
    result[max + 1] = '\0';
}
//define a function for addition

int main(int argc, char **argv)
{
    int alen = strlen(argv[1]);
    int oplen = strlen(argv[2]);
    int blen = strlen(argv[3]);

    if(alen >= 200 || alen <= 0 || blen>= 200 || blen <= 0 || oplen != 1)
    {
        printf("Invalid Input\n");
        return 1;
    }
    //determine if the input is valid

    char a[max] = "0";
    strcpy(a,argv[1]);
    char op[2] = "0";
    strcpy(op,argv[2]);
    char b[max] = "0";
    strcpy(b,argv[3]);
    //initialize with arguments
    char result[max+1] = "0";
    char *a = argv[1];
    char *b = argv[3];
    char *op = argv[2];

    if(strcmp(op, "+") == 0)
    {
        add(a,b,result);
    }
    printf("%s %s %s = %s\n",a,op,b,result);
}
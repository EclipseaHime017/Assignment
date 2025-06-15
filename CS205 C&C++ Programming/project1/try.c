#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool isvalid(char *num)
{
    int neg = 0;
    int sci = 0;
    int dec = 0;
    int boolvalue = 1;
    for (int i = 0;i <= strlen(num)-1;i++)
    { 
        if(num[i] < '0' || num[i] > '9')
        {
            if (num[i] == '+'||num[i] == '-')
            {
                if(i!=0)
                {
                    boolvalue = 0;
                }              
            }
            else if ((num[i] == 'e'||num[i]=='E')&&i != 0)
            {
                if(sci == 0)
                {
                    sci++;
                    if (num[i+1] != '+'&&num[i+1] != '-'&&(num[i+1]< '0' || num[i+1] > '9'))
                    {
                        boolvalue = 0;
                    }
                    i++;
                }
                else
                {
                    boolvalue = 0;
                }
            }
            else if (num[i] == '.')
            {
                if(dec == 0)
                {
                    dec++;
                }
                else
                {
                    boolvalue = 0;
                }
            }
            else
            {
                boolvalue = 0;
            }
        
        }
    }
    if (boolvalue == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int main(int argc,char **argv)
{
    char *a = argv[1];
    char *b = argv[3];
    char *op = argv[2];
    char result[100] = "0";

    if (isvalid(a)==1&&isvalid(b)==1)
    {
        if(strlen(op) != 1)
        {
            printf("Invalid Input\n");
        }
        else
        {
            char *temp;
            double num1 = strtod(a,&temp);
            double num2 = strtod(b,&temp);
            if (strcmp(op,"+") == 0){
                double result = num1 + num2;
                printf("%s %s %s = %f\n",a,op,b,result);
            } 
            else if (strcmp(op,"-") == 0){
                double result = num1 - num2;
                printf("%s %s %s = %f\n",a,op,b,result);
            } 
            else if (strcmp(op,"*") == 0){
            double result = num1 * num2;
            printf("%s %s %s = %f\n",a,op,b,result);
            } 
            else if (strcmp(op,"/") == 0){
                if(strcmp(b,"0") == 0)
                {
                    printf("A number cannot be divied by zero.\n");
                }
                else{
                    double result = num1 / num2;
                    printf("%s %s %s = %f\n",a,op,b,result);
                }  
            }    
            else
            {
                printf("Invalid Input\n");
            } 
        }
    }
    else
    {
        printf("The input cannot be interpret as numbers!\n ");
    }
}

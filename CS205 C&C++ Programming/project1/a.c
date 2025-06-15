#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>

int Istype(char *num)
{
    int negative = 0;
    int expo = 0;
    int decimal = 0;
    int type = 1;  
    for (int i = 0;i <= strlen(num)-1;i++)
    {
        if(num[i] < '0' || num[i] > '9')
        {
            if (num[i] == '+'||num[i] == '-')
            {
                if(i!=0) type = 0; 
            }
            else if ((num[i] == 'e'||num[i]=='E')&&i != 0)
            {
                if(expo == 0)
                {
                    expo++;
                    if (num[i+1] != '+'&&num[i+1] != '-'&&(num[i+1]< '0' || num[i+1] > '9')) type = 0;
                    i++;
                    type = 2;
                }
                else type = 0;
            }
            else if (num[i] == '.')
            {
                if(decimal == 0) decimal++;
                else type = 0;
            }
            else type = 0;
        }
    }
    return type;
}
//判断对象类型函数

void decompose(const char *input, char **beforeE, char **afterE) 
{
    const char *posi = strchr(input, 'e');
    if (posi == NULL) 
    {
        posi = strchr(input, 'E');
    }
    size_t eIndex = posi - input;

    *beforeE = malloc(eIndex + 1);
    strncpy(*beforeE, input, eIndex);
    (*beforeE)[eIndex] = '\0';

    *afterE = strdup(posi + 1);
}
//指数拆解函数

char* combine(double num1, double num2) 
{
    char *result = malloc(100);

    sprintf(result, "%g", num1);
    strcat(result, "e");
    char temp[10];
    sprintf(temp, "%g", num2);
    strcat(result, temp);

    return result;
}
//指数合成函数

double pow(int num){
    double sol = 1;
    if (num > 0){
        for(int i = 0;i < num;i++) sol *= 1e1;
    }
    else if (num < 0){
        for(int i = 0;i < num;i++) sol /= 1e1;
    }
}
//指数函数



int main(int argc,char **argv)
{
    if(argc == 1)
    {
        
    }
    else if(argc == 4)
    {
        char *a = argv[1];
        char *b = argv[3];
        char *op = argv[2];
        double result = 0;

        if (Istype(a)==1 && Istype(b)==1)
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
                if (strcmp(op,"+") == 0)
                {
                    result = num1 + num2;
                } 
                else if (strcmp(op,"-") == 0)
                {
                    result = num1 - num2;
                } 
                else if (strcmp(op,"*") == 0)
                {
                    result = num1 * num2;
                } 
                else if (strcmp(op,"/") == 0)
                {
                    if(strcmp(b,"0") == 0)
                    {
                        printf("A number cannot be divied by zero.\n");
                        return 1;
                    }
                    else{
                        result = num1 / num2;
                    }
                }  
                else{
                    printf("Invalid Input\n");
                    return 1;
                }
                printf("%s %s %s = %g\n",a,op,b,result);
            }
        } 
        else if(Istype(a)==2||Istype(b)==2)
        {
            char *temp;
            char *result;
            double bodyresult = 0; 
            double powerresult = 0;
            char *power;
            char *body;
            decompose(a,&body,&power);
            double body_a = strtod(body,&temp);
            double power_a = strtod(power,&temp);
            decompose(b,&body,&power);
            double body_b = strtod(body,&temp);
            double power_b = strtod(power,&temp);
            
            if (strcmp(op,"+") == 0) 
            {
                bodyresult = body_a + body_b*pow(power_b-power_a);
                powerresult = power_a;
                result = combine(bodyresult,powerresult);
            }
            else if (strcmp(op,"-") == 0)
            {
                bodyresult = body_a - body_b*pow(power_b-power_a);
                powerresult = power_a;
                result = combine(bodyresult,powerresult);
            } 
            else if (strcmp(op,"*") == 0)
            {
                bodyresult = body_a * body_b;
                powerresult = power_a + power_b;
                result = combine(bodyresult,powerresult);
            } 
            printf("%s %s %s = %s\n",a,op,b,result);
            free(body);
            free(power);
            free(result);
        }
        else
        {
            printf("fault gate 1\n");
            printf("The input cannot be interpret as numbers!\n ");
        }
    }        
    else
    {
        printf("Invalid Input\n");
    }
}
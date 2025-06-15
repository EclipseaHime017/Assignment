#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>

int Istype(char *num)
{
    int negative = 0;//negative marks
    int expo = 0;//exponetial denotation
    int decimal = 0;//decimal dot
    int type = 1; //default valid number 
    for (int i = 0;i <= strlen(num)-1;i++)
    {
        if(num[i] < '0' || num[i] > '9')
        {
            if (num[i] == '+'||num[i] == '-')
            {
                if(i!=0) 
                    type = 0; 
            }
            else if ((num[i] == 'e'||num[i]=='E')&&i != 0)
            {
                if(expo == 0)
                {
                    expo++;
                    type = 2;
                    if (num[i+1] != '+'&&num[i+1] != '-'&&(num[i+1]< '0' || num[i+1] > '9')) 
                        type = 0;
                    i++;
                    decimal = 0;
                    //if "e" appear, decimal dot clear
                }
                else 
                    type = 0;
            }
            else if (num[i] == '.')
            {
                if(decimal == 0) 
                {
                    decimal++;
                    type = 2;
                }
                else type = 0;
            }
            else  
                type = 0;
        }
    }
    return type;
}
//Determine type of a number

void Excal(char *num1, char *num2, char *operator)
{
    mpf_t a,b,result;
    mpf_init(a);
    mpf_init(b);
    mpf_init(result);
    mpf_set_str(a,num1,10);
    mpf_set_str(b,num2,10);
    int validation = 1;
    if(strlen(operator) != 1)
        {
            printf("Invalid Input. Please check your operator.\n");
        }
    else
    {
        if (strcmp(operator,"+") == 0)
        {
            mpf_add(result, a, b);
        } 
        else if (strcmp(operator,"-") == 0)
        {
            mpf_sub(result, a, b);
        } 
        else if (strcmp(operator,"*") == 0)
        {
            mpf_mul(result, a, b);
        } 
        else if (strcmp(operator,"/") == 0)
        {
            if (mpf_cmp_ui(b, 0) == 0) 
            {
                printf("A number cannot be divied by zero.\n");
                validation = 0;
            }
            else
                mpf_div(result, a, b); 
        } 
        else
        {
            printf("Invalid Input\n");
            validation = 0;
        }

        //kill zeros(disorder)
        int precision = mpf_get_default_prec();
        char *numberstring;
        gmp_asprintf(&numberstring, "%.*Ff", precision, result);

        if (numberstring != NULL&&validation != 0) 
        {
            int len = strlen(numberstring);
            char *dot = strchr(numberstring, '.');
    
            if (dot != NULL) {
                for (int i = len - 1; i >= 0; i--) 
                {
                    if (numberstring[i] == '0' || numberstring[i] == '.') 
                        numberstring[i] = '\0';
                    else
                    break;
                }
            }
            printf("%s %s %s = ", num1, operator, num2);
            printf("%s\n", numberstring);
            free(numberstring);
        }           
    }
    mpf_clear(a);
    mpf_clear(b);
    mpf_clear(result);
}
//Versatile Calculator

void SciExcal(char *num1, char *num2, char *operator)
{
    mpf_t a,b,result;
    mpf_init(a);
    mpf_init(b);
    mpf_init(result);
    mpf_set_str(a,num1,10);
    mpf_set_str(b,num2,10);
    mpf_set_default_prec(400);

    if(strlen(operator) != 1)
        {
            printf("Invalid Input. Please check your operator.\n");
        }
    else
    {
        if (strcmp(operator,"+") == 0)
        {
            mpf_add(result, a, b);
        } 
        else if (strcmp(operator,"-") == 0)
        {
            mpf_sub(result, a, b);
        } 
        else if (strcmp(operator,"*") == 0)
        {
            mpf_mul(result, a, b);
        } 
        else if (strcmp(operator,"/") == 0)
        {
            if (mpf_cmp_ui(b, 0) == 0) 
                printf("A number cannot be divied by zero.\n");
            else
                mpf_div(result, a, b); 
        } 
        else
            printf("Invalid Input\n");

        mpf_t exp;
        mpf_init_set_d(exp,0);
        while(mpf_cmp_d(result, 10) >= 0 || mpf_cmp_d(result, -10) <= 0)
        {
            mpf_div_ui(result,result,10);
            mpf_add_ui(exp,exp,1);
        }
        if (mpf_cmp_d(result, 10) >= 0)
        {
            mpf_div_ui(result,result,10);
            mpf_add_ui(exp,exp,1);
        }
        printf("%s %s %s = ",num1,operator,num2);
        gmp_printf("%.Ffe%.Ff\n",result,exp);
        mpf_clear(exp);
    }
    mpf_clear(a);
    mpf_clear(b);
    mpf_clear(result);
    
}
//scientific notation

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        char a[100];
        char op[2];
        char b[100];

        while(1)
        {
            scanf("%s",a);
            if(strcmp(a,"quit") == 0)
                break;
            else
            {
                scanf("%s",op);
                scanf("%s",b);
                if(Istype(a) != 0 &&Istype(b) != 0)
                {   
                    if(Istype(a) == 2 && Istype(b) == 2)
                        SciExcal(a,b,op);
                    else
                        Excal(a,b,op);
                }
                else
                    printf("The input cannot be interpret as numbers!\n ");
            }
        }
    }
    else if(argc == 4)
    {
        char *num1 = argv[1];
        char *num2 = argv[3];
        char *op = argv[2];

        if(Istype(num1) != 0 &&Istype(num2) != 0)
        {   
            if(Istype(num1) == 2 && Istype(num2) == 2)
                SciExcal(num1,num2,op);
            else
                Excal(num1,num2,op);
        } 
        else
        {
            printf("The input cannot be interpret as numbers!\n ");
        }
    }        
    else
        printf("Invalid Input. Please type in only two numbers and an operator like 1 + 1\n");
}
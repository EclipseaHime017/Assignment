#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

char* zerokiller(mpf_t num) {
    char *zeroless = mpf_get_str(NULL, NULL, 10, 0, num);

    if (zeroless == NULL) {
        fprintf(stderr, "Error: mpf_get_str returned NULL.\n");
        return NULL;
    }

    size_t length = strlen(zeroless);

    while (length > 1 && zeroless[length - 1] == '0') {
        if (zeroless[length - 2] != '.') {
            zeroless[--length] = '\0';
        }
        length -= 1;
    }

    char *result = strdup(zeroless);

    // 释放原有字符串内存
    free(zeroless);

    return result;
}

void Excal(char *num1, char *num2, char *operator) {
    mpf_t a, b, result;
    mpf_init(a);
    mpf_init(b);
    mpf_init(result);
    mpf_set_str(a, num1, 10);
    mpf_set_str(b, num2, 10);

    if (strlen(operator) != 1) {
        printf("Invalid Input: Operator must be a single character.\n");
    } else {
        if (strcmp(operator, "+") == 0) {
            mpf_add(result, a, b);
        } else if (strcmp(operator, "-") == 0) {
            mpf_sub(result, a, b);
        } else if (strcmp(operator, "*") == 0) {
            mpf_mul(result, a, b);
        } else if (strcmp(operator, "/") == 0) {
            if (mpf_cmp_ui(b, 0) == 0) {
                printf("Invalid Input: Division by zero.\n");
            } else {
                mpf_div(result, a, b);
            }
        } else {
            printf("Invalid Input: Unknown operator.\n");
        }

        // 检查是否成功获取了字符串
        char *formattedResult = zerokiller(result);
        if (formattedResult != NULL) {
            printf("%s %s %s = %s\n", num1, operator, num2, formattedResult);
            // 释放结果字符串内存
            free(formattedResult);
        }
    }

    // 释放资源
    mpf_clear(a);
    mpf_clear(b);
    mpf_clear(result);
}

int main() {
    // 测试函数
    Excal("123.456", "789.012", "+");
    Excal("123.456", "789.012", "-");
    Excal("123.456", "789.012", "*");
    Excal("123.456", "0", "/");  // 测试除法，分母为零
    Excal("123.456", "789.012", "/");

    return 0;
}
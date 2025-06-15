#include <iostream>

using namespace std;

//是一个操作符
int main(){
    char num[3] = {1,2};
    cout << sizeof(int) << endl;//返回数据类型字节
    cout << sizeof(num) << endl;//无需加上尺寸，返回元素数量
}

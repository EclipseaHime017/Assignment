#include<iostream>

using namespace std;

int main()
{
    int * zero = new int[0];//int(0)的话，会分配四个字节并赋值为0
    if (zero == NULL)
        cout << "Memory allocation failed" << endl;
    else
        cout << "Memory allocation successful" << endl;
        cout << zero << endl;
    delete zero;
}

//Even if the size of memory is 0, it still can be allocated successfully.
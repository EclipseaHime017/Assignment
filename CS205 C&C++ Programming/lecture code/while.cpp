//测试while的属性
#include <iostream>

using namespace std;

int main()
{
    cout << "Please input a character ";
    char a = 0;
    cin >> a;
    while(a != 'q')
    {
        switch(a)
        {
            case 'a':
                cout << "a" << endl;
                break;
            case 'A':
                cout << "A" << endl;
                break;
            default:
                cout << "Undefined" << endl;
                break;
        }
        cin >> a;
    }
}
#include <iostream>

using namespace std;

class Hello
{
    static int value;
    int num;

    public:
    int sum(int a, int b)
    {
        return a + b;
    }
    void setvalue(int v)
    {
        value = v;
    }
    int getvalue()
    {
        return value;
    }
};

int main()
{
    Hello h1,h2;
    h1.setvalue(10);
    cout << h2.getvalue() << endl;
    return 0;
}
#include <iostream>
#include <memory>

using namespace std;

union data
{
    int pi;
    float pf;
    double pd;
};

int main()
{
    data d;
    d.pd = (double)10;
    cout << d.pi << endl;
    double c = 10.0f;
    cout << c << endl;
}

#include <iostream>
#include <memory>
#include <cstring>
#include "class.hpp"

using namespace std;

int main()
{
    Matrix m1(2, 2);
    Matrix m2(3, 3);
    Matrix m3(3, 3);
    int arr1[] = {2,2,2,2};
    double arr2[] = {1,2,3,4,5,6,7,8,9}; 
    float arr3[] = {1,0,0,0,1,0,0,0,1};
    m1.genedata(m1, arr1,sizeof(arr1)/sizeof(int));
    m2.genedata(m2, arr2,sizeof(arr2)/sizeof(double));
    m3.genedata(m3, arr3,sizeof(arr3)/sizeof(float));

    Matrix m4 = m2 * m3;
    Matrix m5 = m4.createROI(0, 0, 2, 2);
    //ROI test, shared memory
    cout << "m4 = "<< endl;
    cout << m4 << endl;
    cout << "m5 = "<< endl;
    cout << m5<< endl;
    m4 += m3;
    cout << "m4 = "<< endl;
    cout << m4 << endl;
    cout << "m5 = "<< endl;
    cout << m5<< endl;

    m5 += m1;
    cout << "m5 = "<< endl;
    cout << m5<< endl;

    Matrix m6;
    m6 = m3;//copy assignment test
    cout << "m6 = "<< endl;
    cout << m6<< endl;

    unsigned char arr4[9] = {'a','b','c','d','e','f','g','h'};
    Matrix m7(3, 3);
    m7.genedata(m7, arr4,sizeof(arr4)/sizeof(unsigned char));
    Matrix m8 = m7 * m6;
    cout << "m7 = "<< endl;
    cout << m7<< endl;
    cout << "m8 = "<< endl;
    cout << m8<< endl;//char matrix multiplication test

    Matrix m9(3, 3);
    short arr5[] = {1,2,3,4,5,6,7,8,9};
    m9.genedata(m9, arr5,sizeof(arr5)/sizeof(short));
    Matrix m10 = m9 - m4;
    cout << "m10 = "<< endl;
    cout << m10<< endl;//short matrix subtraction test
}
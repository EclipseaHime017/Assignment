#pragma once
#include <iostream>
#include <memory>
#include <cstring>

using namespace std;

union entry
{
    unsigned char uc;
    short sh;
    int in;
    float fl;
    double db;
};
//we use union as entries so that we make sure that data is aligned

typedef enum Datatype{
    UNSIGNED_CHAR,SHORT,INT,FLOAT,DOUBLE
}Datatype;
//using enumration to indicate the type

class Matrix
{
    private:
        size_t rows;
        size_t cols;
        size_t origin_cols;
        shared_ptr<entry[]> data;
        //最初的想法时如果智能指针可以是一个样本类的（这里矩阵不能有特定的样本），那么就可以实现直接转化
        //结果是不能识别data，所以默认使用double类型，后面来改。   
    public:
        Datatype datatype;
        Matrix(): rows(0), cols(0),origin_cols(0),data(nullptr),datatype(DOUBLE){}
        Matrix(size_t i): rows(1), cols(i), origin_cols(i),data(new entry[i]),datatype(DOUBLE){}
        Matrix(size_t r, size_t c): rows(r), cols(c), origin_cols(c),data(new entry[r*c]),datatype(DOUBLE){}
        Matrix(const Matrix &m):rows(m.rows),cols(m.cols), origin_cols(m.origin_cols),data(m.data),datatype(m.datatype){}//copy constructor
        ~Matrix(){}
        Matrix& operator=(const Matrix &m) 
        {
            if (this != &m) {
                this->rows = m.rows;
                this->cols = m.cols;
                this->origin_cols = m.origin_cols;
                this->datatype = m.datatype;
                this->data = m.data;
            }
            return *this;
        }
        template <typename T>
        void genedata(Matrix &m, T*arr,size_t n);//assign entries for matrices
        template <typename V>
        V getelement(const Matrix &m, size_t r, size_t c);//get the element of the matrix
        Matrix operator+(const Matrix &m);
        Matrix operator+=(const Matrix &m);
        Matrix operator-(const Matrix &m);
        Matrix operator*(const Matrix &m);
        Matrix createROI(size_t r, size_t c, size_t h, size_t w);
        friend ostream & operator<<(ostream &os, const Matrix &m);
};

template <typename U>
Datatype Whatisit(U*arr,size_t n);//determine the type of the data
Datatype WhoamI(const type_info &t);

template <typename T>
void Matrix::genedata(Matrix &m, T*arr,size_t n)
{
    if (n == 0)  
    {
        cerr << "No data is in." << endl;
        return;
    }
    if (m.rows == 0 || m.cols == 0 || m.rows*m.cols < n)
    {
        cerr << "Size of your matrix is too small." << endl;
        return;
    }
    m.datatype = Whatisit(arr,n);
    switch(m.datatype)
    {
        case DOUBLE:
            for(size_t a = 0;a<m.rows*m.cols;a++)
                m.data.get()[a].db = arr[a];
            break;
        case FLOAT:
            for(size_t a = 0;a<m.rows*m.cols;a++)
                m.data.get()[a].fl = arr[a];
            break;
        case INT:
            for(size_t a = 0;a<m.rows*m.cols;a++)
                m.data.get()[a].in = arr[a];
            break;
        case SHORT:
            for(size_t a = 0;a<m.rows*m.cols;a++)
                m.data.get()[a].sh = arr[a];
            break;
        case UNSIGNED_CHAR:
            for(size_t a = 0;a<m.rows*m.cols;a++)
                m.data.get()[a].uc = arr[a];
            break;
        default:
            throw invalid_argument("No appropriate datatype.");
    }
}

template <typename V>
V Matrix::getelement(const Matrix &m, size_t r, size_t c)
{
    V element;
    if (r >= m.rows || c >= m.cols)
    {
        throw invalid_argument("Out of range.");
    }
    size_t loc = r*m.origin_cols+c;
    switch(m.datatype)
    {
        case DOUBLE:
            element =  m.data.get()[loc].db;
            break;
        case FLOAT:
            element =  m.data.get()[loc].fl;
            break;
        case INT:
            element =  m.data.get()[loc].in;
            break;
        case SHORT:
            element =  m.data.get()[loc].sh;
            break;
        case UNSIGNED_CHAR:
            element =  m.data.get()[loc].uc;
            break;
        default:
            throw invalid_argument("No appropriate datatype.");
    }
    return element;
}

template <typename U>
Datatype Whatisit(U* arr,size_t n)
{
    Datatype temp = UNSIGNED_CHAR;
    for (size_t a = 0; a < n-1; a++)
    {
        const type_info& t1 = typeid(arr[a]);
        const type_info& t2 = typeid(arr[a + 1]);
        Datatype d1 = WhoamI(t1);
        Datatype d2 = WhoamI(t2);
        if (d1 > d2)
            temp = d1;
        else
            temp = d2;
    }
    return temp;
}
#pragma once
#include <iostream>
#include <memory>
#include <cstring>

using namespace std;

template <typename T>
class Matrix
{
    private:
        size_t rows;
        size_t cols;
        shared_ptr<T> data;
    public:
        Matrix() : rows(0), cols(0), data(new T[rows * cols]){}
        Matrix(size_t i) : rows(1), cols(i), data(new T[rows * cols]){}
        Matrix(size_t r, size_t c) : rows(r), cols(c), data(new T[rows * cols]){}
        Matrix(const Matrix & m) : rows(m.rows), cols(m.cols), data(m.data){}//copy constructor
        ~Matrix() {}
        Matrix & operator=(const Matrix & m)//copy assignment
        {
            return *this;
        }
        void genedata(Matrix &m, T *arr)//data generation
        {
            if(m.cols*m.rows < sizeof(arr)/sizeof(T))
            {
                cerr << "The entries of matrices are limited." << endl;
                return;
            }
            if(m.cols == 0||m.rows == 0)
            {
                cerr << "The size of matrix is zero." << endl;
                return;
            }
            memcpy(m.data.get(),arr,m.rows*m.cols*sizeof(T));
            return;
        }

        bool operator==(const Matrix & m) const //comparison
        {
            if(rows != m.rows || cols != m.cols)
                return false;
            for(size_t i = 0; i < rows; i++)
            {
                size_t loc = i*cols;
                for(size_t j = 0; j < cols; j++)
                {
                    if(data.get()[loc+j] != m.data.get()[loc+j])
                        return false;
                }
            }
            return true;
        }
        bool operator!=(const Matrix & m) const //inverse comparison
        {
            return !(*this == m);
        }
        Matrix operator+(const Matrix & m) const//addition
        {
            if (rows != m.rows || cols != m.cols)
            {
                cerr << "Matrix dimensions do not match" << endl;
                return *this;
            }
            if (this->rows == 0 || this->cols == 0)
            {
                cerr << "Matrix has no size" << endl;
                return *this;
            }
            Matrix result(rows, cols);
            for(size_t i = 0; i < rows; i++)
            {
                size_t loc = i*cols;
                for(size_t j = 0; j < cols; j++)
                    result.data.get()[loc+j] = data.get()[loc+j] + m.data.get()[loc+j];
            }
            return result;
        }
        Matrix operator-(const Matrix & m) const
        {
            if (rows != m.rows || cols != m.cols)
            {
                cerr << "Matrix dimensions do not match" << endl;
                return *this;
            }
            if (this->rows == 0 || this->cols == 0)
            {
                cerr << "Matrix has no size" << endl;
                return *this;
            }
            Matrix result(rows, cols);
            for(size_t i = 0; i < rows; i++)
            {
                size_t loc = i*cols;
                for(size_t j = 0; j < cols; j++)
                    result.data.get()[loc+j] = data.get()[loc+j] - m.data.get()[loc+j];
            }
            return result;//return an object
        }
        Matrix operator*(const Matrix & m) const
        {
            if (cols != m.rows)
            {
                cout << "Matrix dimensions do not match" << endl;
                return *this;
            }
            if (this->rows == 0 || this->cols == 0 || m.rows == 0 || m.cols == 0)
            {
                cout << "Matrix has no size" << endl;
                return *this;
            }
            Matrix result(rows, m.cols);
            for(size_t i = 0; i < rows; i++)
            {
                for(size_t j = 0; j < m.cols; j++)
                {
                    result.data.get()[i*m.cols+j] = 0;
                    for(size_t k = 0; k < cols; k++)
                        result.data.get()[i*m.cols+j] += data.get()[i*cols+k] * m.data.get()[k*m.cols+j];
                }
            }
            return result;
        }
        friend ostream & operator<<(ostream & os, const Matrix & m)
        {
            for(size_t i = 0; i < m.rows; i++)
            {
                size_t loc = i*m.cols;
                for(size_t j = 0; j < m.cols; j++)
                    os << m.data.get()[loc+j] << " ";
                os << endl;
            }
            return os;
        }
};
#include "class.hpp"

Matrix Matrix::createROI(size_t r, size_t c, size_t h, size_t w)//row location,column location,height,width
{
    Matrix temp(h,w);
    if (r+h > this->rows || c+w > this->cols)
    {
        cerr << "Out of range." << endl;
        return temp;
    }
    temp.origin_cols = this->origin_cols;
    temp.datatype = this->datatype;
    size_t offset = r * this->origin_cols + c;
    temp.data = shared_ptr<entry[]>(this->data.get() + offset, [](entry*){});
    return temp;
}

Datatype WhoamI(const type_info& t)
{
    if (t == typeid(double))
        return DOUBLE;
    else if (t == typeid(float))
        return FLOAT;
    else if (t == typeid(int))
        return INT;
    else if (t == typeid(short))
        return SHORT;
    else if (t == typeid(unsigned char))
        return UNSIGNED_CHAR;
    else
        throw invalid_argument("Unknown type");
}


Matrix Matrix::operator+(const Matrix &m)
{
    if(this->rows != m.rows || this->cols != m.cols)
    {
        cerr << "Different size." << endl;
        return *this;
    }
    if(this->rows == 0 || this->cols == 0)
    {
        cerr << "Empty matrix." << endl;
        return *this;
    }
    Matrix temp(m.rows,m.cols);
    if (this->datatype > m.datatype)
        temp.datatype = this->datatype;
    else
        temp.datatype = m.datatype;
    for(size_t a = 0; a < m.rows; a++)
    {
        for(size_t b = 0; b < m.cols; b++)
        {
            switch(this->datatype)
            {
                case DOUBLE:
                    this->data.get()[a*this->origin_cols+b].db += getelement<double>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case FLOAT:
                    this->data.get()[a*this->origin_cols+b].fl += getelement<float>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case INT:
                    this->data.get()[a*this->origin_cols+b].in += getelement<int>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case SHORT:
                    this->data.get()[a*this->origin_cols+b].sh += getelement<short>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case UNSIGNED_CHAR:
                    this->data.get()[a*this->origin_cols+b].uc += getelement<unsigned char>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                default:
                    throw invalid_argument("Unable to add.");
            }
        }
    }
    return temp;  
}

Matrix Matrix::operator+=(const Matrix &m)
{
    if(this->rows != m.rows || this->cols != m.cols)
    {
        cerr << "Different size." << endl;
        return *this;
    }
    if(this->rows == 0 || this->cols == 0)
    {
        cerr << "Empty matrix." << endl;
        return *this;
    }
    if (this->datatype > m.datatype)
        this->datatype = this->datatype;
    else
        this->datatype = m.datatype;
    for(size_t a = 0; a < m.rows; a++)
    {
        for(size_t b = 0; b < m.cols; b++)
        {
            switch(this->datatype)
            {
                case DOUBLE:
                    this->data.get()[a*this->origin_cols+b].db += getelement<double>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case FLOAT:
                    this->data.get()[a*this->origin_cols+b].fl += getelement<float>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case INT:
                    this->data.get()[a*this->origin_cols+b].in += getelement<int>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case SHORT:
                    this->data.get()[a*this->origin_cols+b].sh += getelement<short>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                case UNSIGNED_CHAR:
                    this->data.get()[a*this->origin_cols+b].uc += getelement<unsigned char>(m, (a*m.rows+b) / m.cols, (a*m.rows+b) % m.cols);
                    break;
                default:
                    throw invalid_argument("Unable to add.");
            }
        }
    }
    return *this;  

}

Matrix Matrix::operator-(const Matrix &m)
{
    if(this->rows != m.rows || this->cols != m.cols)
    {
        cerr << "Different size." << endl;
        return *this;
    }
    if(this->rows == 0 || this->cols == 0)
    {
        cerr << "Empty matrix." << endl;
        return *this;
    }
    Matrix temp(m.rows,m.cols);
    if (this->datatype > m.datatype)
        temp.datatype = this->datatype;
    else
        temp.datatype = m.datatype;
    for(size_t a = 0; a < m.rows * m.cols; a++)
    {
        switch(temp.datatype)
        {
            case DOUBLE:
                temp.data.get()[a].db = getelement<double>(*this, a / m.cols, a % m.cols) - getelement<double>(m, a / m.cols, a % m.cols);
                break;
            case FLOAT:
                temp.data.get()[a].fl = getelement<float>(*this, a / m.cols, a % m.cols) - getelement<float>(m, a / m.cols, a % m.cols);
                break;
            case INT:
                temp.data.get()[a].in = getelement<int>(*this, a / m.cols, a % m.cols) - getelement<int>(m, a / m.cols, a % m.cols);
                break;
            case SHORT:
                temp.data.get()[a].sh = getelement<short>(*this, a / m.cols, a % m.cols) - getelement<short>(m, a / m.cols, a % m.cols);
                break;
            case UNSIGNED_CHAR:
                temp.data.get()[a].uc = getelement<unsigned char>(*this, a / m.cols, a % m.cols) - getelement<unsigned char>(m, a / m.cols, a % m.cols);
                break;
            default:
                throw invalid_argument("Unable to subtract.");
        }
    }
    return temp;  
}

Matrix Matrix::operator*(const Matrix &m)
{
    if(this->cols != m.rows)
    {
        cerr << "Different size." << endl;
        return *this;
    }
    if(this->rows == 0 || this->cols == 0 || m.rows == 0 || m.cols == 0)
    {
        cerr << "Empty matrix." << endl;
        return *this;
    }
    Matrix temp(this->rows, m.cols);
    if (this->datatype > m.datatype)
        temp.datatype = this->datatype;
    else
        temp.datatype = m.datatype;
    for(size_t a = 0; a < this->rows; a++)
    {
        for(size_t b = 0; b < m.cols; b++)
        {
            size_t loc = a*temp.origin_cols+b;
            switch(temp.datatype)
            {
                case DOUBLE:
                    temp.data.get()[loc].db = 0;
                    for(size_t c = 0; c < this->cols; c++)
                        temp.data.get()[loc].db += getelement<double>(*this, a, c) * getelement<double>(m, c, b);
                    break;
                case FLOAT:
                    temp.data.get()[loc].fl = 0;
                    for(size_t c = 0; c < this->cols; c++)
                        temp.data.get()[loc].fl += getelement<float>(*this, a, c) * getelement<float>(m, c, b);
                    break;
                case INT:
                    temp.data.get()[loc].in = 0;
                    for(size_t c = 0; c < this->cols; c++)
                        temp.data.get()[loc].in += getelement<int>(*this, a, c) * getelement<int>(m, c, b);
                    break;
                case SHORT:
                    temp.data.get()[loc].sh = 0;
                    for(size_t c = 0; c < this->cols; c++)
                        temp.data.get()[loc].sh += getelement<short>(*this, a, c) * getelement<short>(m, c, b);
                    break;
                case UNSIGNED_CHAR:
                    temp.data.get()[loc].uc = 0;
                    for(size_t c = 0; c < this->cols; c++)
                        temp.data.get()[loc].sh += getelement<unsigned char>(*this, a, c) * getelement<unsigned char>(m, c, b);
                    break;
                default:
                    throw invalid_argument("Unable to multiply.");
            }
        }
    }
    return temp;
}

ostream & operator<<(ostream &os, const Matrix &m)
{
    if (m.rows == 0 || m.cols == 0)
    {
        cerr << "Empty matrix." << endl;
        return os;
    }
    for(size_t a = 0;a<m.rows;a++)
    {
        for(size_t b = 0;b<m.cols;b++)
        {
            switch(m.datatype)
            {
                case DOUBLE:
                    os << m.data.get()[a*m.origin_cols+b].db << " ";
                    break;
                case FLOAT:
                    os << m.data.get()[a*m.origin_cols+b].fl << " ";
                    break;
                case INT:
                    os << m.data.get()[a*m.origin_cols+b].in << " ";
                    break;
                case SHORT:
                    os << m.data.get()[a*m.origin_cols+b].sh << " ";
                    break;
                case UNSIGNED_CHAR:
                    os << m.data.get()[a*m.origin_cols+b].uc << " ";
                    break;
                default:
                    throw invalid_argument("No data.");
            }
        }
        os << endl;
    }
    return os;
}
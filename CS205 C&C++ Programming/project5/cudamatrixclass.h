#ifndef _CUDA_MATRIX_H_
#define _CUDA_MATRIX_H_

class Matrix
{
    private:
        size_t rows;
        size_t cols;
        float * data;
    public:
        float * gpu_data;
        size_t len = rows*cols;
        Matrix(): rows(0), cols(0), data(NULL), gpu_data(NULL),len(0) {}
        Matrix(size_t r, size_t c):rows(r), cols(c),len(r * c),data(new float[r*c])
        {
            cudaMalloc(&gpu_data, rows * cols * sizeof(float));
        }
        Matrix(const Matrix &m):rows(m.rows), cols(m.cols),len(m.cols *m.rows),data(new float[m.rows*m.cols])
        {
            cudaMalloc(&gpu_data, rows * cols * sizeof(float));
            for(size_t i = 0; i < rows*cols; i++)
                data[i] = m.data[i];//hard copy
            cudaMemcpy(gpu_data, data, rows * cols * sizeof(float), cudaMemcpyHostToDevice);
        }
        ~Matrix() 
        {   
            if(data != NULL) 
                delete[] data;
            if(gpu_data != NULL)
                cudaFree(gpu_data);
        }
        Matrix & operator=(const Matrix &m)
        {
            if(this == &m)
                return *this;
            if(data != NULL)
                delete[] data;
            if(gpu_data != NULL)
                cudaFree(gpu_data);
            rows = m.rows;
            cols = m.cols;
            data = new float[rows*cols];
            cudaMalloc(&gpu_data, rows * cols * sizeof(float));
            for(size_t i = 0; i < rows*cols; i++)
                data[i] = m.data[i];
            cudaMemcpy(gpu_data, data, rows * cols * sizeof(float), cudaMemcpyHostToDevice);
            return *this;
        }
        void set(float val)
        {
            for(size_t i = 0; i < rows*cols; i++)
                data[i] = val;
            cudaMemcpy(gpu_data, data, rows * cols * sizeof(float), cudaMemcpyHostToDevice);
        }
        bool scalar_addcpu(const Matrix &fmat, float factor, float add);
        bool scalar_addgpu(const Matrix &fmat, float factor, float add);
        Matrix & multiply_openblas(const Matrix &fmat);
        Matrix & multiply_cublas(const Matrix &fmat);
        void print();
};

bool Matrix::scalar_addcpu(const Matrix &fmat, float factor, float add)
{
    if (fmat.rows != this->rows || fmat.cols != this->cols)
    {
        throw "Size mismatch";
    } 
    if (fmat.data == NULL||this->data == NULL)
    {
        throw "Empty matrix";
    }
    for(size_t i = 0; i < rows*cols; i++)
        this->data[i] = fmat.data[i] * factor + add;
    cudaMemcpy(this->gpu_data, this->data, len * sizeof(float), cudaMemcpyHostToDevice);

    return true;
}

__global__ void addKernel(const float * input, float factor, float add, float * output, size_t len)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
    if(i < len)
	    output[i] = factor*input[i] + add;
}

bool Matrix::scalar_addgpu(const Matrix &fmat, float factor, float add)
{
    if (fmat.rows != this->rows || fmat.cols != this->cols)
        throw "Size mismatch";
    if (fmat.data == NULL || this->data == NULL)
        throw "Empty matrix";

    float* d_input = nullptr;
    float* d_output = nullptr;

    cudaError_t cudaStatus;
    cudaStatus = cudaMalloc(&d_input, len * sizeof(float));
    if (cudaStatus != cudaSuccess) 
        throw "cudaMalloc failed for d_input!";

    cudaStatus = cudaMalloc(&d_output, len * sizeof(float));
    if (cudaStatus != cudaSuccess) 
    {
        cudaFree(d_input);
        throw "cudaMalloc failed for d_output!";
    }

    cudaStatus = cudaMemcpy(d_input, fmat.gpu_data, len * sizeof(float), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) 
    {
        cudaFree(d_input);
        cudaFree(d_output);
        throw "cudaMemcpy failed for d_input!";
    }

    addKernel<<<(len + 255) / 256, 256>>>(d_input, factor, add, d_output, len);

    cudaStatus = cudaMemcpy(this->gpu_data, d_output, len * sizeof(float), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) 
    {
        cudaFree(d_input);
        cudaFree(d_output);
        throw "cudaMemcpy failed for d_output!";
    }
    
    cudaFree(d_input);
    cudaFree(d_output);
    cudaMemcpy(this->data, this->gpu_data, len * sizeof(float), cudaMemcpyDeviceToHost);
    return true;
}

Matrix & Matrix::multiply_openblas(const Matrix &fmat)
{
    if (fmat.rows != this->cols)
    {
        throw "Size mismatch";
    } 
    if (fmat.data == NULL||this->data == NULL)
    {
        throw "Empty matrix";
    }
    Matrix * rmat = new Matrix(this->rows, fmat.cols);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, this->rows, fmat.cols, this->cols, 1.0, this->data, this->cols, fmat.data, fmat.cols, 0.0, rmat->data, rmat->cols);
    cudaMemcpy(rmat->gpu_data, rmat->data, len * sizeof(float), cudaMemcpyHostToDevice);
    return *rmat;
}

Matrix & Matrix::multiply_cublas(const Matrix &fmat)
{
    if (fmat.rows != this->cols)
    {
        throw "Size mismatch";
    } 
    if (fmat.data == NULL||this->data == NULL)
    {
        throw "Empty matrix";
    }
    Matrix * rmat = new Matrix(this->rows, fmat.cols);
    cublasHandle_t handle;
    cublasCreate(&handle);
    float alpha = 1.0;
    float beta = 0.0;
    cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, this->rows, fmat.cols, this->cols, &alpha, this->gpu_data, this->cols, fmat.gpu_data, fmat.cols, &beta, rmat->gpu_data, rmat->cols);
    cublasDestroy(handle);
    cudaMemcpy(rmat->data, rmat->gpu_data, len * sizeof(float), cudaMemcpyDeviceToHost);
    return * rmat;
}

void Matrix::print()
{
    printf("Result = [%.1f, ..., %.1f]\n", this->data[0], this->data[this->cols*this->rows-1]);
}

#endif
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <future>

using namespace std;

const int ARRAY_SIZE = 10000000;
const float A = 3.14;

/**
 * Utility function to generate a random array
 */
void initArray(float *array) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = dis(gen);
    }
}

/**
 * Utility function to display an array
 */
void printVector(const string &label, float *vec) {
    cout << label << ": " << "[";
    for (int i = 0; i < ARRAY_SIZE; i+=ARRAY_SIZE-1)
        cout << vec[i] << (i == ARRAY_SIZE - 1 ? "" : ", ");
    cout << "]" << endl;
}

/**
 * Kernel function: each thread compute an element of the array
 */
__global__ void saxpy(float *x, float *y, float*z) {
    // We need to compute the index of the element to be computed by this thread
    // This is given by the block_index_inside_the_grid * block_dimension + thread_index_inside_the_block
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < ARRAY_SIZE) { // check if the thread is actually computing an element inside the array
        z[idx] = A * x[idx] + y[idx];
    }
}

int main(int argc, char const *argv[])
{
    // Allocate host (CPU) memory
    float *h_x, *h_y, *h_z;
    h_x = new float[ARRAY_SIZE];
    h_y = new float[ARRAY_SIZE];
    h_z = new float[ARRAY_SIZE];

    // Initialize arrays
    initArray(h_x);
    initArray(h_y);

    // Allocate device (GPU) memory and copy the three arrays in it
    float *d_x, *d_y, *d_z;
    cudaMalloc((void**)&d_x, ARRAY_SIZE * sizeof(float));
    cudaMalloc((void**)&d_y, ARRAY_SIZE * sizeof(float));
    cudaMalloc((void**)&d_z, ARRAY_SIZE * sizeof(float));
    cudaMemcpy(d_x, h_x, ARRAY_SIZE * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, h_y, ARRAY_SIZE * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_z, h_z, ARRAY_SIZE * sizeof(float), cudaMemcpyHostToDevice);

    // Define grid and block size
    int blockSize = 256;
    dim3 grid((ARRAY_SIZE + blockSize - 1) / blockSize);
    dim3 block(blockSize);

    auto start = chrono::high_resolution_clock::now();

    // Launch the kernel function with the defined grid and block dimensions
    saxpy<<<grid, block>>>(d_x, d_y, d_z);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    // Copy the result from the device to the host
    cudaMemcpy(h_z, d_z, ARRAY_SIZE * sizeof(float), cudaMemcpyDeviceToHost);

    printVector("X", h_x);
    printVector("Y", h_y);
    printVector("Z", h_z);

    cout << "Time taken: " << duration.count() << " microseconds" << endl;

    // Free host and device memory
    delete[] h_x;
    delete[] h_y;
    delete[] h_z;
    cudaFree(d_x);
    cudaFree(d_y);
    cudaFree(d_z);
    return 0;
}

/**
 * 
 * CUDA VERSION OF THE 'SIMPLE 1D STENCIL OPERATION'
 * 
 */

#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>
#include <cuda_runtime.h>
#include <curand_kernel.h>
#include <ctime>

using namespace std;

void printVector(const string &label, float *vec, int n) {
    cout << label << ": " << "[";
    for (int i = 0; i < n; i+=n-1)
        cout << vec[i] << (i == n - 1 ? "" : ", ..., ");
    cout << "]" << endl;
}

// Kernel to initialize the cuRAND states
__global__ void setupCurandStates(curandState *state, unsigned long seed, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        curand_init(seed, idx, 0, &state[idx]);
    }
}

// Kernel to generate random doubles between 0.0 and 1.0 directly on the device
__global__ void initArrayKernel(curandState *state, float *array, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        array[idx] = curand_uniform_double(&state[idx]);
    }
} 

__global__ void kernel(float *input, float *output, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        output[idx] = ((idx != 0 ? input[idx-1] : 0.0) + input[idx] + (idx != n - 1 ? input[idx+1] : 0.0)) / 3.0;
    }
}

int main(int argc, char const *argv[])
{
    int n;
    cout << "Insert the dimension of the array: "; cin >> n;

    float *h_input = new float[n];
    float *h_output = new float[n];
    float *d_input, *d_output;

    int blockSize = 256; dim3 block(blockSize);
    int numBlocks = (n + blockSize - 1) / blockSize; dim3 grid(numBlocks);

    cudaMalloc((void**)&d_input, n * sizeof(float));
    
    curandState *d_states;
    cudaMalloc((void**) &d_states, n * sizeof(curandState));
    setupCurandStates<<<grid, block>>>(d_states, time(NULL), n);
    initArrayKernel<<<grid, block>>>(d_states, d_input, n);
    cudaFree(d_states);
    
    cudaMemcpy(h_input, d_input, n * sizeof(float), cudaMemcpyDeviceToHost);
    printVector("Input vector", h_input, n);

    cudaMalloc((void**)&d_output, n * sizeof(float));

    auto start = chrono::high_resolution_clock::now();

    kernel<<<grid, block>>>(d_input, d_output, n);
    cudaDeviceSynchronize();

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    cudaMemcpy(h_output, d_output, n * sizeof(float), cudaMemcpyDeviceToHost);
    printVector("Output vector", h_output, n);

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    
    delete[] h_input;
    delete[] h_output;
    cudaFree(d_input);
    cudaFree(d_output);

    return 0;
}

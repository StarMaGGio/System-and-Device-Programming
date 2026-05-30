/**
 * 
 * CUDA VERSION OF THE 'VECTOR ELEMENT-WISE TRANSFORMATION'
 * 
 */

#include <iostream>
#include <cuda_runtime.h>
#include <functional>
#include <chrono>
#include <random>
#include <cmath>
#include <ctime>
#include <curand_kernel.h>

using namespace std;

// Macro for checking CUDA errors
#define CUDA_CHECK(call) \
    do { \
        cudaError_t err = call; \
        if (err != cudaSuccess) { \
            cerr << "CUDA error at " << __FILE__ << ":" << __LINE__ << " - " << cudaGetErrorString(err) << endl; \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

void printVector(const string &label, double *vec, int size) {
    cout << label << ": " << "[";
    if (size > 0) {
        cout << vec[0];
        if (size > 1) {
            // Print the last element if size > 1, similar to original logic
            cout << ", " << vec[size - 1];
        }
    }
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
__global__ void initArrayKernel(curandState *state, double *array, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        array[idx] = curand_uniform_double(&state[idx]);
    }
} 

// --- Example Function A: x^2 + 2x + 1 ---
__device__ double opA(double x) {
    return (x * x) + (2 * x) + 1;
}

// --- Example Function B: sin(x) + cos(x) ---
__device__ double opB(double x) {
    double s, c;
    // sincos is a CUDA math optimization to compute sine and cosine simultaneously
    sincos(x, &s, &c);
    return s + c;
}

// --- Example Function C: Conditional Scaling ---
__device__ double threshold = 2.5;
__device__ double scale_factor = 20.0;
__device__ double opC(double x) {
    return (x > threshold) ? (x * scale_factor) : x;
}

__global__ void parallelTransformationKernel(double *inputVec, double *outputVecA, double *outputVecB, double *outputVecC, int n) {
    // Compute the global index of the element of the input vector
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        double element = inputVec[idx];
        outputVecA[idx] = opA(element);
        outputVecB[idx] = opB(element);
        outputVecC[idx] = opC(element);
    }
}

int main(int argc, char const *argv[])
{
    int n;
    cout << "Insert input vector size: "; cin >> n;

    // Define grid and block size
    int blockSize = 256; dim3 block(blockSize);
    int numBlocks = (n + blockSize - 1) / blockSize; dim3 grid(numBlocks);

    // Initialize input and output vector
    double *h_inputVec = new double[n];
    double *h_outputVecA = new double[n];
    double *h_outputVecB = new double[n];
    double *h_outputVecC = new double[n];

    // Allocate device memory and copy the input array
    double *d_inputVec, *d_outputVecA, *d_outputVecB, *d_outputVecC;
    CUDA_CHECK(cudaMalloc((void**) &d_inputVec, n * sizeof(double)));
    
    // Setup cuRAND states and initialize input array natively on the device
    curandState *d_states;
    CUDA_CHECK(cudaMalloc((void**) &d_states, n * sizeof(curandState)));
    setupCurandStates<<<grid, block>>>(d_states, time(NULL), n);
    CUDA_CHECK(cudaGetLastError()); // Check for kernel launch errors
    initArrayKernel<<<grid, block>>>(d_states, d_inputVec, n);
    CUDA_CHECK(cudaGetLastError());
    
    // Copy the generated data back to host just so we can print it
    CUDA_CHECK(cudaMemcpy(h_inputVec, d_inputVec, n * sizeof(double), cudaMemcpyDeviceToHost));
    cout << "Processing " << n << " elements in CUDA parallel..." << endl;
    printVector("Input (first, last)", h_inputVec, n);

    CUDA_CHECK(cudaMalloc((void**) &d_outputVecA, n * sizeof(double)));
    CUDA_CHECK(cudaMalloc((void**) &d_outputVecB, n * sizeof(double)));
    CUDA_CHECK(cudaMalloc((void**) &d_outputVecC, n * sizeof(double)));
    
    auto start = chrono::high_resolution_clock::now();

    // Launch the kernel function with the defined grid and block dimensions
    parallelTransformationKernel<<<grid, block>>>(d_inputVec, d_outputVecA, d_outputVecB, d_outputVecC, n);
    CUDA_CHECK(cudaDeviceSynchronize()); // Ensure kernel completes before stopping timer
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    CUDA_CHECK(cudaMemcpy(h_outputVecA, d_outputVecA, n * sizeof(double), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(h_outputVecB, d_outputVecB, n * sizeof(double), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(h_outputVecC, d_outputVecC, n * sizeof(double), cudaMemcpyDeviceToHost));

    printVector("Output A (first, last)", h_outputVecA, n);
    printVector("Output B (first, last)", h_outputVecB, n);
    printVector("Output C (first, last)", h_outputVecC, n);

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    // Free host and device memory
    delete[] h_inputVec; 
    delete[] h_outputVecA;
    delete[] h_outputVecB;
    delete[] h_outputVecC;
    cudaFree(d_inputVec); 
    cudaFree(d_outputVecA);
    cudaFree(d_outputVecB);
    cudaFree(d_outputVecC);
    cudaFree(d_states);

    return 0;
}

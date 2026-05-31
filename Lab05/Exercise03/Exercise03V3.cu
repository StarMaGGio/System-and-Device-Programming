/**
 * 
 * CUDA VERSION OF THE 'HISTOGRAM GENERATION FOR A SMALL NUMBER OF BINS'
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

// Kernel to generate random integer values (stored as floats) between 0 and 100 directly on the device
__global__ void initArrayKernel(curandState *state, float *array, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        array[idx] = (float)(curand(&state[idx]) % 101);
    }
} 

__global__ void kernel(float *scores, int *bins, int n, int num_bins) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int tid = threadIdx.x;

    extern __shared__ int local_bins[];

    // 1. Initialize shared memory bins to 0
    for (int i = tid; i < num_bins; i += blockDim.x) {
        local_bins[i] = 0;
    }
    __syncthreads();

    // 2. Compute local block histogram
    if (idx < n) {
        int bin_idx = static_cast<int>(scores[idx]) / 10;
        if (bin_idx >= num_bins) bin_idx = num_bins - 1;

        atomicAdd(&local_bins[bin_idx], 1);
    }
    __syncthreads();

    // 3. Add local block results into the global memory bins
    // Being only 10 bins the loop will have only one iteration where threads from 0 to 9
    // of each block update the respective bin in parallel with atomicAdd
    for (int i = tid; i < num_bins; i += blockDim.x) {
        atomicAdd(&bins[i], local_bins[i]);
    }
}

int main(int argc, char const *argv[])
{
    int n;
    cout << "Insert the dimension of the array: "; cin >> n;

    float *h_scores = new float[n];
    float *d_scores;

    int num_bins = 10;
    int *h_bins = new int[num_bins]();
    int *d_bins;

    int blockSize = 256; dim3 block(blockSize);
    int numBlocks = (n + blockSize - 1) / blockSize; dim3 grid(numBlocks);

    cudaMalloc((void**)&d_scores, n * sizeof(float));

    curandState *d_states;
    cudaMalloc((void**) &d_states, n * sizeof(curandState));
    setupCurandStates<<<grid, block>>>(d_states, time(NULL), n);
    initArrayKernel<<<grid, block>>>(d_states, d_scores, n);
    cudaFree(d_states);

    cudaMemcpy(h_scores, d_scores, n * sizeof(float), cudaMemcpyDeviceToHost);
    printVector("Input vector", h_scores, n);

    cudaMalloc((void**)&d_bins, num_bins * sizeof(int));
    cudaMemset(d_bins, 0, num_bins * sizeof(int));

    auto start = chrono::high_resolution_clock::now();

    size_t shared_mem_size = num_bins * sizeof(int);
    kernel<<<grid, block, shared_mem_size>>>(d_scores, d_bins, n, num_bins);
    cudaDeviceSynchronize();
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    cudaMemcpy(h_bins, d_bins, num_bins * sizeof(int), cudaMemcpyDeviceToHost);

    for (int b = 0; b < num_bins; b++) {
        cout << b*10 << "-" << (b == num_bins - 1 ? 100 : b * 10 + 9) << ": " << h_bins[b] << endl;
    }

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    delete[] h_scores;
    delete[] h_bins;
    cudaFree(d_scores);
    cudaFree(d_bins);

    return 0;
}

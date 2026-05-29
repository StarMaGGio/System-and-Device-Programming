#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 1024

/**
 * Utility function to generate a random matrix
 */
void generateRandomMatrix(int* m, int nRow, int nCol) {
    for (int i = 0; i < nRow * nCol; i++) {
        m[i] = rand() % 100;
    }
}

__global__ void processMatrixRowSum(int *M) {
    // Allocate shared memory with the dimension of a row
    // to avoid to read two times the same elements from two threads in a block
    // This variable has a block visibility, so every block will have its s_row shared array
    __shared__ int s_row[N];

    int row = blockIdx.x; // Every block contains an entire row
    int col = threadIdx.x; // The col is the thread inside the block

    int global_idx = row * N + col; // Compute the global index of the 1D flattened matrix

    // Insert the current element in the shared array
    s_row[col] = M[global_idx];

    __syncthreads(); // Synchronize to ensure all threads has written their value in s_row

    if (col < N - 1) { // Check to avoid out-of-bounds at the end of the col
        // Compute the result by reading the values from the shared memory
        M[global_idx] = s_row[col] + s_row[col + 1];
    }
}

int main(int argc, char const *argv[])
{
    int* h_M = new int[N * N];
    int* d_M;

    cudaMalloc((void**)&d_M, N * N * sizeof(int));

    generateRandomMatrix(h_M, N, N);

    cudaMemcpy(d_M, h_M, N * N * sizeof(int), cudaMemcpyHostToDevice);

    dim3 grid(N);
    dim3 block(N);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    processMatrixRowSum<<<grid, block>>>(d_M);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaMemcpy(h_M, d_M, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    cout << "Kernel execution time: " << milliseconds << " ms" << endl;

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    delete[] h_M; cudaFree(d_M);
    
    return 0;
}

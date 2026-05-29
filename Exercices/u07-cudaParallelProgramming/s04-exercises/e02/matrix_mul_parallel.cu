#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

/**
 * Utility function to generate a random matrix
 */
void generateRandomMatrix(int* m, int nRow, int nCol) {
    for (int i = 0; i < nRow * nCol; i++) {
        m[i] = rand() % 100;
    }
}

/**
 * Utility function to display a matrix
 */
void printMatrix(const vector<vector<int>>& m) {
    for (auto &row : m) {
        for (int element : row) 
            cout << element << " ";
        cout << endl;
    }
}

/**
 * Kernel function that compute the sum of the multiplication of the elements of row A and col B
 */
__global__ void matrixMultiplyKernel(int* A, int* B, int*C, int nRowA, int nColA, int nColB) {
    // We need to pass from the 2D matrix of values to the 1D global array of threads
    int r = blockIdx.y * blockDim.y + threadIdx.y; // row index of the element computed by this thread
    int c = blockIdx.x * blockDim.x + threadIdx.x; // col index ...

    if (r < nRowA && c < nColB) { // check if we are inside the matrix
        int sum = 0;
        for (int k = 0; k < nColA; k++) { // Iterate over the elements of row A and col B
            sum += A[r * nColA + k] * B[k * nColB + c];
        }
        C[r * nColB + c] = sum;
    }
}

int main(int argc, char const *argv[])
{
    int nRowA, nColA, nRowB, nColB;

    cout << "Insert size of matrix A: "; cin >> nRowA >> nColA;
    cout << "Insert size of matrix B: " << nColA << " "; cin >> nColB;
    nRowB = nColA;

    int* h_A = new int[nRowA * nColB];
    int* h_B = new int[nRowB * nColB];
    int* h_C = new int[nRowA * nColB];
    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, nRowA * nColA * sizeof(int));
    cudaMalloc((void**)&d_B, nRowB * nColB * sizeof(int));
    cudaMalloc((void**)&d_C, nRowA * nColB * sizeof(int));

    generateRandomMatrix(h_A, nRowA, nColA);
    generateRandomMatrix(h_B, nRowB, nColB);

    cudaMemcpy(d_A, h_A, nRowA * nColA * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, nRowB * nColB * sizeof(int), cudaMemcpyHostToDevice);

    int blockSize = 16;
    dim3 block(blockSize, blockSize);
    dim3 grid((nColB + blockSize - 1) / blockSize, (nRowA + blockSize -1) / blockSize);

    auto start = chrono::high_resolution_clock::now();

    matrixMultiplyKernel<<<grid, block>>>(d_A, d_B, d_C, nRowA, nColA, nColB);
    cudaDeviceSynchronize(); // Wait for the GPU to finish before stopping the timer

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cudaMemcpy(h_C, d_C, nRowA * nColB * sizeof(int), cudaMemcpyDeviceToHost);
    cout << "Time taken: " << duration.count() << " microseconds" << endl;

    delete[] h_A; delete[] h_B; delete[] h_C;
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);

    return 0;
}

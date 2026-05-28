#include <stdio.h>
#include <cuda_runtime.h>

__global__ void my_kernel() {
    if (threadIdx.x == 0 && blockIdx.x == 0 &&
        threadIdx.y == 0 && blockIdx.y == 0 &&
        threadIdx.z == 0 && blockIdx.z == 0 )
            printf("%d %d %d %d %d %d\n",
                gridDim.x, gridDim.y, gridDim.z,
                blockDim.x, blockDim.y, blockDim.z);
}

int main() {
    dim3 gridDim(2, 3, 4);              // 2*3*4 blocks grid
    dim3 blockDim(5, 6, 7);             // with blocks of 5*6*7 threads
    my_kernel<<<gridDim, blockDim>>>(); // In total -> 2*3*4*5*6*7 = 5040 threads
    cudaDeviceSynchronize();
    return 0;
}
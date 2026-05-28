#include <stdio.h>

#define N 100

__global__ void myf() {
    printf("%d\n", threadIdx.x*threadIdx.x);
}

int main() {
    /**
     * No loop: Launch 1*100 thread grid that execute in parallel
     * In particular, grid 1*1*1 with a block 1*1*100
     */
    myf<<<1,N>>>();
    cudaDeviceSynchronize();
    return 0;
}
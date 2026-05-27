#include <stdio.h>

#define N 100

/**
 * Kernel Function executed by the GPU
 */
__global__ void myf() {
    for (int i = 0; i < N; i++) {
        printf("%d\n", i*i);
    }
}

int main(int argc, char const *argv[])
{
    myf<<<1,1>>>();
    cudaDeviceSynchronize();
    return 0;
}
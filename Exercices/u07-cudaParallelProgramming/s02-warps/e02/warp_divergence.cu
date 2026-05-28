#include <stdio.h>
#include <cuda_runtime.h>

__global__ void Kernel_divergence(int x, int y, int z) {
    assert (x == y || x == z);
    if (x == y)
        x = z;
    else 
        x = y;
}

__global__ void kernel_no_divergence(int x, int y, int z) {
    assert (x == y || x == z);
    x = y + z - x;
}
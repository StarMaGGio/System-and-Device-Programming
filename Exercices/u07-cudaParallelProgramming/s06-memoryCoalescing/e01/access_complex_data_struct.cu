#include <iostream>
#include <cuda_runtime.h>

using namespace std;

typedef struct node_s {
    char a;
    int b;
    float c;
} node_t;
/**
 * In this data structure (Array of Structures)
 * objects are contiguous but their fields are interleaved.
 * [a, b, c, a, b, c, a, b, c, ...] and so they are in memory.
 */
__global__ void mykernel(node_t *v) {
    unsigned idx = blockIdx.x * blockDim.x + threadIdx.x;
    v[idx].a = 'x'; // Threads in the same warp access fields a of different objects, which are not contiguous.
    v[idx].b = idx;
    v[idx].c = 0.0;
}

typedef struct node_s_v {
    char *va;
    int *vb;
    float *vc;
} node_t_v;
/**
 * In this data structure (Structure of Arrays)
 * there is one only object with all fields in its proper array.
 * [a, a, a, ..., b, b, b, ..., c, c, c, ...] so also in memory they will be stored in a contiguous way.
 */
__global__ void mykernel (char *va, int *vb, float *vc) {
    unsigned idx = blockIdx.x * blockDim.x + threadIdx.x;
    va[idx] = 'x'; // Threads in the same warp access fields a in a contiguous way from the memory.
    vb[idx] = idx;
    vc[idx] = 0.0;
}

int main(int argc, char const *argv[])
{
    node_t *v;
    node_t_v e;

    return 0;
}

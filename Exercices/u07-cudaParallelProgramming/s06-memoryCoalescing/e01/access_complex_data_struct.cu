#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 1024

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
__global__ void mykernel_v (char *va, int *vb, float *vc) {
    unsigned idx = blockIdx.x * blockDim.x + threadIdx.x;
    va[idx] = 'x'; // Threads in the same warp access fields a in a contiguous way from the memory.
    vb[idx] = idx;
    vc[idx] = 0.0;
}

int main(int argc, char const *argv[])
{
    /**
     * Array of Structures (Not optimal for GPU)
     */
    node_t *h_v = (note_t*) malloc (N * sizeof(note_t));

    note_t *d_v;

    cudaMalloc((void**)&d_v, N * sizeof(note_t));

    cudaMemcpy(d_v, h_v, N * sizeof(note_t), cudaMemcpyHostToDevice);

    free(h_v);
    cudaFree(d_v);

    /**
     * Structure of Arrays (Optimal for GPU - memory coaleshing)
     */
    char *h_va = (char*) malloc (N * sizeof(char));
    int *h_vb = (int*) malloc (N * sizeof(int));
    float *h_vc = (float*) malloc (N * sizeof(float));

    note_t_v d_e;

    cudaMalloc((void**)&d_e.va, N * sizeof(char));
    cudaMalloc((void**)&d_e.vb, N * sizeof(int));
    cudaMalloc((void**)&d_e.vc, N * sizeof(float));

    cudaMemcpy(d_e.va, h_va, N * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_e.vb, h_vb, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_e.vc, h_vc, N * sizeof(float), cudaMemcpyHostToDevice);

    free(h_va); free(h_vb); free(h_vc);
    cudaFree(d_e);

    return 0;
}

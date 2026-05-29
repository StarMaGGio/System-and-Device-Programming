#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <future>

using namespace std;

const int ARRAY_SIZE = 1000000;
const float A = 3.14;
const int N_THREADS = 8;

void saxpy_parallel(float, float*, float*, float*);

/**
 * Utility function to generate a random array
 */
void initArray(float *array) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = dis(gen);
    }
}

/**
 * Utility function to display an array
 */
void printVector(const string &label, float *vec) {
    cout << label << ": " << "[";
    for (int i = 0; i < ARRAY_SIZE; i++)
        cout << vec[i] << (i == ARRAY_SIZE - 1 ? "" : ", ");
    cout << "]" << endl;
}

int main(int argc, char const *argv[])
{
    float* x = new float[ARRAY_SIZE];
    float* y = new float[ARRAY_SIZE];
    float* z = new float[ARRAY_SIZE];

    initArray(x);
    initArray(y);
    
    auto start = chrono::high_resolution_clock::now();

    saxpy_parallel(A, x, y, z);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    printVector("X", x);
    printVector("Y", y);
    printVector("Z", z);
    
    cout << "Time taken: " << duration.count() << " microseconds" << endl;

    delete[] x;
    delete[] y;
    delete[] z;
    return 0;
}

void saxpy_parallel(float A, float *x, float *y, float *z) {
    vector<future<void>> tasks;
    tasks.reserve(N_THREADS); // preallocate a block of memory large enough to hold N_THREADS number of futures
    
    // compute the number of elements of the array for each thread
    int base_chunk = ARRAY_SIZE / N_THREADS;
    int remainder = ARRAY_SIZE % N_THREADS;
    
    // launch N_THREADS tasks to each one compute a chunk of the elements of the array
    for (int t = 0; t < N_THREADS; t++) {
        int start = t * base_chunk;
        int end = (t + 1) * base_chunk + (t == N_THREADS - 1 ? remainder : 0);
        tasks.push_back(async (launch::async, [A, x, y, z, start, end]() {
            for (int i = start; i < end && i < ARRAY_SIZE; i++) 
                z[i] = A * x[i] + y[i];
        }));
    }

    // wait for all tasks to finish
    for (auto &f : tasks) {
        f.get();
    }
}

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

const int ARRAY_SIZE = 1000000;
const float A = 3.14;

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
    
    for (int i = 0; i < ARRAY_SIZE; i++)
        z[i] = A * y[i] + x[i];
        
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

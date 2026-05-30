/**
 * 
 * SEQUENTIAL VERSION OF THE 'SIMPLE 1D STENCIL OPERATION'
 * 
 */

#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>
#include "../utils.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
    int n;
    cout << "Insert the dimension of the array: "; cin >> n;

    float *input = new float[n];
    float *output = new float[n];

    initArray(input, n);
    printVector("Input vector", input, n);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        output[i] = ((i != 0 ? input[i-1] : 0.0) + input[i] + (i != n - 1 ? input[i+1] : 0.0)) / 3.0;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    printVector("Output vector", output, n);

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}

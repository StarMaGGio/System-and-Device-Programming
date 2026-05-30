/**
 * 
 * PARALLEL VERSION OF THE 'SIMPLE 1D STENCIL OPERATION'
 * 
 */

#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>
#include <future>
#include <thread>
#include <vector>
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

    int max_threads = thread::hardware_concurrency();
    if (max_threads == 0) max_threads = 4;
    cout << "Working Threads: " << max_threads << endl;

    int chunk_size = n / max_threads;

    vector<future<void>> futures;
    for (int i = 0; i < max_threads; i++) {
        int start = i * chunk_size;
        int end = i == max_threads - 1 ? n : (i + 1) * chunk_size;
    
        // function that will be executed in parallel in threads
        futures.push_back(async (launch::async, [&input, &output, start, end, n, i]() {
            for (int j = start; j < end; j++) {
                output[j] = ((i != 0 ? input[j-1] : 0.0) + input[j] + (j != n - 1 ? input[j+1] : 0.0)) / 3.0;
            }
        }));
    }

    for (auto & f : futures) {
        f.get();
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    printVector("Output vector", output, n);

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}

/**
 * 
 * PARALLEL VERSION OF THE 'HISTOGRAM GENERATION FOR A SMALL NUMBER OF BINS'
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

void initArrayInt(float *array, int n, int min = 0, int max = 100) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(min, max);

    for (int i = 0; i < n; i++) {
        array[i] = static_cast<float>(dis(gen));
    }
}

int main(int argc, char const *argv[])
{
    int n;
    cout << "Insert the dimension of the array: "; cin >> n;

    float *scores = new float[n];

    initArrayInt(scores, n, 0, 100);
    printVector("Exam scores", scores, n);

    int num_bins = 10;
    int *bins = new int[num_bins]();
    
    int max_threads = thread::hardware_concurrency() + 8;
    cout << "Working Threads: " << max_threads << endl;

    int chunk_size = n / max_threads;

    auto start = chrono::high_resolution_clock::now();

    vector<future<vector<int>>> futures;
    for (int i = 0; i < max_threads; i++) {
        int start = i * chunk_size;
        int end = i == max_threads - 1 ? n : (i + 1) * chunk_size;

        futures.push_back(async (launch::async, [&scores, start, end, num_bins]() {
            vector<int> local_bins(num_bins, 0);
            for (int j = start; j < end; j++) {
                int bin_idx = static_cast<int>(scores[j]) / 10;
                if (bin_idx == num_bins) bin_idx = num_bins - 1;
                
                local_bins[bin_idx]++;
            }
            return local_bins;
        }));
    }

    for (auto & f: futures) {
        vector<int> local_result = f.get();
        for (int b = 0; b < num_bins; b++) {
            bins[b] += local_result[b];
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    for (int b = 0; b < num_bins; b++) {
        cout << b*10 << "-" << (b == num_bins - 1 ? 100 : b * 10 + 9) << ": " << bins[b] << endl;
    }

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    delete[] scores;
    delete[] bins;

    return 0;
}

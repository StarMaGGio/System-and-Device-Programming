/**
 * 
 * SEQUENTIAL VERSION OF THE 'HISTOGRAM GENERATION FOR A SMALL NUMBER OF BINS'
 * 
 */

#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>
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
    int *bins = new int[num_bins](); // Each element of the array stores how many scores fall in that bin

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        int bin_idx = static_cast<int>(scores[i]) / 10;
        if (bin_idx == num_bins) bin_idx = num_bins - 1;
        bins[bin_idx]++;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    for (int b = 0; b < num_bins; b++) {
        cout << b*10 << "-" << (b == num_bins - 1 ? 100 : b * 10 + 9) << ": " << bins[b] << endl;
    }

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}

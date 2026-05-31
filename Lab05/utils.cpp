#include <iostream>
#include <vector>
#include <random>

using namespace std;

/**
 * Utility function to generate a random array
 */
void initArray(float *array, int n, float min = 0.0f, float max = 1.0f) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(min, max);

    for (int i = 0; i < n; i++) {
        array[i] = dis(gen);
    }
}

/**
 * Utility function to display an array by its pointer and length
 */
void printVector(const string &label, float *vec, int n) {
    cout << label << ": " << "[";
    for (int i = 0; i < n; i+=n-1)
        cout << vec[i] << (i == n - 1 ? "" : ", ..., ");
    cout << "]" << endl;
}

/**
 * Function to print a vector of double passed as parameter
 */
void printVector(const string& name, const vector<double>& vec) {
    cout << name << ": [ ";
    for (double val : vec) {
        cout << val << " ";
    }
    cout << "]" << endl;
}
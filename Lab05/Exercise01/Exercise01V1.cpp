/**
 * 
 * SEQUENTIAL VERSION OF THE 'VECTOR ELEMENT-WISE TRANSFORMATION'
 * 
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <chrono>

using namespace std;

/**
 * Function that take an input vector and apply any element-wise transformation
 * to its elements in a sequential way
 */
vector<double> sequentialTransform(const vector<double>& input, const function<double(double)>& transformOp) {
    // Allocate an output vector of the same size of the input one
    vector<double> output(input.size());

    // Iterate through each element of the input vector (SEQUENTIALLY)
    // and apply the trasformOp
    for (int i = 0; i < input.size(); i++) {
        output[i] = transformOp(input[i]);
    }

    return output;
}

int main(int argc, char const *argv[])
{
    // Initialize the input vector
    vector<double> inputVec(10000000);
    for (int i = 0; i < inputVec.size(); i++) {
        inputVec[i] = static_cast<double>(i);
    }

    cout << "Processing " << inputVec.size() << " elements sequentially..." << endl;
    auto start = chrono::high_resolution_clock::now();

    // --- Example Function A: x^2 + 2x + 1 ---
    auto opA = [](double x) { return (x * x) + (2 * x) + 1; }; // lambda function
    vector<double> outputVecA = sequentialTransform(inputVec, opA);

    // --- Example Function B: sin(x) + cos(x) ---
    auto opB = [](double x) { return sin(x) + cos(x); };
    vector<double> outputVecB = sequentialTransform(inputVec, opB);

    // --- Example Function C: Conditional Scaling ---
    double threshold = 2.5;
    double scale_factor = 10.0;
    auto opC = [threshold, scale_factor](double x) { return (x > threshold) ? (x * scale_factor) : x; };
    vector<double> outputVecC = sequentialTransform(inputVec, opC);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Done! First element: " << outputVecA[0] << ", Last element: " << outputVecC.back() << endl;
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}

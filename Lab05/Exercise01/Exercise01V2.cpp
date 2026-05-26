/**
 * 
 * PARALLEL VERSION OF THE 'VECTOR ELEMENT-WISE TRANSFORMATION'
 * 
 */

 #include <iostream>
 #include <vector>
 #include <cmath>
 #include <functional>
 #include <future>
 #include <thread>
 #include <chrono>

using namespace std;

 /**
 * Function that take an input vector and apply any element-wise transformation
 * to its elements in a parallel way
 */
vector<double> parallelTransform(const vector<double>& input, const function<double(double)>& transformOp) {
    // Allocate an output vector of the same size of the input one
    vector<double> output(input.size());

    // Determine optimal number of threads
    size_t max_threads = thread::hardware_concurrency();
    if (max_threads == 0) max_threads = 4;

    // Calculate the size of each chunk
    size_t chunk_size = input.size() / max_threads;

    // Launch an async task for each chunk
    vector<future<void>> futures;
    for (size_t i = 0; i < max_threads; i++) {
        // Compute start and end indexes of the elements for the current thread
        size_t start_idx = i * chunk_size;
        size_t end_idx = (i == max_threads - 1) ? input.size() : (i + 1) * chunk_size; // Last thread compute also division remaining elements

        // Launch async task
        futures.push_back(async (launch::async, [&input, &output, &transformOp, start_idx, end_idx]() {
            /**
             * Loop that runs in parallel on the different threads
             */ 
            for (size_t j = start_idx; j < end_idx; j++) {
                output[j] = transformOp(input[j]);
            }
        }));

    }

    // the .get() blocks the code until all threads returned their value
    for (auto& f : futures) {
        f.get();
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
    vector<double> outputVecA = parallelTransform(inputVec, opA);

    // --- Example Function B: sin(x) + cos(x) ---
    auto opB = [](double x) { return sin(x) + cos(x); };
    vector<double> outputVecB = parallelTransform(inputVec, opB);

    // --- Example Function C: Conditional Scaling ---
    double threshold = 2.5;
    double scale_factor = 10.0;
    auto opC = [threshold, scale_factor](double x) { return (x > threshold) ? (x * scale_factor) : x; };
    vector<double> outputVecC = parallelTransform(inputVec, opC);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "Done! First element: " << outputVecA[0] << ", Last element: " << outputVecC.back() << endl;
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}

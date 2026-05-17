#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>

using namespace std;

void compute_v_v2_product(const vector<double>& v, const vector<double>& v2, double& res) {
    for (int i = 0; i < v.size(); i++) {
        res += v[i] * v2[i];
    }
}

void compute_v1_matrix_product(const vector<double>& v1, const vector<vector<double>>& matrix, vector<double>& v, int i, const vector<double>&v2, double& res, atomic<int>& completed_threads) {
    for (int j = 0; j < v1.size(); j++) {
        v[i] += v1[j] * matrix[i][j];
    }

    // Check if I am the last thread to finish computing "v"
    if (++completed_threads == v1.size()) {
        compute_v_v2_product(v, v2, res);
    }
}



int main(int argc, char const *argv[]) {
    
    // Take the size of the vectors from the command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Create two vectors of size n and a matrix of size n x n
    vector<double> v1(n);
    vector<double> v2(n);
    vector<vector<double>> matrix(n, vector<double>(n));

    // Initialize the vectors and the matrix with random variables in range [-0.5, 0.5]
    for (int i = 0; i < n; i++) {
        v1[i] = rand() / double(RAND_MAX) - 0.5;
        v2[i] = rand() / double(RAND_MAX) - 0.5;
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() / double(RAND_MAX) - 0.5;
        }
    }

    // Debug
    // cout << "v1: ";
    // for (int i = 0; i < n; i++) {
    //     cout << v1[i] << " ";
    // }
    // cout << endl;
    // cout << "matrix: " << endl;
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // Create and run n threads to compute the product of v1 and the matrix
    vector<thread> threads;
    vector<double> v(n, 0.0);
    double res = 0.0;
    atomic<int> completed_threads(0);
    
    for (int i =0; i < n; i++) {
        threads.emplace_back(compute_v1_matrix_product, ref(v1), ref(matrix), ref(v), i, ref(v2), ref(res), ref(completed_threads));
    }

    // Wait for all threads to finish concurrently
    for (int i = 0; i < n; i++) {
        threads[i].join();
    }

    // Print the result
    cout << "res = " << res << endl;

    return 0;
}

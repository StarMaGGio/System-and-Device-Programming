#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <future>

using namespace std;

/**
 * Utility function to generate a random matrix
 */
void generateRandomMatrix(vector<vector<int>>& m, int nRow, int nCol) {
    for (int i = 0; i < nRow; i++) {
        vector<int> row;
        for (int j = 0; j < nCol; j++) 
            row.push_back(rand()%100);
        m.push_back(row);
    }
}

/**
 * Utility function to display a matrix
 */
void printMatrix(const vector<vector<int>>& m) {
    for (auto &row : m) {
        for (int element : row) 
            cout << element << " ";
        cout << endl;
    }
}

/**
 * Function that compute the element given by a row and a column
 * To be executed in parallel (each thread compute one element)
 */
int computeSumOfProducts (const vector<int>& v1, const vector<int>& v2) {
    return inner_product(v1.begin(), v1.end(), v2.begin(), 0);
}

int main(int argc, char const *argv[])
{
    int nRowA, nColA, nRowB, nColB;
    vector<vector<int>> A, B;
    vector<vector<future<int>>> futures; // C

    cout << "Insert size of matrix A: "; cin >> nRowA >> nColA;
    cout << "Insert size of matrix B: " << nColA << " "; cin >> nColB;
    nRowB = nColA;

    generateRandomMatrix(A, nRowA, nColA);
    generateRandomMatrix(B, nRowB, nColB);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < nRowA; i++) { // Iterate over rows of A
        vector<future<int>> futureRow;
        for (int j = 0; j < nColB; j++) { // Iterate over columns of B
            future<int> f = async (launch::deferred, computeSumOfProducts, A[i], B[j]); // Launch a task to compute the element
            futureRow.push_back(move(f)); // Move the ownership of the future result to the promise of the futureRow
        }
        futures.push_back(move(futureRow));
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    //cout << "A" << endl; printMatrix(A);
    //cout << "B" << endl; printMatrix(B);
    // cout << "C" << endl;
    // for (auto &row : futures) {
    //     for (future<int> &f : row)
    //         cout << f.get() << " ";
    //     cout << endl;
    // }

    cout << "Time taken: " << duration.count() << " microseconds" << endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

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
 * Function to multiply to matrix in a sequential way
 */
void matrixMultiply(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C, int rowsA, int colsA, int colsB) { 
    for (int i = 0; i < rowsA; i++) { // iterate over the rows of A
        vector<int> row;
        for (int j = 0; j < colsB; j++) { // iterate over the columns of B
            int sum = 0;
            for (int k = 0; k < colsA; k++) { // iterate over the elements of row A and col B
                sum += A[i][k] * B[k][j];
            }
            row.push_back(sum); // add the element to the result row
        }
        C.push_back(row); // add the row to the result matrix
    }
}

int main(int argc, char const *argv[])
{
    int nRowA, nColA, nRowB, nColB;
    vector<vector<int>> A, B, C;

    cout << "Insert size of matrix A: "; cin >> nRowA >> nColA;
    cout << "Insert size of matrix B: " << nColA << " "; cin >> nColB;
    nRowB = nColA;

    generateRandomMatrix(A, nRowA, nColA);
    generateRandomMatrix(B, nRowB, nColB);

    auto start = chrono::high_resolution_clock::now();

    matrixMultiply(A, B, C, nRowA, nColA, nColB);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "A" << endl; printMatrix(A);
    cout << "B" << endl; printMatrix(B);
    cout << "C" << endl; printMatrix(C);

    cout << "Time taken: " << duration.count() << " microseconds" << endl;

    return 0;
}
#include <iostream>
#include <vector>

using namespace std;

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
#include "functions.h"

void printArray(float *array, int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << "]";
}
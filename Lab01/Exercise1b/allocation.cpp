#include "functions.h"

float* dynamicAllocArray(int n) {
    // dynamic allocation of n elements of type double
    float *array;
    array = (float*)malloc(n * sizeof(float));

    cout << "Enter " << n << " real numbers" << endl;

    // read from the std in
    for (int i = 0; i < n; i++) {
        cout << "Element " << i << ": ";
        cin >> array[i];
    }

    return array;
}
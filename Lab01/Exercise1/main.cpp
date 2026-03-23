#include <iostream>
#include <cstdlib>

using namespace std;

// Function to dynamically allocates an array of size "n" of real values and reads the array from standard input
// Return the pointer to the allocated area of memory
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

int compareFloat(const void* a, const void* b) {
    float arg1 = *(const float*)a;
    float arg2 = *(const float*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

// Function to sorts the array in increasing order
void sortIncreasing(float *array, int length) {
    qsort(array, length, sizeof(float), compareFloat);
}

// Function to displays the sorted array on standard output
void printArray(float *array, int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << "]";
}

int main(int argc, char ** argv) {
    int n;
    float* array;

    cout << "Enter an integer value: " << endl;
    cin >> n;

    array = dynamicAllocArray(n);
    sortIncreasing(array, n);
    printArray(array, n);

    free(array);
    return 0;
}
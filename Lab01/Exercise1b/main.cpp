#include "functions.h"

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
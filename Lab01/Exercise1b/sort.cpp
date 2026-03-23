#include "functions.h"

int compareFloat(const void* a, const void* b) {
    float arg1 = *(const float*)a;
    float arg2 = *(const float*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

void sortIncreasing(float *array, int length) {
    qsort(array, length, sizeof(float), compareFloat);
}
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <cstdlib>

using namespace std;

// Function to dynamically allocates an array of size "n" of real values and reads the array from standard input
// Return the pointer to the allocated area of memory
float* dynamicAllocArray(int n);

int compareFloat(const void* a, const void* b);

// Function to sorts the array in increasing order
void sortIncreasing(float *array, int length);

// Function to displays the sorted array on standard output
void printArray(float *array, int n);

#endif
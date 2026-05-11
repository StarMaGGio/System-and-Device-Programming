#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <thread>
#include <semaphore>

using namespace std;

const int TIME = 3;

map<int, unique_ptr<binary_semaphore>> my_sem; // the map is sorted. Semaphores are stored using dynamic memory since they cannot copied nor moved
mutex m1, // protects I/O
      m2; // protects my_sem

static void worker (int i, int priority);

int main(int argc, char const *argv[])
{
    
    return 0;
}

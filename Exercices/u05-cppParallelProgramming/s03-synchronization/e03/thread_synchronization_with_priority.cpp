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

/*
* Thread function prototype
*/
static void worker (int i, int priority);

/*
* Main function
*/
int main(int argc, char const *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Syntax: %s num_threads\n", argv[0]);
        return 1;
    }

    int i, priority, n = atoi(argv[1]);
    vector<thread> pool;
    for (i=0; i<n; i++) {
        priority = (i+1)*10; // priorities are 10, 20, ...
        pool.emplace_back([i, priority] { worker (i, priority); }); // create a thread with the worker function and pass the thread id and its priority as arguments
    }
    this_thread::sleep_for(chrono::seconds(rand()%TIME));

    i = 0;
    m2.lock(); // protect my_sem during iteration
    for (const auto &t : my_sem) {
        m1.lock();  // protect I/O
        cout << "    Unlocking thread " << i++ << " with priority " << t.first << endl;
        m1.unlock();
        (*(t.second)).release(); // release the semaphore of the worker with the highest priority (the first one in the map)
    }
    m2.unlock();
    for (i=0; i<n; i++) {
        pool[i].join();
    }
    cout << "Main exits." << endl;

    return 1;
}

/*
* Thread function worker:
*/
static void worker (int i, int priority) {
    m1.lock(); // protect I/O
    cout << "Locking thread " << i << " with priority " << priority << endl;
    m1.unlock();

    m2.lock(); // protect my_sem
    my_sem.insert({priority, make_unique<binary_semaphore>(0)});    // insert a new semaphore in the map
    binary_semaphore* sem = my_sem[priority].get();                 // get the pointer before unlocking
    m2.unlock();
    sem->acquire();                                                 // wait for the main thread to release the semaphore of this worker

    m1.lock();
    cout << "        Unlocked thread " << i << " with priority " << priority << endl;
    m1.unlock();
    return;
}
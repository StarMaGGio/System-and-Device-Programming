#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <thread>
#include <semaphore>

using namespace std;

/*
* Structure for global variables to store the best candidate
*/
struct Best {
    int rank;
    thread::id id;
    int num_votes;
    mutex mtx;
};
Best* best; // Pointer to the best candidate
counting_semaphore<0> barrier(0); // Semaphore to make threads wait
mutex io_mtx; // Mutex to protect the output stream

/*
* Functions prototypes
*/
int max_random(int max);
void process(int rank);

/*
* Main
*/
int main(int argc, char const *argv[]) {
    best = new Best();
    best->rank = 0;
    best->num_votes = 0;
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    vector<thread> threads;
    for (int i=0; i<10; i++) {
        int rank = i+1; threads.emplace_back(process, rank);
    }
    for (auto& t : threads) {
        t.join();
    }
    delete best;

    return 0;
}

/*
* Function run in the thread to process the election
*/
void process(int rank) {
    thread::id id = this_thread::get_id();

    // Lock the mutex to update the best candidate
    best->mtx.lock();
    if (rank > best->rank) {
        best->rank = rank;
        best->id = id;
    }
    best->num_votes++;

    if (best->num_votes < 10) {
        // Wait for all threads to vote
        cout << "Thread WAITING id = " << id << ", rank = " << rank << ", best rank = " << best->rank << ", best id = " << best->id << endl;
        best->mtx.unlock();
        barrier.acquire(); // acquire the semaphore to make the thread wait
        io_mtx.lock();
        cout << "    Thread RELEASED id = " << id << ", rank = " << rank << ", best rank = " << best->rank << ", best id = " << best->id << endl;
        io_mtx.unlock();
    } else {
        cout << "  Last Thread id = " << id << " releasing all threads" << endl;
        best->mtx.unlock();
        // Release all waiting threads
        for (int i=0; i<9; i++) {
            barrier.release();
        }
    }
}

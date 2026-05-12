#include <mutex>
#include <condition_variable>

using namespace std;

struct Semaphore {
    int count;
    mutex m;
    condition_variable cv;

    /**
     * Constructor of the Semaphore class. Initializes the count variable with the given value n.
     */
    Semaphore (int n) {
        count = n;
        return;
    }

    /**
     * Wait function that decrements the count variable. If the count is less than or equal to zero, the thread will wait until it is signaled.
     */
    void sem_wait() {
        m.lock(); // Lock the mutex to protect access to the count variable
        count--;
        while (count <= 0) {
            cv.wait(m);
        }
        m.unlock();
    }

    /**
     * Notify function that increments the count variable and signals one waiting thread (if any) to wake up. It locks the mutex before modifying the count variable and unlocks it afterward.
     */
    void notify() {
        m.lock();
        count++;
        cv.notify_one();
        m.unlock();
    }

}


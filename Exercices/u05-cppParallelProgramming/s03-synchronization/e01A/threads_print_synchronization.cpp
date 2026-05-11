#include <iostream>
#include <semaphore>
#include <thread>
#include <unistd.h>

using namespace std;

counting_semaphore sa{1},   // semaphore for thread that print character A
                   sb{2},   // semaphore for thread that print character B
                   me{1};   // mutex semaphore
int n;                      // global counter, it will be visible by all the threads

/*
 * Thread function prototypes
 */
static void TA (int);
static void TB (int);

int main(int argc, char const *argv[]) {
    int n1, n2;

    if (argc != 2) {
        fprintf(stderr, "Syntax: %s num_threads\n", argv[0]);
        return 1;
    }

    n1 = atoi(argv[1]);     // num iterations for TA
    n2 = 2 * n1;            // num iterations for TB

    thread ta (TA, n1), tb (TB, n2);
    ta.join();
    tb.join();

    return 0;
}

/*
 * Thread function TA: prints character A n times
 */
static void TA (int nc) {
    for (int i=0; i<nc; i++) {
        sleep(rand()%2);    // wait for a random time
        sa.acquire();       // wait for semaphore sa to be avaible and acquire it
        me.acquire();       // wait for mutex me to be avaible and acquire it (must not start with TB)
        cout << "A";
        n++;
        if (n >= 3) {
            cout << endl;
            n = 0; sa.release(); sb.release(); sb.release(); // the third thread wakes-up one TA and two TB
        }
        me.release();
    }
    return;
}

/*
 * Thread function TB: prints character B n times
 */
static void TB (int nc) {
    for (int i=0; i<nc; i++) {
        sleep (rand()%2);
        sb.acquire();
        me.acquire();
        cout << "B";
        n++;
        if (n >= 3) {
            cout << endl;
            n = 0; sa.release(); sb.release(), sb.release();
        }
        me.release();
    }
    return;
}


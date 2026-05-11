#include <iostream>
#include <semaphore>
#include <thread>
#include <unistd.h>

using namespace std;

counting_semaphore sp{1},   // semaphore for thread that print character + (this is the one who starts)
                   sm{0},   // semaphore for thread that print character -
                   ss{0},   // semaphore for thread that print character *
                   snl{0};  // semaphore for thread that print character \n

/*
 * Thread function prototypes
 */
static void TP (int);
static void TM (int);
static void TS (int);
static void TNL ();

int main(int argc, char const *argv[]) {
    int n;
    if (argc != 2) {
        fprintf(stderr, "Syntax: %s num_threads\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);      // num iterations
    thread tp (TP, n);
    thread tm (TM, n);
    thread ts (TS, n);
    thread tnl (TNL);
    tp.join();
    tm.join();
    ts.join();
    tnl.join();
    
    return 0;
}

/*
 * Thread function TP: prints character + n times
 */
static void TP (int n) {
    int np = 0;
    while (1) {
        sp.acquire();       // wait for sp semaphore to be avaible and acquire it
        cout << "+";
        np ++;
        if (np < n) {
            sp.release();   // wake-up again TP thread
        } else {
            np = 0;
            snl.release();  // wake-up TNL thread 
        }
    }
    return;
}

/*
 * Thread function TM: prints character - n times
 */
static void TM (int n) {
    int nm = 0;
    while (1) {
        sm.acquire();       // wait for sm semaphore to be avaible and acquire it
        cout << "-";
        nm ++;
        if (nm < n) {
            sm.release();   // wake-up again TM thread
        } else {
            nm = 0;
            snl.release();  // wake-up TNL thread 
        }
    }
    return;
}

/*
 * Thread function TS: prints character * n times
 */
static void TS (int n) {
    int ns = 0;
    while (1) {
        ss.acquire();       // wait for ss semaphore to be avaible and acquire it
        cout << "*";
        ns ++;
        if (ns < n) {
            ss.release();   // wake-up again TS thread
        } else {
            ns = 0;
            snl.release();  // wake-up TNL thread 
        }
    }
    return;
}

/*
 * Thread function TNL: prints character \n after each n characters
 */
static void TNL () {
    int nnl = 0;
    while (1) {
        snl.acquire(); nnl++; cout << endl;
        sleep (rand()%2);
        if (nnl == 1) {
            sm.release();
        } else if (nnl == 2) {
            ss.release();
        } else {
            sp.release(); nnl = 0;
        }
    }
    return;
}


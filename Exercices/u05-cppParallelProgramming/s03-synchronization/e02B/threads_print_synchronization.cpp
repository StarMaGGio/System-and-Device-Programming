#include <iostream>
#include <semaphore>
#include <thread>
#include <unistd.h>

using namespace std;

counting_semaphore sp{1},   // semaphore for thread that print character + (this is the one who starts)
                   sm{0},   // semaphore for thread that print character -
                   ss{0},   // semaphore for thread that print character *
                   snl{0};  // semaphore for thread that print character \n

struct ThreadData {
    int n;
    char c;
};

/*
 * Thread function prototypes
 */
static void TC (ThreadData);
static void TNL ();

int step = 0;

int main(int argc, char const *argv[]) {
    int n;
    if (argc != 2) {
        fprintf(stderr, "Syntax: %s num_threads\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);      // num iterations
    thread tp (TC, ThreadData{n, '+'});
    thread tm (TC, ThreadData{n, '-'});
    thread ts (TC, ThreadData{n, '*'});
    thread tnl (TNL);
    tp.join();
    tm.join();
    ts.join();
    tnl.join();
    
    return 0;
}

/*
 * Thread function TC: prints character c n times, where c is +, - or * depending on the thread
 */
static void TC (ThreadData td) {
    int nc = 0;
    while (1) {
        switch (td.c) {
            case '+':
                sp.acquire();
                cout << td.c;
                nc ++;
                if (nc < td.n) {
                    sp.release();
                } else {
                    nc = 0;
                    snl.release(); 
                }
                break;
            case '-':
                sm.acquire();
                cout << td.c;
                nc ++;
                if (nc < td.n) {
                    sm.release();
                } else {
                    nc = 0;
                    snl.release(); 
                }
                break;
            case '*':
                ss.acquire();
                cout << td.c;
                nc ++;
                if (nc < td.n) {
                    ss.release();
                } else {
                    nc = 0;
                    snl.release(); 
                }
                break;
            default:
                return;
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


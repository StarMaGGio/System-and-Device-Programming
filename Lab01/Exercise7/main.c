#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// struct to pass data to child threads
typedef struct {
    int level;      // Current level in the tree
    int target_n;   // Max hight n
    pthread_t *path; // Array with TID of predecessors
} thread_data_t;

void *thread_func(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    // Add self TID to the path
    pthread_t my_tid = pthread_self();
    data->path[data->level] = my_tid;

    // Check: am I a leaf?
    if (data->level == data->target_n -1) {
        // Print complete path from root to this leaf
        for (int i = 0; i <= data->level; i++) {
            printf("%lu-", (unsigned long)data->path[i]);
        }
        printf("\n");
        free(data->path);
        free(data);
        return NULL;
    }
    
    // If not a leaf -> generate two children
    pthread_t t1, t2;
    int i;
    for (i = 0; i < 2; i++) {
        thread_data_t *child_data = malloc(sizeof(thread_data_t));
        child_data->level = data->level + 1;
        child_data->target_n = data->target_n;

        // Alloc new array fot the child and copy current path
        child_data->path = malloc(data->target_n * sizeof(pthread_t));
        int j;
        for (j = 0; j <= data->level; j++) {
            child_data->path[j] = data->path[j];
        }

        if (i==0) pthread_create(&t1, NULL, thread_func, child_data);
        else pthread_create(&t2, NULL, thread_func, child_data);
    }

    // Wait the end of children
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    free(data->path);
    free(data);
    return NULL;
}

int main (int argc, char *argv[])
{
    int n = atoi(argv[1]);
    if (n <= 0) return 0;

    // The main thread, act as a "virtual" root
    thread_data_t *root_data = malloc(sizeof(thread_data_t));
    root_data->target_n = n;
    root_data->path = malloc(n * sizeof(pthread_t));

    // Generate first two threads
    pthread_t t1, t2;

    // Prepare separated data for these two initial branches
    thread_data_t *d1 = malloc(sizeof(thread_data_t));
    thread_data_t *d2 = malloc(sizeof(thread_data_t));
    d1->level = 0; d1->target_n = n; d1->path = malloc(n * sizeof(pthread_t));
    d2->level = 0; d2->target_n = n; d2->path = malloc(n * sizeof(pthread_t));

    pthread_create(&t1, NULL, thread_func, d1);
    pthread_create(&t2, NULL, thread_func, d2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    free(root_data->path);
    free(root_data);

    return 0;
}
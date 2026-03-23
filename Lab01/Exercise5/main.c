#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void generate_tree(int h, int n) {
    if (h == 0) {
        printf("PID: %d\n", getpid());
        return;
    }

    int i;
    for (i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            generate_tree(h-1, n);
            exit(0);
        }
    }

    exit(0);
}

int main(int argc, char *argv[]) {
    int h, n;

    printf("Enter tree height (h): ");
    scanf("%d", &h);

    printf("Enter tree degree (n): ");
    scanf("%d", &n);

    generate_tree(h, n);

    return 0;
}
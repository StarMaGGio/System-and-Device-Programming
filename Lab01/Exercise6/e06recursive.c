#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void binary_concurrent(int *, int, int);

int main (int argc, char *argv[])
{
  int n;
  int *vet;

  n = atoi(argv[1]);

  vet = (int *)malloc(n * sizeof(int));
  if (vet == NULL) {
    printf("Allocatin Error.\n");
    exit(1);
  }

  printf("Binary Numbers:\n");
  binary_concurrent(vet, n, 0);
  free(vet);

  return 0;
}

void binary_concurrent(int *vet, int n, int i)
{
  int j;

  if (i >= n) {
    for (j=0; j<n; j++) {
      printf("%d", vet[j]);
    }
    printf("\n");
    return;
  }

  // Generate a child to handle bit "0"
  pid_t pid = fork();

  if (pid == 0) {
    // --- CHILD PROCESS ---
    // Manage the branch with the 0
    vet[i] = 0;
    binary_concurrent(vet, n, i+1);
    exit(0);
  } else {
    // --- FATHER PROCESS ---
    // Manage the branch with the 1
    vet[i] = 1;
    binary_concurrent(vet, n, i+1);
  }
  
  return;
}




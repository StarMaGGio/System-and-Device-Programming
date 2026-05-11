#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char ** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input1> [<input2> ...] <output>\n", argv[0]);
        return 1;
    }

    int n = argc - 2;
    char *output_filename = argv[argc - 1];
}

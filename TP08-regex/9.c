#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Err : Arg manquant.\n");
        return 1;
    }
    int n = atoi(argv[1]);
    printf("%d\n", 2 * n);
    return 0;
}

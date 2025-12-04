#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    FILE* fptr = fopen(argv[1], "r");
    char chaine[10000];
    int count = 0;
    while (fscanf(fptr, "%[^\n] ", chaine) != EOF) count++;
    fclose(fptr);
    printf("%d\n", count);
    return 0;
}

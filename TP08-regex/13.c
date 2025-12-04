#include <stdio.h>
#include <string.h>

int main(void) {
    FILE* fptr = fopen("moby_dick.txt", "r");
    char ligne[10000];
    int num = 1;
    while (fscanf(fptr, "%[^\n]\n", ligne) != EOF) {
        if (strstr(ligne, "wallow") != NULL) {
            printf("%d : %s\n", num, ligne);
            break;
        }
        num++;
    }
    fclose(fptr);
    return 0;
}

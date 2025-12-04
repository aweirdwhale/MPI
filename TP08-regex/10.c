#include <stdio.h>
#include <stdlib.h>

void ecrit(const char* fichier, const char* message) {
    FILE* fptr = fopen(fichier, "w");
    fprintf(fptr, "%s", message);
    fclose(fptr);
}

int main(void) {
    ecrit("test.txt", "Hello World!\n");
    return 0;
}

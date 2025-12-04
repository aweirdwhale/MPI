#include <stdio.h>
#include <stdlib.h>

void lit_premiere_ligne(const char* fichier) {
    FILE* fptr = fopen(fichier, "r");
    char chaine[10000] = "";
//    fscanf(fptr, "%[^\n]", chaine);

    while (fscanf(fptr, "%[^\n] ", chaine) != EOF) {
 	printf("%s\n", chaine);
    } 
    fclose(fptr);
}

int main(void) {
    lit_premiere_ligne("message.txt");
    return 0;
}

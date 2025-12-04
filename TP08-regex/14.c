#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool chercheRK(const char* fichier, const char* mot) {
    FILE* fptr = fopen(fichier, "r");
    if (!fptr) return false;

    int m = strlen(mot);
    int hmot = 0;
    for (int i = 0; i < m; i++) hmot += mot[i];

    char ligne[10000];
    while (fscanf(fptr, "%[^\n] ", ligne) != EOF) {
        int n = strlen(ligne);
        if (n < m) continue;
        int h = 0;
        for (int i = 0; i < m; i++) h += ligne[i];
        if (h == hmot && strncmp(ligne, mot, m) == 0) { fclose(fptr); return true; }
        for (int i = m; i < n; i++) {
            h = h - ligne[i - m] + ligne[i];
            if (h == hmot && strncmp(ligne + i - m + 1, mot, m) == 0) {
                fclose(fptr);
                return true;
            }
        }
    }
    fclose(fptr);
    return false;
}

int main(void) {
    printf("%d\n", chercheRK("moby_dick.txt", "whale"));
    printf("%d\n", chercheRK("moby_dick.txt", "dolphin"));
    printf("%d\n", chercheRK("moby_dick.txt", "goldfish"));
    printf("%d\n", chercheRK("moby_dick.txt", "jellyfish"));
    printf("%d\n", chercheRK("moby_dick.txt", "shark"));
    printf("%d\n", chercheRK("moby_dick.txt", "sea horse"));
    printf("%d\n", chercheRK("moby_dick.txt", "dick"));
    return 0;
}

void affiche_plateau(bool** p, int n) {
    for (int i=0; i < n; i+=1) {
        printf(" ");
        for (int j=0; j < n; j+=1) {
            if (p[i][j]) printf("♛ ");
            else printf(". ");
        }
        printf("\n");
    }
}

bool verifie_derniere(int n, int sol[], int k) {
    for (int i = 0; i < k; i++) {
        if (sol[i] == sol[k] || abs(sol[i] - sol[k]) == abs(i - k))
            return false;
    }
    return true;
}
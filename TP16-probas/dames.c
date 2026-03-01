#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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


void affiche_solution(int sol[], int n) {
    //on crée un tableau de booleens d'après sol[] tel que table[ligne][sol[ligne]] = true et le reste est faux:
    bool** table = malloc(n * sizeof(bool*));
    for (int i = 0; i < n; i++) {
        table[i] = malloc(n * sizeof(bool));
        for (int j = 0; j < n; j++) {
            table[i][j] = false;
        }
        if (sol[i] >= 0 && sol[i] < n) {
            table[i][sol[i]] = true;
        }
    }

    // puis on affiche
    affiche_plateau(table, n);
    // et on n'oublie pas de nettoyer
    for (int i = 0; i < n; i++) {
        free(table[i]);
    }
    free(table);
}


int alea(int n, int sol[], int k) {
    int possibles[n];
    int count = 0;
    for (int col = 0; col < n; col++) {
        bool conflict = false;
        for (int i = 0; i < k; i++) {
            if (sol[i] == col || sol[i] - i == col - k || sol[i] + i == col + k) {
                conflict = true;
                break;
            }
        }
        if (!conflict) {
            possibles[count++] = col;
        }
    }
    if (count == 0) return -1;
    int idx = rand() % count;
    return possibles[idx];
}

bool descente(int n, int sol[]) {
    for (int i = 0; i < n; i++) {
        int col = alea(n, sol, i);
        if (col == -1) {
            return false;
        }
        sol[i] = col;
    }
    return true;
}

void sol_n_dames(int n, int sol[], int* essais) {
    *essais = 0;
    bool trouve = false;
    while (!trouve) {
        for (int i = 0; i < n; i++) {
            sol[i] = -1;
        }
        (*essais)++;
        trouve = descente(n, sol);
    }
}

double moyenne(int n, int sol[], int nb_executions){
    int total_essais = 0;
    int essais;
    for (int i = 0; i < nb_executions; i++) {
        sol_n_dames(n, sol, &essais);
        total_essais += essais;
    }
    return (double)total_essais / nb_executions;
}

int main() {
    int S[] = {2, 0, 3, 1};
    int n = 4;
    affiche_solution(S, n);

    // Test pour alea
    int sol_test[4] = {2, 0, 3, -1};
    int col = alea(4, sol_test, 3);
    printf("Colonne possible pour la 4e reine : %d\n", col); // on expect 1

    // Test pour descente
    int sol_descente[4] = {-1, -1, -1, -1};
    bool res = descente(4, sol_descente);
    printf("descente retourne : %s\n", res ? "true" : "false");
    if (res) {
        affiche_solution(sol_descente, 4);
    }


    //test pour sol_n_dames
    int essais = 0;
    int sol_final[8];
    sol_n_dames(8, sol_final, &essais);
    printf("Solution pour 8 dames trouvée en %d essais :\n", essais);
    affiche_solution(sol_final, 8);

    return 0;
}

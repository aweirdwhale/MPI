#include "freivalds.h"
#include <assert.h>
#include <stdio.h>


void printmatrice(int** A, int n, int m) {
    if (A == NULL || n <= 0 || m <= 0) {
        printf("Matrice invalide\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

int** mult(int** A, int n, int pA, int** B, int pB, int q) {

    // printmatrice(A, n, pA);
    // printmatrice(B, q, pB);
    // printf("%d", n);
    // printf("%d", pA);
    // printf("%d", q);
    // printf("%d", pB);

    // Vérification défensive des dimensions
    if (A == NULL || B == NULL || n <= 0 || pA <= 0 || pB <= 0 || q <= 0) {
        return NULL;
    }

    // if (pA != pB) {
    //     // Les dimensions ne sont pas compatibles pour la multiplication
    //     return NULL;
    // }
    assert(pA == pB);

    // Allocation de la matrice résultat (n x q)
    int** res = malloc(n * sizeof(int*));
    if (res == NULL) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        res[i] = malloc(q * sizeof(int));
        if (res[i] == NULL) {
            // Libérer les lignes déjà allouées en cas d'échec
            for (int k = 0; k < i; k++) {
                free(res[k]);
            }
            free(res);
            return NULL;
        }
        // Initialiser à zéro
        for (int j = 0; j < q; j++) {
            res[i][j] = 0;
        }
    }

    // Multiplication naïve
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < q; j++) {
            for (int k = 0; k < pA; k++) {
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return res;
}


void free_matrice(int** A, int p) {
    if (A == NULL || p <= 0) {
        return;
    }
    for (int i = 0; i < p; i++) {
        free(A[i]);
    }
    free(A);
}


bool egales(int**A, int**B, int n) {
    if (A == NULL || B == NULL || n <= 0) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] != B[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool verifie_mult_naif(int** A, int** B, int** C, int n) {
    if (A == NULL || B == NULL || C == NULL || n <= 0) {
        return false;
    }

    // Calculer AB
    int** AB = mult(A, n, n, B, n, n);
    if (AB == NULL) {
        return false;
    }

    // Comparer AB et C
    bool resultat = egales(AB, C, n);

    // Libérer la mémoire allouée pour AB
    free_matrice(AB, n);

    return resultat;
}



bool freivalds(int** A, int** B, int** C, int n) {
    if (A == NULL || B == NULL || C == NULL || n <= 0) {
        return false;
    }

    // Générer un vecteur aléatoire X de taille n, avec des valeurs 0 ou 1
    int* X = malloc(n * sizeof(int));
    if (X == NULL) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        X[i] = rand() % 2;
    }

    // Calculer BX = B * X (vecteur de taille n)
    int* BX = malloc(n * sizeof(int));
    if (BX == NULL) {
        free(X);
        return false;
    }
    for (int i = 0; i < n; i++) {
        BX[i] = 0;
        for (int j = 0; j < n; j++) {
            BX[i] += B[i][j] * X[j];
        }
    }

    // Calculer ABX = A * (B * X) (vecteur de taille n)
    int* ABX = malloc(n * sizeof(int));
    if (ABX == NULL) {
        free(X);
        free(BX);
        return false;
    }
    for (int i = 0; i < n; i++) {
        ABX[i] = 0;
        for (int j = 0; j < n; j++) {
            ABX[i] += A[i][j] * BX[j];
        }
    }

    // Calculer CX = C * X (vecteur de taille n)
    int* CX = malloc(n * sizeof(int));
    if (CX == NULL) {
        free(X);
        free(BX);
        free(ABX);
        return false;
    }
    for (int i = 0; i < n; i++) {
        CX[i] = 0;
        for (int j = 0; j < n; j++) {
            CX[i] += C[i][j] * X[j];
        }
    }

    // Vérifier si ABX == CX
    bool ok = true;
    for (int i = 0; i < n; i++) {
        if (ABX[i] != CX[i]) {
            ok = false;
            break;
        }
    }

    free(X);
    free(BX);
    free(ABX);
    free(CX);

    return ok;
}


int main(void) {

    //TODO : VOS PROPRES TESTS !!!!!!!!!!

    return EXIT_SUCCESS;
}

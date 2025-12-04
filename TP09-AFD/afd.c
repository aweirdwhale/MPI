#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "afdio.h"
#include "afd.h"



etat transition(afd a, etat q, lettre l) {
    if (q < 0 || q >= a.n) return -1;
    if (l < 0 || l >= a.m) return -1;
    return a.delta[q][l];
}


void acceptants(afd a) {
    bool first = true;
    for (int q = 0; q < a.n; q++) {
        if (a.term[q]) {
            if (!first) printf(" ");
            printf("%d", q);
            first = false;
        }
    }
    printf("\n");
}


void free_afd(afd a) {
    if (a.term) free(a.term);
    if (a.delta) {
        for (int i = 0; i < a.n; i++) {
            if (a.delta[i]) free(a.delta[i]);
        }
        free(a.delta);
    }
}


etat delta_etoile(afd a, etat q, mot u, int longueur) {
    etat cur = q;
    for (int i = 0; i < longueur; i++) {
        if (cur < 0) return -1;
        lettre l = u[i];
        if (l < 0 || l >= a.m) return -1;
        cur = transition(a, cur, l);
        if (cur < 0) return -1;
    }
    return cur;
}


bool accepte(afd a, mot u, int longueur) {
    etat qf = delta_etoile(a, a.init, u, longueur);
    if (qf < 0) return false;
    return a.term[qf];
}


etat** copie_matrice(etat** mat, int n, int m) {
    if (!mat) return NULL;
    etat** copie = malloc(n * sizeof(etat*));
    if (!copie) { printf("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; i++) {
        copie[i] = malloc(m * sizeof(etat));
        if (!copie[i]) { printf("malloc"); exit(EXIT_FAILURE); }
        memcpy(copie[i], mat[i], m * sizeof(etat)); // hp ? en tout cas très pratique : copie m * sizeof(etat) bits de mat[i] à copie[i] (string.h)
    }
    return copie;
}


afd complementaire(afd a) {
    afd res;
    res.m = a.m;
    res.n = a.n;
    res.init = a.init;
    /* copie inv des terminaux */
    res.term = malloc(res.n * sizeof(bool));
    if (!res.term) { printf("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < res.n; i++) res.term[i] = !a.term[i];
    /* copie profonde des transitions */
    res.delta = copie_matrice(a.delta, a.n, a.m);
    return res;
}


afd complete(afd a) {
    int m = a.m;
    int n_old = a.n;
    int n_new = n_old + 1; /* indice du puits = n_old */
    etat sink = n_old;

    afd res;
    res.m = m;
    res.n = n_new;
    res.init = a.init;

    /* termes */
    res.term = malloc(n_new * sizeof(bool));
    if (!res.term) { printf("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n_old; i++) res.term[i] = a.term[i];
    res.term[sink] = false; /* puits nn acc par défaut */

    /* trans */
    res.delta = malloc(n_new * sizeof(etat*));
    if (!res.delta) { printf("malloc"); exit(EXIT_FAILURE); }
    for (int q = 0; q < n_old; q++) {
        res.delta[q] = malloc(m * sizeof(etat));
        if (!res.delta[q]) { printf("malloc"); exit(EXIT_FAILURE); }
        for (int l = 0; l < m; l++) {
            int dest = a.delta[q][l];
            if (dest < 0) res.delta[q][l] = sink;
            else res.delta[q][l] = dest;
        }
    }
    /* puits -> puits */
    res.delta[sink] = malloc(m * sizeof(etat));
    if (!res.delta[sink]) { printf("malloc"); exit(EXIT_FAILURE); }
    for (int l = 0; l < m; l++) res.delta[sink][l] = sink;

    return res;
}


bool** inverse(afd a) {
    int n = a.n;
    bool** adj = malloc(n * sizeof(bool*));
    for (int i = 0; i < n; i++) {
        adj[i] = malloc(n * sizeof(bool));
        for (int j = 0; j < n; j++) adj[i][j] = false;
    }
    for (int q = 0; q < n; q++) {
        for (int l = 0; l < a.m; l++) {
            int q2 = a.delta[q][l];
            if (q2 >= 0 && q2 < n && q2 != q) {
                /* arc q->q2 in G(A) => dans in on met q2->q */
                adj[q2][q] = true;
            }
        }
    }
    return adj;
}


static void dfs_accessible(afd a, int q, bool* visited) {
    if (visited[q]) return;
    visited[q] = true;
    for (int l = 0; l < a.m; l++) {
        int q2 = a.delta[q][l];
        if (q2 >= 0 && !visited[q2]) dfs_accessible(a, q2, visited);
    }
}

bool* accessibles(afd a) {
    bool* visited = malloc(a.n * sizeof(bool));
    for (int i = 0; i < a.n; i++) visited[i] = false;
    if (a.init >= 0 && a.init < a.n) dfs_accessible(a, a.init, visited);
    return visited;
}


bool langage_non_vide(afd a) {
    return EXIT_FAILURE;
}


bool* coaccessibles(afd a){
    return EXIT_FAILURE;
}

bool est_emonde(afd a) {
    return EXIT_FAILURE;
}

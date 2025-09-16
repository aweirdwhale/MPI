#include "graphes.h"
#include "dynarray.h"
#include "file.h"
#include "pile.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* variable globale compteur */
//int compteur_dfs_rec = 0;

/* Première section */

graphe cree_graphe_exemple(void) {

  int n = 14;
  dynarray* lA = malloc(n * sizeof(dynarray));

  // Ne perdez pas de temps entre cette ligne et la ligne 29
  int deg[14] = {2, 2, 2, 3, 0, 2, 0, 3, 0, 1, 0, 1, 1, 1};
  int l[14][14] = {{1, 6},
                   {2, 3},
                   {4, 5},
                   {2, 5, 9},
                   {},
                   {9, 12},
                   {},
                   {6, 8, 11},
                   {},
                   {10},
                   {},
                   {10},
                   {13},
                   {11} };

  for (int s = 0; s < n; s += 1) {
    lA[s] = dynarray_of_array(deg[s], l[s]);
  } // À partir d'ici lA contient les listes d'adja

  graphe G = {lA, n};
  return G;

}

void graphe_free(graphe G) {
    for (int i = 0; i<G.n; i++) {
        dyn_free(G.lA[i]);

    }
}

graphe graphe_transpose(graphe G) {
    //créer un nouveau graphe
    int n = G.n;
    dynarray* lA_transpose = malloc(n * sizeof(dynarray));

    // init array vide pr les nouv. arretes
    for (int i = 0; i < n; i++) {
        lA_transpose[i] = dynarray_of_array(0, 0);
    }

      // pour chaque arrete et ses adjacents on ajoute le contraire
    for (int i = 0; i < n; i++) {
    dynarray adj_list = G.lA[i];
        for (int j = 0; j < dyn_len(adj_list); j++) {
            int neighbor = dyn_acces(adj_list, j);
            dyn_ajoute(&lA_transpose[neighbor], i);
        }
    }

    graphe G_transpose = {lA_transpose, n};
    return G_transpose;
}

void graphe_print(graphe G) {
  printf("---\n");
  for (int i = 0; i < G.n; i += 1) {
    printf("%d vers : ", i);
    dynarray a = G.lA[i];
    for (int j = 0; j < dyn_len(a); j += 1) {
      printf("%d ", dyn_acces(a, j));
    }
    printf("\n");
  }
  printf("---\n");
  return;
}


/* UTILITAIRES */

// crée et initialise un tableau de marquages
bool* cree_marquage(int size) {
    bool *tab = (bool*)malloc(size * sizeof(bool));

    for (int i = 0; i<size; i++) {
        tab[i] = false;
    }

    return tab;
}


/* Seconde section : BFS */

void graphe_bfs_s0(graphe G, int s0, int ordre[]) {


    // crée la file d'attente et le tableau de marquage
    file* F = file_vide(G.n + 10);
    bool* marques = cree_marquage(G.n);

    // ajoute s0 à la file
    file_add(F, s0);

    int count = 0;

    // tant que la file n'est pas vide
    while(!file_est_vide(F)) {
        int s = file_take(F);
        if (!marques[s]) {

            // il faudra sort mais pour l'instant on add juste les voisins de s à la file
            for (int i = 0; i<dyn_len(G.lA[s]); i++) { // itère sur tout les voisins de s
                file_add(F, dyn_acces(G.lA[s], i));
            }

            // ici on process les sommets parcourus
            ordre[count] = s;
            count++;

            // marquer le sommet
            marques[s] = true;
        }
    }

    file_free(F);

    // remplir ordre de -1
    for (int i = count; i<G.n; i++) {
        ordre[i] = -1;
    }


    // debug
    // for (int i = 0; i<G.n; i++) {
    //     printf("%d ", ordre[i]);
    // }

    free(marques);

}

void graphe_bfs_complet(graphe G, int ordre[]) {
    bool* marques = cree_marquage(G.n);
    int count = 0;

    // parcourir tous les sommets
    for (int s0 = 0; s0 < G.n; s0++) {
        if (!marques[s0]) {

            // BFS à partir de s0
            file* F = file_vide(G.n);
            file_add(F, s0);

            while(!file_est_vide(F)) {
                int s = file_take(F);
                if (!marques[s]) {

                    // ajouter les voisins non marqués à la file
                    for (int i = 0; i < dyn_len(G.lA[s]); i++) {
                        int voisin = dyn_acces(G.lA[s], i);
                        if (!marques[voisin] && !file_mem(voisin, F)) {
                            file_add(F, voisin);
                        }
                    }

                // traiter le sommet courant
                ordre[count] = s;
                count++;
                marques[s] = true;
                }
            }
            file_free(F);


        }

    }

    free(marques);

    // debug
    // for (int i = 0; i<G.n; i++) {
    //     printf("%d ", ordre[i]);
    // }

}


void graphe_dist_s0(graphe G, int s0, int dist[]) {
    // Init les distances à -1 (sommets non accessible)
    for (int i = 0; i < G.n; i++) {
        dist[i] = -1;
    }

    // s0 -> s0
    dist[s0] = 0;

    // BFS pour calculer les distances
    file* F = file_vide(G.n);
    file_add(F, s0);

    while (!file_est_vide(F)) {
        int s = file_take(F);

        // Parcourir tous les voisins de s
        for (int i = 0; i < dyn_len(G.lA[s]); i++) {
            int voisin = dyn_acces(G.lA[s], i);

            // Si le voisin n'a pas encore été visité
            if (dist[voisin] == -1 && !file_mem(voisin, F)) {
                dist[voisin] = dist[s] + 1;
                file_add(F, voisin);
            }
        }
    }

    file_free(F);
}



void graphe_dfs_s0_pile(graphe G, int s0, int *ordre) {
    cellule *pile = pile_create(s0); // on crée une pile avec s0 dans la premiere cell
    bool *marques = cree_marquage(G.n);

    int count = 0;

    while(!pile_est_vide(pile)) {
        // depile le premier item
        int s = pile_hd(pile);
        pile = pile_pop(pile);

        if (!marques[s]) {
            marques[s] = true;
            ordre[count] = s;
            count ++;

            // ajouter tout les voisins de s à la pile (en ordre inverse pour préserver l'ordre DFS)
            for (int i = dyn_len(G.lA[s]) - 1; i >= 0; i--) {
                int voisin = dyn_acces(G.lA[s], i);
                if (!marques[voisin]) {
                    pile = pile_cons(pile, voisin);
                }
            }
        }
    }

    pile_free(pile);

    // on ne remplis plus ? si avec nouveau testeur
    // remplir ordre de -1
    for (int i = count; i<G.n; i++) {
        ordre[i] = -1;
    }

    free(marques);
}


void auxi(graphe G, int s, bool* marque, int* ouverture, int* fermeture, int* count) {
    marque[s] = true;
    ouverture[s] = *count;
    *count +=1;

    for (int i = 0; i < dyn_len(G.lA[s]); i++) {
        int voisin = dyn_acces(G.lA[s], i);
        if (!marque[voisin]) {
            auxi(G, voisin, marque, ouverture, fermeture, count);
        }
    }

    fermeture[s] = *count;
    *count +=1;
}

void graphe_dfs_s0_rec(graphe G, int s0, int ouverture[], int fermeture[]) {
    bool* marques = cree_marquage(G.n);
    // première idée était de faire une var globale
    // problématique quand il faut faire plusieurs appels de graphe_dfs_s0_rec -> jamais remise à 0
    // solution : fonction auxiliaire
    int count = 0;
    auxi(G, s0, marques, ouverture, fermeture, &count);
    free(marques);
}

void graphe_dfs_rec(graphe G, int ouverture[], int fermeture[]) {
    bool* marques = cree_marquage(G.n);

    int count = 0;

    for (int s0 = 0; s0 < G.n; s0++) { // appel sur tout les sommets
        if (!marques[s0]) {
            auxi(G, s0, marques, ouverture, fermeture, &count);
        }
    }

    free(marques);
}


void graphe_tri_topo(graphe G, int tri_topo[]) {
    int* ouverture = malloc(G.n * sizeof(int));
    int* fermeture = malloc(G.n * sizeof(int));

    // DFS complet pour temps de fermeture
    graphe_dfs_rec(G, ouverture, fermeture);

    // Créer un arr de paires
    typedef struct {
        int sommet;
        int temps_fermeture;
    } sommet_temps;

    sommet_temps* paires = malloc(G.n * sizeof(sommet_temps));

    for (int i = 0; i < G.n; i++) {
        paires[i].sommet = i;
        paires[i].temps_fermeture = fermeture[i];
    }

    // Trier par temps de fermeture dec
    for (int i = 0; i < G.n - 1; i++) {
        for (int j = 0; j < G.n - 1 - i; j++) {
            if (paires[j].temps_fermeture < paires[j + 1].temps_fermeture) {
                sommet_temps temp = paires[j];
                paires[j] = paires[j + 1];
                paires[j + 1] = temp;
            }
        }
    }

    // Remplir le tableau
    for (int i = 0; i < G.n; i++) {
        tri_topo[i] = paires[i].sommet;
    }

    free(ouverture);
    free(fermeture);
    free(paires);
}

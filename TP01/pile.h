/** Piles simplement chainées */
#ifndef _H_PILE_H_
#define _H_PILE_H_ 0

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct cellule_s {
  int elem;
  struct cellule_s* next;
};
typedef struct cellule_s cellule;

/* Initialisateurs */

/** Crée une pile contenant uniquement l'élement x */
cellule* pile_create(int x);

/** Désalloue une cellule de type cellule_s */
void cellule_free(cellule* p);

/** Désalloue successivement toute une pile,
  * représentée par un pointeur vers sa tête */
void pile_free(cellule* p);


/* Transformateurs */

/** Ajoute un élément en tête de pile
  * et renvoie un pointeur vers cette nouvelle tête */
cellule* pile_cons(cellule* p, int x);

/** Supprime et libère l'élément de tête de pile.
  * Renvoie un pointeur vers la nouvelle tête.
  * /!\ l'ancienne tête est donc effacée */
cellule* pile_pop(cellule* p);

/** Convertit une pile en tableau */
int* tableau_of_pile(const cellule* p, size_t n);

/** Convertit un tableau en pile */
cellule* pile_of_tableau(int* T, int n);

/** Renvoie la pile p triée (tri par insertion) */
cellule* pile_sort(cellule* p);


/* Accesseurs */

/** Teste si une pile est vide */
bool pile_est_vide(const cellule* p);

/** Renvoie l'élément de tête de pile */
int pile_hd (const cellule* p);

/** Renvoie un pointeur vers la queue d'une pile */
cellule* pile_tl(const cellule* p);

/** Renvoie le nombre d'éléments d'une pile */
size_t pile_size(const cellule* p);
size_t pile_size_rec(const cellule* p);

/** Teste l'appartenance d'un élément à une pile */
bool pile_mem(int x, const cellule* p);

/** Affiche récursivement tout une pile */
void print_pile(const cellule* p);

#endif
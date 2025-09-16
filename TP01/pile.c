/** Implémentation de piles comme listes simplement chainées */

#include "pile.h"


/* Initialisateurs */

cellule* pile_create(int x) {
  cellule* p = (cellule*) malloc(sizeof(cellule));
  p->elem = x;
  p->next = NULL;
  return p;
}

void cellule_free(cellule* p) {
  free(p);
}

void pile_free(cellule* p) {
  if (p != NULL) {
    pile_free(p->next);
    free(p);
  }
  return;
}


/* Transformateurs */

cellule* pile_cons(cellule* p, int x) {
  // Il suffit de créer un nouveau maillon
  // et de le chainer à l'ancien premier maillon
  cellule* p_new = pile_create(x);
  p_new->next = p;
  return p_new;
}

cellule* pile_pop(cellule* p) {
  cellule* p_new = p->next;
  cellule_free(p);
  return p_new;
}

int* tableau_of_pile(const cellule* p, size_t n) {
  int* T = (int*) malloc(n * sizeof(int));
  for (size_t i = 0; i < n; i += 1) {
    T[i] = p->elem;
    p = p->next;
  }
  return T;
}

cellule* pile_of_tableau(int* T, int n) {
  if (n==0) return NULL;
  cellule* c = pile_create(T[n-1]);
  for (int i=n-2; i >= 0; i-=1) {
    cellule* p = pile_create(T[i]);
    p->next = c;
    c = p;
  }
  return c;
}

void insert(int x, cellule** p_ptr) {
  cellule* c = pile_create(x);
  if (*p_ptr == NULL) { //cas p = []
    *p_ptr = c;
    return;
  }
  if ((**p_ptr).elem >= x) { //cas x < tête de p
    (*c).next = *p_ptr;
    *p_ptr = c;
    return;
  }
  //sinon
  cellule* previous = *p_ptr;
  while (previous->next != NULL && previous->next->elem < x) {
    previous = previous->next;
  }
  c->next = previous->next;
  previous->next = c;
  return;
}

cellule* pile_sort(cellule* p) { //tri par insertion
  cellule* p_trie = NULL;
  while (p != NULL) {
    insert(p->elem, &p_trie);
    p = p->next;
  }
  return p_trie;
}


/* Accesseurs */

bool pile_est_vide(const cellule* p) {
  return p == NULL;
}

int pile_hd (const cellule* p) {
  return p->elem;
}

cellule* pile_tl(const cellule* p) {
  return p->next;
}

size_t pile_size(const cellule* p) {
  // il suffit de compter les flèches
  // i.e. les ptrs non-NULL
  size_t s = 0;
  while (p != NULL) {
    s += 1;
    p = p->next;
  } // Invariant : s = nb de flèches traversées
  return s;
}

/* Bonus : version récursive*/
size_t pile_size_rec(const cellule* p) {
  if (p == NULL) { return 0; }
  return 1 + pile_size_rec(p->next);
  // preuve rec : pile_size_rec renvoie le nb d'elem de la
  // liste pointée par p
}

bool pile_mem(int x, const cellule* p) {
  bool res = false;
  while (!res && p != NULL) {
    res = (p->elem == x);
    p = p->next;
  }
  return res;
}

void print_pile(const cellule* p)  {
  printf("[");
  if (p == NULL) {
    printf("]\n");
    return;
  }
  while (p->next != NULL) {
    printf("%d; ", p->elem);
    p = p->next;
  } // "Invariant" : s = nb de flèches traversées
  printf("%d]\n", p->elem);
  return;
}
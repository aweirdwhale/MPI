#include "dynarray.h"
#include <stdlib.h>



/* Création, conversion, suppression */

int puissance_2(int n) {
  int res = 1;
  while (res < n) {
    res = 2*res;
  }
  return res;
}


dynarray dyn_create(int len, int x) {
  // 1: on crée le tableau sous-jacent
  int n = puissance_2(len);
  int* tab = malloc(n*sizeof(int));
  for (int i = 0; i<len; i += 1) {
    tab[i] = x;
  }
  // 2 : on crée le dynarray
  dynarray a = {.tab = tab, .len = len, .n = n};
  return a;
}


void dyn_free(dynarray a) {
  free(a.tab);
  return;
}


dynarray dynarray_of_array(int len, int T[]) {
  dynarray a = dyn_create(len, 0);
  for (int i = 0; i < len; i = i+1) {
    a.tab[i] = T[i];
  }
  return a;
}


int* array_of_dynarray(dynarray* a) {
  int* T = (int*) malloc(a->len * sizeof(int));
  for (int i = 0; i < a->len; i = i+1) {
    T[i] = a->tab[i];
  }
  return T;
}



/* Accès et modifications */

int dyn_acces(dynarray a, int i) {
  return a.tab[i]; // ou de manière strictement identique
                      // return a->tab[i];
}


void dyn_remplace(dynarray* a, int i, int x) {
  (*a).tab[i] = x; // ou de manière str id,
                   // a->tab[i] = x;
  return;
}


void dyn_ajoute(dynarray* a, int x){
  if ((*a).len < (*a).n) {
    (*a).tab[(*a).len] = x; // on aurait aussi pu écrire
                            // a->tab[a->len] = x;
    a->len = a->len + 1;
  }
  else {
    // 1 on crée le nouveau tableau
    int* new_tab = malloc((2*a->n)*sizeof(int));
    for (int i=0; i < a->len; i = i+1){
      new_tab[i] = a->tab[i];
    }
    new_tab[a->len] = x;

    // 2 on maj le dynarray *a en csq
    // sans oublier de free l'ancien tableau
    free(a->tab);
    a->tab = new_tab;
    a->len += 1;
    a->n = 2 * a->n;
  }

  return;
}



void dyn_retire(dynarray* a){
  a->len = a->len - 1;
  if (a->len <= a->n /4) {
    // 1 on crée le nouveau tableau
    int* tab2 = malloc((a->n/2)*sizeof(int));
    for (int i=0; i < a->len; i = i+1) {
      tab2[i] = a->tab[i];
    }

    // 2 on maj, comme avant
    free(a->tab);
    a->tab = tab2;
    a->n = a->n/2;
  }

  return;
}


/* Miscelaneous */

int dyn_len(dynarray a) {
  return a.len;
}

void dyn_affiche(dynarray a) {
  for (int i = 0; i < a.len; i = i+1) {
    printf("%d", a.tab[i]);
    if (i+1 < a.len) {
      printf(" ");
    } else {
      printf("\n");
    }
  }
  return;
}

bool dyn_in(dynarray a, int x) {
  for (int i = 0; i < a.len; i += 1) {
    if (dyn_acces(a, i) == x) { return true; }
  }
  return false;
}
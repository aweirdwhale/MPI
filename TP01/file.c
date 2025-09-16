/* Implémentation de files par tableaux circulaires */

#include "file.h"



/* Initialisateurs : création, suppression */

file* file_vide(size_t capacite) {
  int* contenu = (int*) malloc(capacite * sizeof(int));
  file* f = (file *) malloc(sizeof(file));
  f->contenu = contenu;
  f->capacite = capacite;
  f->sortie = 0;
  f->entree = 0;
  f->est_vide = true;
  return f;
}

void file_free(file* f) {
  free(f->contenu);
  free(f);
  return;
}


/* Transformateurs : enfilage, defilage */

void file_add(file* f, int x) {
  file_enfile(f, x);
  return;
}

void file_enfile(file* f, int x) {
  assert(f->sortie != f->entree || f->est_vide); // sinon c'est plein
  f->contenu[f->entree] = x;
  f->entree = (f->entree + 1) % f->capacite;
  f->est_vide = false;
  return;
}

int file_take(file* f) {
  return file_defile(f);
}

int file_defile(file* f) {
  assert(!file_est_vide(f));
    // sinon il n'y a rien à défiler
  int x = f->contenu[f->sortie];
  f->sortie = (f->sortie + 1) % f->capacite;
  if (f->sortie == f->entree) {
    // on vérifie fu'on a pas vidé la file
    f->est_vide = true;
  }
  return x;
} // TODO : resize when too small.


/* Accesseurs */

bool file_est_vide(const file* f) {
  return f->est_vide;
}

int file_premier(const file* f) {
  assert(!file_est_vide(f));
  return (f->contenu[f->sortie]);
}

int file_len(const file* f) {
  return (f->entree - f->sortie + f->capacite) % f->capacite;
}

bool file_mem(int x, const file* f) {
  for (size_t i = f->sortie; i != f->entree; 
                             i = (i+1) % f->capacite ) {
    if (f->contenu[i]==x) {return true;}
  }
  return false;
}

void file_affiche(const file* f) {
  for (size_t i = f->sortie; i != f->entree; 
                             i = (i+1) % f->capacite ) {
    printf("%d", f->contenu[i]);
    if ( (i+1) % f->capacite != f->entree) {
      printf(" ");
    }
  }
  printf("\n");
  return;
}
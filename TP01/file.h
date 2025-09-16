/* Files de taille bornée */
#ifndef _H_FILE_H_
#define _H_FILE_H_ 0

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>


struct file_s {
  int* contenu; // le tableau circulaire
  size_t capacite; // le nb max d'elem de la file (aussi appelé N)
  size_t sortie; // l'indice du prochain élément à sortir (aka debut)
  size_t entree; // l'indice suivant le dernier élément à être entré (aka fin)
  bool est_vide; // true iff la file est vide
};
typedef struct file_s file;


/* Initialisateurs : création, suppression */

/** Crée une file vide de capacité maximale capacite */
file* file_vide(size_t capacite);

/** Supprime une file */
void file_free(file* f);


/* Transformateurs */

/** Enfilage d'un élément x dans la file pointée par q 
  * NB : add et enfile sont les mêmes, simplement deux langues différentes 
  */
void file_add(file* f, int x);
void file_enfile(file* f, int x);

/** Défilage dans la file pointée par q
  * Renvoie l'élément défilé 
  * NB : take et defile sont les mêmes, simplement deux langues différentes 
  */
int file_take(file* f);
int file_defile(file* f);


/* Accesseurs */

/** Teste si une file est vide */
bool file_est_vide(const file* f);

/** Renvoie le prochain élément à sortir de la file */
int file_premier(const file* f);

/** Renvoie le nb d'éléments d'une file */
int file_len(const file* f);

/** Teste l'appartenance d'un élément à une file */
bool file_mem(int x, const file* f);

/** Affiche les éléments d'une file dans l'ordre */
void file_affiche(const file* f);

#endif
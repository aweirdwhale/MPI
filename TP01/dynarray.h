/* Tableaux dynamiques */
#ifndef _H_DYNARRAY_H_
#define _H_DYNARRAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct dynarray_s {
 int* tab; // le tableau dynamique
 int n; // sa capacité maximale
 int len; // son nombre d'éléments
};
typedef struct dynarray_s dynarray;


/* Création, conversion, suppression */

/** Crée un tableau dynamique initialisé à len fois x.*/
dynarray dyn_create(int len, int x);

/** Supprime un tableau dynamique */
void dyn_free(dynarray a);

/* Accès et modifications */

/** Accès à l'élément d'indice i d'un tableau dynamique */
int dyn_acces(dynarray a, int i);

/** Remplace par x l'élément d'indice i d'un tableau dynamique */
void dyn_remplace(dynarray* a, int i, int x);

/** Ajoute un élément x dans un tableau dynamique a.*/
void dyn_ajoute(dynarray* a, int x);

/** Enlève un élément du tableau dynamique a (doit être non vide).*/
void dyn_retire(dynarray* a);


/* Divers */

/** Renvoie le nombre d'éléments d'un tableau dynamique */
int dyn_len(dynarray a);

/** Teste si x est dans a */
bool dyn_in(dynarray a, int x);

/** Affiche un tableau dynamique */
void dyn_affiche(dynarray a);


/** Crée un tableau dynamique qui contient les valeurs d'un tableau/zone allouée */
dynarray dynarray_of_array(int len, int T[]);

/** Crée une zone allouée contenant les valeurs d'un tableau dynamique */
int* array_of_dynarray(dynarray* a);

#endif

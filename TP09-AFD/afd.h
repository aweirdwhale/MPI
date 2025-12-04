#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "afdio.h"

//http://magjac.com/graphviz-visual-editor/


etat transition(afd a, etat q, lettre l);

void acceptants(afd a);

void free_afd(afd a);

/* Manipulation */


etat delta_etoile(afd a, etat q, mot u, int longueur);


bool accepte(afd a, mot u, int longueur);

int** copie_matrice(int** mat, int n, int m);

afd complementaire(afd a);

afd complete(afd a);


bool** inverse(afd a);

bool* accessibles(afd a);

bool langage_non_vide(afd a);

bool* coaccessibles(afd a);


bool est_emonde(afd a);

// Pour aller plus loin :

/*On construit l’automate produit avec un choix approprié des états acceptants. C’est un peu
long mais il n’y a pas de vraie difficulté : l’état (q, q') est numéroté q · nA2 + q' .*/
afd inter(afd a, afd b);


// Pour A, B deux parties d’un ensemble E, on a (A ⊂ B) ⇔ (A ∩ Bc = ∅). On en déduit :
bool inclus(afd a, afd b);

bool equivalent(afd a, afd b);



int test__eleve();
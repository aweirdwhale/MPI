#ifndef AFDIO
#define AFDIO 0

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define taille_max 10000


// Type des automates

typedef int lettre;
typedef lettre* mot; //un mot est un tableau de lettres, donc d'entiers
typedef int etat;

struct afd_s {
    int m; // taille de l'alphabet, Sigma = {0; 1; ...; m-1}
    int n; // nombre d'états de l'AFD, Q = {0; 1; ...; n-1}
    etat init; // état initial
    bool* term; // états terminaux (acceptants) : tableaux de booléns de taille n
    etat** delta; // transitions : matrice d'adjacence de taille n x m
};

typedef struct afd_s afd;

/*
m et n sont des entiers strictement positifs.
delta est une matrice de taille n x m telle que delta[q][x] contient l'état delta(q,x) s'il existe, et -1 sinon.
*/


afd afd_exemple_a1();

afd afd_exemple_a2();

afd afd_exemple_a3();

void graphviz(afd a, char* filename);

void genere_pdf(char* input_file, char* output_file);



int* copie_tableau(int* tab, int n);

bool* copie_tab_bool(bool* tab, int n);


#endif



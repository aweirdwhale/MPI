#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* Fonctions d'affichage */

void affiche_solution(int sol[], int n);

/* Résolution par algo probabiliste */

int alea(int n, int sol[], int k);

bool descente(int n, int sol[]);

void sol_n_dames(int n, int sol[], int* essais);

double moyenne(int n, int sol[], int nb_tentatives);

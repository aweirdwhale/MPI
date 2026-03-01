#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>


int** mult(int** A, int n, int pA, int** B, int pB, int q);

void free_matrice(int** A, int p);

bool egales(int**A, int**B, int n);

bool verifie_mult_naif(int** A, int** B, int**C, int n);

bool freivalds(int** A, int** B, int**C, int n);


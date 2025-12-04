#include "afdio.h"

etat* convertit_tableau_etat(etat tab[], int n) {
    etat* res = (etat*) malloc(n * sizeof(etat));
    for (int i=0; i<n; i+=1) {
        res[i] = tab[i];
    }
    return res;
}

/* Automates exemples : */


afd afd_exemple_a1() {
    int m = 2;
    int n = 3;
    etat init = 0;
    bool* term = (bool*) malloc(n*sizeof(bool));
    term[0] = true, term[1] = false, term[2] = false;
    etat** delta = (etat**) malloc(n*sizeof(etat*));
    int ligne0[2] = {1,0};
    delta[0] = convertit_tableau_etat(ligne0, 2);
    int ligne1[2] = {2,1};
    delta[1] = convertit_tableau_etat(ligne1, 2);
    int ligne2[2] = {0,2};
    delta[2] = convertit_tableau_etat(ligne2, 2);
    afd a = {.m = m, .n = n, .init = init, .term = term, .delta = delta};
    return a;
}


afd afd_exemple_a2() {
    int m = 3;
    int n = 3;
    etat init = 0;
    bool* term = (bool*) malloc(n*sizeof(bool));
    term[0] = false, term[1] = false, term[2] = true;
    etat** delta = (etat**) malloc(n*sizeof(etat*));
    int ligne0[3] = {1,-1, -1};
    delta[0] = convertit_tableau_etat(ligne0, m);
    int ligne1[3] = {-1,-1, 2};
    delta[1] = convertit_tableau_etat(ligne1, m);
    int ligne2[3] = {-1,2,-1};
    delta[2] = convertit_tableau_etat(ligne2, m);
    afd a = {.m = m, .n = n, .init = init, .term = term, .delta = delta};
    return a;
}

afd afd_exemple_a3() {
    int m = 3;
    int n = 7;
    etat init = 0;
    bool* term = (bool*) malloc(n*sizeof(bool));
    term[0] = false; term[1] = true; term[2] = true;
    term[3] = false; term[4] = false; term[5] = false;
    term[6] = true;
    etat** delta = (etat**) malloc(n*sizeof(etat*));
    int ligne0[3] = {1,2, 4};
    delta[0] = convertit_tableau_etat(ligne0, m);
    int ligne1[3] = {6,6,6};
    delta[1] = convertit_tableau_etat(ligne1, m);
    int ligne2[3] = {-1,-1,-1};
    delta[2] = convertit_tableau_etat(ligne2, m);
    int ligne3[3] = {3,4,2};
    delta[3] = convertit_tableau_etat(ligne3, m);
    int ligne4[3] = {5,5,5};
    delta[4] = convertit_tableau_etat(ligne4, m);
    int ligne5[3] = {4,4,4};
    delta[5] = convertit_tableau_etat(ligne5, m);
    int ligne6[3] = {1,1,1};
    delta[6] = convertit_tableau_etat(ligne6, m);
    afd a = {.m = m, .n = n, .init = init, .term = term, .delta = delta};
    return a;
}







/* visualisation graphique des automates */

void graphviz(afd a, char* filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");  // Open a file in writing mode
    fprintf(fptr, "digraph a {\nrankdir = LR;\n");  // Write some text to the file
    /* etats */
    for (etat q = 0; q < a.n; q+=1) {
        if (a.term[q]) fprintf(fptr, "node [shape = doublecircle, label = %d] %d;\n", q, q);
        else fprintf(fptr, "node [shape = circle, label = %d] %d;\n", q, q);
    }
    /* etat initial */
    fprintf(fptr, "node [shape = point]; I\n");
    fprintf(fptr, "I -> %d;\n", a.init);

    /* transitions */
    char* etiquette = (char*) malloc(taille_max * sizeof(char));
    for (etat q1 = 0; q1 < a.n; q1+= 1) {
        for (etat q2 = 0; q2 < a.n; q2+= 1) {
            etiquette[0] = '\0';
            int index = 0;
            for (int l = 0; l < a.m; l += 1) {
                if (a.delta[q1][l] == q2) {
                    if (!etiquette[0]) index += sprintf(etiquette + index, "%d", l);
                    else index += sprintf(etiquette + index, ", %d", l);
                }
            }
            if (etiquette[0])
                fprintf(fptr, "%d -> %d [ label = \"%s\" ];\n",q1, q2, etiquette);
        }
    }
    fprintf(fptr, "}\n");

    fclose(fptr);  // Close the file
    free(etiquette);
    
}

void genere_pdf(char* input_file, char* output_file) {
    char* commande = malloc((15 + strlen(input_file) + strlen(output_file)) * sizeof(char)); //1 de place pour \0
    sprintf(commande, "dot -Tpdf %s -o %s", input_file, output_file);
    system(commande);
    free(commande);
}



int* copie_tableau(int* tab, int n) {
    int* copie = malloc(n*sizeof(int));
    return memcpy(copie, tab, n*sizeof(int));
}

bool* copie_tab_bool(bool* tab, int n) {
    bool* copie = malloc(n*sizeof(bool));
    return memcpy(copie, tab, n*sizeof(bool));
}


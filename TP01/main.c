#include "graphes.h"
#include <stdlib.h>

int main(int argc, char** argv){
    graphe g = cree_graphe_exemple();

    graphe_print(g);



    graphe_print(graphe_transpose(g));


    int* ordre = (int*)malloc(g.n * sizeof(int));


    graphe_bfs_s0(g, 0, ordre);
    //graphe_bfs_complet(g, ordre);

    graphe_free(g);
    free(ordre);
}

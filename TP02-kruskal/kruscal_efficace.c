#include "kruskal.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]){

    // lecture du fichier de graphe
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <graph_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");

    if (!fp) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    graph_t* graphe = read_graph(fp);
    fclose(fp);


    // init de kruskal
    int nb_chosen = 0;
    edge* T = kruskal(graphe, &nb_chosen);

    printf("Arêtes choisies par l'algorithme de Kruskal :\n");
    float total_weight = 0.0f; // init poids total

    for (int i = 0; i < nb_chosen; i++) {
        printf("(%d, %d) rho=%.3f\n", T[i].x, T[i].y, T[i].rho);
        total_weight += T[i].rho;
    }

    printf("Poids total des arêtes choisies : %.3f\n", total_weight);

    // TODO: differencier arbre et foret couvrant

    // penser à free
    free(T);
    graph_free(graphe);

    return EXIT_SUCCESS;



    // debug kruscal.c
    // if (!graphe) {
    //     fprintf(stderr, "Erreur lors de la lecture du graphe\n");
    //     return EXIT_FAILURE;
    // }

    // int nb_vertex_ex = number_of_edges(graphe);

    // int nb_edges;
    // edge* edges = get_edges(graphe, &nb_edges);

    // if (!edges && nb_edges > 0) {
    //     fprintf(stderr, "Erreur d'allocation des arêtes\n");
    //     graph_free(graphe);
    //     return EXIT_FAILURE;
    // }

    // print_edge_array(edges, nb_edges);

    // sort_edges(edges, nb_edges);

    // // trié
    // print_edge_array(edges, nb_edges);

    // printf("%d\n", nb_vertex_ex);

    // return EXIT_SUCCESS;
}

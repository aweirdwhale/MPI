#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "union_find.h"

#include "kruskal.h"


void graph_free(graph_t* g){
    free(g->degrees);
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

graph_t* read_graph(FILE* fp){
    int n;
    fscanf(fp, "%d", &n);
    int* degrees = malloc(n * sizeof(int));
    edge** adj = malloc(n * sizeof(edge*));
    for (int i = 0; i < n; i++){
        fscanf(fp, "%d", &degrees[i]);
        printf("%d : %d\n", i, degrees[i]);
        adj[i] = malloc(degrees[i] * sizeof(edge));
        for (int j = 0; j < degrees[i]; j++) {
            edge e;
            e.x = i;
            fscanf(fp, " (%d, %f)", &e.y, &e.rho);
            adj[i][j] = e;
        }
    }
    graph_t* g = malloc(sizeof(graph_t));
    g->degrees = degrees;
    g->n = n;
    g->adj = adj;
    return g;
}

void print_graph(graph_t* g, FILE* fp){
    fprintf(fp, "%d\n", g->n);
    for (int i = 0; i < g->n; i++) {
        fprintf(fp, "%d ", g->degrees[i]);
        for (int j = 0; j < g->degrees[i]; j++) {
            edge e = g->adj[i][j];
            fprintf(fp, "(%d, %.3f) ", e.y, e.rho);
        }
        fprintf(fp, "\n");
    }
}


int number_of_edges(graph_t *g) {
    int count = 0; // nb d'arretes

    for (int i = 0; i < g->n; i++) { // g->n == nb de sommets
        for (int j = 0; j < g->degrees[i]; j++) {
            edge e = g->adj[i][j];
            if (i < e.y) { // sommet de départ < sommet d'arrivée => on ne considere chaque arrete qu'ine fois
                count++;
            }
        }
    }
    return count;
}


edge* get_edges(graph_t* g, int* nb_edges) {
    *nb_edges = number_of_edges(g);
    edge* edges = (edge*)malloc(sizeof(edge) * *nb_edges);

    int count = 0;
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->degrees[i]; j++) {
            edge e = g->adj[i][j];
            if (i < e.y) {
                edges[count++] = e;
            }
        }
    }

    return edges;
}

int comparaison(const void* x, const void* y) {
    if (((edge*) x)->rho < ((edge*) y)->rho) {
        return -1;
    } else {
        return 1;
    }
}

void sort_edges(edge* edges, int p) { // p == len(edges) ?

    qsort(edges, p, sizeof(edge), comparaison);

}


void print_edge_array(edge *edges, int len) {
    printf("[|\n");
    for (int i = 0; i < len; i++) {
        printf("Edge %d: (%d, %d) rho=%.3f\n", i, edges[i].x, edges[i].y, edges[i].rho);
    }
    printf("|]\n");
}

edge* kruskal(graph_t* g, int* nb_chosen){
    // pseudo code du tp
    int nb_edges;
    edge* edges = get_edges(g, &nb_edges);

    // tri des arretes
    sort_edges(edges, nb_edges);

    // T <- {}
    edge* T = malloc(nb_edges * sizeof(edge));
    int T_size = 0;

    // X <- CreatePartition(n)
    partition_t* part = partition_new(g->n);

    // pour chaque xy ds A, ordre croissant poids
    for (int i = 0; i < nb_edges; i++) {
        int x = edges[i].x;
        int y = edges[i].y;

        if (find(part, x) != find(part, y)) {
            // Union(X, x, y)
            merge(part, x, y);

            // T <- T U {xy}
            T[T_size] = edges[i];
            T_size++;
        }
    }

    partition_free(part);
    free(edges);

    *nb_chosen = T_size;
    return T;
}

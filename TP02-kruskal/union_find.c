#include <stdlib.h>
#include <stdint.h>
#include "union_find.h"

int nb_sets(partition_t *p){
    return p->nb_sets;
}

int nb_elements(partition_t *p){
    return p->nb_elements;
}

partition_t *partition_new(int nb_elements){
    // crée une partition de [0 n-1] en singketons
    int* arr = (int*)malloc(nb_elements*sizeof(int));
    for (int i = 0; i < nb_elements; i++) {
        arr[i] = -1;
    }

    // struct partition {
    //     int nb_sets;
    //     int nb_elements;
    //     int *arr;
    // };
    partition_t* bassline = (partition_t*)malloc(sizeof(partition_t));

    (*bassline).nb_sets = nb_elements;
    (*bassline).nb_elements = nb_elements;
    (*bassline).arr = arr;


    return bassline;
}

void partition_free(partition_t *p ){
    free((*p).arr);
    free(p);
}

element find(partition_t *p, int x) {

    if (p->arr[x] < 0) {
        return x;
    }
    int representant = find(p, p->arr[x]);

    p->arr[x] = representant;


    return representant;
}

void merge(partition_t *p, element x, element y) {
    element rep_x = find(p, x);
    element rep_y = find(p, y);

    if (rep_x == rep_y) {
        return;
    }

    if ((*p).arr[rep_x] >= (*p).arr[rep_y]) {
        (*p).arr[rep_y] += (*p).arr[rep_x];
        (*p).arr[rep_x] = rep_y;
    } else {
        (*p).arr[rep_x] += (*p).arr[rep_y];
        (*p).arr[rep_y] = rep_x;
    }

    (*p).nb_sets--;

}

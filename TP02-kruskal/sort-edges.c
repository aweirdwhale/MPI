/* version avec qsort */

int compare_weights(const void* e1, const void* e2){
    weight_t w1 = ((edge*)e1)->rho;
    weight_t w2 = ((edge*)e2)->rho;
    if (w1 < w2) return -1;
    if (w1 > w2) return 1;
    return 0;
}

void sort_edges(edge* edges, int p){
    qsort(edges, p, sizeof(edge), compare_weights);
}


/* version tri rapide recodé */

void swap_edges(edge* arr, int i, int j){
    edge tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

int partition(edge* arr, int len){
    weight_t w = arr[0].rho;
    int i = 1;
    for (int j = 1; j < len; j++) {
        if (arr[j].rho <= w) {
            swap_edges(arr, i, j);
            i++;
        }
    }
    swap_edges(arr, 0, i - 1);
    return i - 1;
}

void quicksort_edges(edge* arr, int len){
    if (len <= 1) return;
    int pivot = partition(arr, len);
    quicksort_edges(arr, pivot);
    quicksort_edges(arr + pivot + 1, len - pivot - 1);
}

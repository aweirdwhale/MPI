#include "kmeans.h"


/** Renvoie le CARRÉ de la distance entre deux pixels */
int sqd(pixel p0, pixel p1) {
  int dr = (int)(uint8_t)p0.r - (int)(uint8_t)p1.r;
  int dg = (int)(uint8_t)p0.g - (int)(uint8_t)p1.g;
  int db = (int)(uint8_t)p0.b - (int)(uint8_t)p1.b;
  int da = (int)(uint8_t)p0.a - (int)(uint8_t)p1.a;
  return dr*dr + dg*dg + db*db + da*da;
}

/** Modifie l'isobarycentre de tous les clusters. */
void maj_isobar(const pixel* img, const int* cluster, int N,
                    const int* card, pixel* isobar, int k) {

  // /!\ On fera attention à éviter les overflows. Il faut faire les calculs
  // (sommes de rouge, bleu, etc) dans des int, et retransformer en char le résultat final.

  // sommes pt cluster
  int* sum_r = (int*)malloc(k * sizeof(int));
  int* sum_g = (int*)malloc(k * sizeof(int));
  int* sum_b = (int*)malloc(k * sizeof(int));
  int* sum_a = (int*)malloc(k * sizeof(int));
  for (int i = 0; i < k; ++i) {
    sum_r[i] = 0;
    sum_g[i] = 0;
    sum_b[i] = 0;
    sum_a[i] = 0;
  }

  if (!sum_r || !sum_g || !sum_b || !sum_a) {
    free(sum_r); free(sum_g); free(sum_b); free(sum_a);
    exit(EXIT_FAILURE);
  }

  // composantes pt cluster
  for (int i = 0; i < N; ++i) {
    int c = cluster[i];
    sum_r[c] += (int)(uint8_t)img[i].r;
    sum_g[c] += (int)(uint8_t)img[i].g;
    sum_b[c] += (int)(uint8_t)img[i].b;
    sum_a[c] += (int)(uint8_t)img[i].a;
  }

  // isobar pt cluster
  for (int c = 0; c < k; ++c) {
    if (card[c] > 0) {
      isobar[c].r = (uint8_t)(sum_r[c] / card[c]);
      isobar[c].g = (uint8_t)(sum_g[c] / card[c]);
      isobar[c].b = (uint8_t)(sum_b[c] / card[c]);
      isobar[c].a = (uint8_t)(sum_a[c] / card[c]);
    } else {
      // Si cluster est vide -> isobar ok
    }
  }

  free(sum_r);
  free(sum_g);
  free(sum_b);
  free(sum_a);

  return;
}


/** Renvoie le numéro du cluster associé à un pixel. */
int trouve_cluster(pixel p, const pixel* isobar, int k) {
  int min_dist = sqd(p, isobar[0]);
  int min_idx = 0;

  for (int i = 1; i < k; ++i) {
    int dist = sqd(p, isobar[i]);
    if (dist < min_dist) {
      min_dist = dist;
      min_idx = i;
    }
  }

  return min_idx;
}


/** Remplit le tableau `isobar` avec k centres initialement choisis au hasard parmi les pixels (deux à deux distincts). */
void initialise_centres(int k, const pixel* img, int N, pixel* isobar) {
    /** Remplit le tableau `isobar` avec k centres initialement choisis au hasard parmi les pixels (deux à deux distincts)
        Entrées :
        - `k` le nombre de custers
        - `img`, l'image
        - `N`, le nombre de pixels.
        - `isobar`, le tableau (à remplir) qui à un cluster associe son isobarycentre
    */
    /* On fera un mélange de Fisher-Yates des indices des pixels {0, ..., N-1}, et on sélectionnera les pixels des k premiers indices du mélange dans isobar. */

    int* indices = (int*)malloc(N * sizeof(int));
    if (!indices) {
        fprintf(stderr, "Erreur d'allocation mémoire dans initialise_centres.\n");
        exit(EXIT_FAILURE);
    }

    // Initialiser le tableau des indices
    for (int i = 0; i < N; ++i) {
        indices[i] = i;
    }

    // Mélange de Fisher-Yates
    for (int i = N - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }

    // Sélectionner les k premiers indices pour les centres
    for (int i = 0; i < k; ++i) {
        isobar[i] = img[indices[i]];
    }

    free(indices);
}



/** Algorithme des k moyennes.*/
int kmeans(int k, const pixel* img, int N, int* cluster, pixel* isobar) {

  /* Initialisation */

  /* Itérations de l'algorithme des k moyennes */

  /* Fin */

  return -1;
}
/* ASTUCE : insérer cette ligne dans votre code si vous souhaitez voir joliment la progression de votre clustering sur la grosse image :
printf("\r%5d" " itérations effectuées.", nb_iter);
(avec nb_iter le nombre actuel d'itéations effectuées).
*/


int main(int argc, char* argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage : %s [add-alpha | compress]\n", argv[0]);
    return EXIT_FAILURE;
  }

  srand(time(NULL)); /* Initialise la génération aléatoire d'entiers.
                        L'initialisation dépend de l'heure (en s) depuis le 1er janvier 1970,
                        donc change à chaque seconde.
                      */


  /* Add-alpha : crée une copie de l'image donnée. On
     garantit que cette copie dispose d'un canal alpha.. */
  if (strcmp(argv[1], "add-alpha") == 0) {

    char* input_file = argv[2];
    char* output_file = argv[3];
    if (strcmp(input_file, output_file) == 0) {
      fprintf(stderr, "Par mesure de sécurité, les fichiers d'entrée "
                      "et de sortie doivent être distincts.\n");
      return EXIT_FAILURE;
    }

    int nb_row;
    int nb_col;
    printf("Lecture de %s...\n", input_file);
    fflush(stdout);
    pixel* img = decode_png(input_file, &nb_row, &nb_col);
    printf("Écriture de %s...\n", output_file);
    fflush(stdout);
    encode_png(output_file, img, nb_row, nb_col);
    printf("Fin.\n");
    free(img);

    return EXIT_SUCCESS;
  }

  else if (strcmp(argv[1], "compress") == 0) {

    // Lecture des entrées
    if (argc != 5) {
      fprintf(stderr, "Usage : %s compress input_file output_file nb_de_couleurs.\n", argv[0]);
      return EXIT_FAILURE;
    }

    char* input_file = argv[2];
    char* output_file = argv[3];
    if (strcmp(input_file, output_file) == 0) {
      fprintf(stderr, "Par mesure de sécurité, les fichiers d'entrée "
                      "et de sortie doivent être distincts.\n");
      return EXIT_FAILURE;
    }

    int nb_row = 0;
    int nb_col = 0;
    printf("Lecture de %s...\n", input_file);
    fflush(stdout);
    pixel* img = decode_png(input_file, &nb_row, &nb_col);
    int N = nb_row * nb_col;

    int k = atoi(argv[4]);
    if (k < 0 || k > N) {
      fprintf(stderr, "Un nombre incohérent de couleurs a été demandé. "
                      "Il a été demandé %d couleurs; l'image %s à compresser "
                      "a %d pixels.\n", k, input_file, N);
      free(img);
      return EXIT_FAILURE;
    }

    // Utilisation de kmeans
    int* cluster = (int*) malloc(N*sizeof(int));
    pixel* isobar = (pixel*) malloc(k*sizeof(pixel));
    printf("Début de l'algorithme des k-moyennes...\n");
    fflush(stdout);
    int nb_iter = kmeans(k, img, N, cluster, isobar);
    if(nb_iter < 0) {
      fprintf(stderr, "L'algorithme des k-moyennes n'a pas encore  "
                      "été implémenté.\n");
      free(img);
      free(isobar);
      free(cluster);
      return EXIT_FAILURE;
    }
    printf("\nL'algorithme des k-moyennes a convergé en %d itérations.\n", nb_iter);

    // Grand final !
    for (int i = 0; i < N; i+=1) {
      img[i] = isobar[cluster[i]];
    }
    printf("Écriture de %s...\n", output_file);
    fflush(stdout);
    encode_png(output_file, img, nb_row, nb_col);
    printf("Fin.\n");

    free(img);
    free(isobar);
    free(cluster);
    return EXIT_SUCCESS;

  }

  else {
    fprintf(stderr, "Usage : %s [add-alpha | remove-alpha | compress]\n", argv[0]);
    return EXIT_FAILURE;
  }
}

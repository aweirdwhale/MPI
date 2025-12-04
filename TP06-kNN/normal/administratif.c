/* Helpers pour le TP kNN MNIST */

#include "administratif.h"


/* En plus des 4 modes, on doit coder quelques fonctions.
   Voici leur prototype et documentation.
   
   Elles seront implémentées en dessous des 4 modes.
*/

/** Charge les données de la base MNIST.
	* 
	* Entrées :
  * - file_obs et f_obs sont respectivement 
	*			les FILE* d'observations et d'etiquettes.
	*			Ils doivent avoir été obtenus via opendb et openlb
  *     (rangées de toolbox).
	* - n_obs est le nombre d'observations (le nombre d'images)
	* - taille est le nombre de pixels de chaqueimage
	*			(c'est le même pour tous)
	* - text est un message personnalisé à afficher
	*			durant le chargement.
	*
	* Effets secondaires :
	* - modifie buffer de sorte à y stocker les images,
	*			concaténées les unes après les autres
	* - modifie observations de sorte à ce que observations[i] 
	* 		soit la ième observation (çad la ième image).
	*			Autrement dit, c'est le dé-linéarisé de buffer.
	* - modifie etiquettes de sorte à ce que etiquettes[i]
	*			soit l'étiquette de la ième observation
	*/
void load_data(FILE* file_obs, FILE* file_labels, int n_obs, int taille, char const* text, 
               uint8_t** observations, uint8_t* buffer, uint8_t* etiquettes);

/** Fonction cosmétique pour afficher une jolie barre de progression.
  * Vous n'avez pas à la comprendre. */
void progression(int prog, int prec, int num_test, int tot);




/* Les 4 modes */

/* Affiche la num_image-ème image du fichier nom_fichier.
   Agrandit l'image du niveau de zoom indiqué. 
   
  Sommaire du code :
  0) ouverture du fichier et lecture des métadonnées
  1) lecture de la num_image ème image
  2) Affichage de cette image
  3) Fin (et libération de la mémoire)
*/
void mode_display(char const* nom_fichier, int num_image, int zoom) {

  /* 0) Ouverture du fichier d'images 
        (et lecture des métadonnées) */
  FILE* fichier_img;
  int number_of_images=0;
  int n_rows=0;
  int n_cols=0;
  fichier_img = opendb(nom_fichier, &number_of_images, &n_rows, &n_cols);
  
  // Vérification que l'ouverture a marché
  if (fichier_img == NULL){ 
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_fichier);
    exit(EXIT_FAILURE);
  }
  
  // Vérifcation de la cohérence de la requête.
  if (num_image >= number_of_images){ 
    printf(BOLDRED "Erreur : on veut afficher l'image %d mais il n'existe que %d images\n" RESET, num_image, number_of_images);
    fclose(fichier_img);
    exit(EXIT_FAILURE);
  }
  
  printf("Nombre d'images : %d\nNombre de lignes : %d\nNombre de colonnes : %d\n", number_of_images, n_rows, n_cols);


  /* 1) Lecture de l'image */

  // On alloue l'image
  uint8_t* img = malloc(n_rows * n_cols * sizeof(uint8_t));
  
  // On lit le fichier depuis le début jusqu'à trouver 
  // l'image qui nous intéresse. Chaque nouvelle
  // image écrase la précédente dans img.
  for(int i = 0; i <= num_image; i++) {
    readImage(fichier_img, n_rows * n_cols, img);
  }

  // On n'oublie pas de fermer le fichier !!
  fclose(fichier_img);


  /* 2) Affichage de l'image*/

  afficheImage(img, n_rows, n_cols, zoom);
  

  /* 3) Fin =) . ON N'OUBLIE PAS DE LIBÉRER ! */

  free(img);
  return;
}



/* 1NN : classifie la num_ieme image par 1NN 

  Sommaire du code :
  O) Ouverture des fichiers train et lecture des métadonnées
  1) Lecture de ces deux fichiers
  2) Ouverture et lecture du num_test ème test
  3) Recherche du plus prochain voisin par nearest_obs
  4) Affichage de résultat
  5) Fin (et libération de la mémoire)
*/
void mode_1NN(char const* nom_obs_train, char const* nom_labels_train, char const* nom_obs_test, int num_test, 
              int (*nearest_obs)(uint8_t**, uint8_t const*, int, int, int*) ) {

  /* 0) Ouverture des fichiers de la base d'entrainement 
        (et lecture de leurs métadonnées) */

  // Variables des métadonnées
  int  n_train = 0;
  int   n_rows = 0;
  int   n_cols = 0;
  int  n_tests = 0;
  
  // Ouverture des observations d'entrainement
  FILE* file_obs_train = opendb(nom_obs_train, &n_train, &n_rows, &n_cols);
  if(file_obs_train == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_obs_train);
    exit(EXIT_FAILURE);
  }
  
  // Ouverture des étiquettes associées
  FILE *file_labels_train = openlb(nom_labels_train, &n_train);
  if(file_labels_train == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_labels_train);
    fclose(file_obs_train);
    exit(EXIT_FAILURE);
  }
  

  /* 1) Lecture de ces deux fichiers */
  
  int taille = n_rows * n_cols;
  uint8_t** observations_train = malloc(n_train * sizeof(uint8_t*));
  uint8_t*        buffer_train = malloc(n_train * taille * sizeof(uint8_t));
  uint8_t*    etiquettes_train = malloc(n_train * sizeof(uint8_t));
  load_data(file_obs_train, file_labels_train, n_train, taille, "Chargement des observations", observations_train, buffer_train, etiquettes_train);
  
  // On n'oublie pas de fermer
  fclose(file_obs_train);
  fclose(file_labels_train);
  
  printf(GREEN "\rObservations chargées en mémoire. Nombre d'images de référence : %d\n" RESET, n_train);
  

  /* 2) On ouvre et lit le test (similaire à 0) et 1) ) */
  
  // Ouverture du fichier
  FILE *file_obs_test = opendb(nom_obs_test, &n_tests, &n_rows, &n_cols);
  if(file_obs_test == NULL) {
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_obs_test);
    free(observations_train);
    free(buffer_train);
    free(etiquettes_train);
    exit(EXIT_FAILURE);
  }
  
  // Vérification de la cohérence de la requête
  if(num_test >= n_tests) {
    printf(BOLDRED "Erreur : on veut traiter l'image %d mais il n'existe que %d images\n" RESET, num_test, n_tests);
    fclose(file_obs_test);
    free(observations_train);
    free(buffer_train);
    free(etiquettes_train);
    exit(EXIT_FAILURE);
  }
  
  // On lit le fichier depuis le début jusqu'à trouver 
  // l'image qui nous intéresse. Chaque nouvelle
  // image écrase la précédente dans img.
  uint8_t* img = malloc(taille * sizeof(uint8_t));
  for(int i = 0; i <= num_test; i++) {
    readImage(file_obs_test, taille, img);
  }

  // Fermeture du fichier
  fclose(file_obs_test);
  

  /* 3) On trouve le 1-NN associé */

  int d_max = 0;
  int id_max = nearest_obs(observations_train, img, n_train, taille, &d_max);


  /* 4) On affichage le résultat */
  
  // Compte-rendu de 1-NN
  printf(GREEN "L'image la plus proche est l'observation" BOLDYELLOW " %d " GREEN "(distance de %d)\n" RESET, id_max, d_max);
  printf("La classe de l'image de référence est " BOLDMAGENTA "%d\n" RESET, etiquettes_train[id_max]);
  // printf("Exécuter '" BOLDCYAN "LD_LIBRARY_PATH=. %s display %s %d 10" RESET "' pour afficher l'image de test\n", argv[0], nom_obs_test, atoi(argv[5]));
  printf("L'observation la plus proche selon 1NN est affichée. Si vous voulez revoir l'image de test, utilisez le mode display.\n");
  
  // Affichage de l'image la plus proche
  afficheImage(observations_train[id_max], n_rows, n_cols, 10);
  

  /* 5) Fin =) . On libère ! */

  free(observations_train);
  free(buffer_train);
  free(etiquettes_train);
  free(img);
  return;
}



/* kNN : classifie la num_ieme image par kNN. 

  Sommaire :
  O) Ouverture des fichiers train et lecture des métadonnées
  1) Lecture de ces deux fichiers
  2) Ouverture et lecture du num_test ème test
  3) Classification du test par classify_kNN
  4) Affichage du résultat
  5) Fin (et libération de la mémoire)
*/
void mode_kNN(char const* nom_obs_train, char const* nom_labels_train, char const* nom_obs_test, int num_test, int k,
              uint8_t (*classify_kNN)(uint8_t**, uint8_t const*, uint8_t const*, int, int, int, int*, int*) ) {

  // Les parties 0 1 et 2 sont les mêmes que 1NN.

  /* 0) Ouverture des fichiers de la base d'entrainement 
        (et lecture de leurs métadonnées) */

  // Variables des métadonnées
  int   n_train = 0;
  int   n_rows = 0;
  int   n_cols = 0;
  int n_tests = 0;
  
  // Ouverture des observations d'entrainement
  FILE* file_obs_train = opendb(nom_obs_train, &n_train, &n_rows, &n_cols);
  if(file_obs_train == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_obs_train);
    exit(EXIT_FAILURE);
  }
  
  // Ouverture des étiquettes associées
  FILE *file_labels_train = openlb(nom_labels_train, &n_train);
  if(file_labels_train == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_labels_train);
    fclose(file_obs_train);
    exit(EXIT_FAILURE);
  }
  

  /* 1) Lecture de ces deux fichiers */
  
  int taille = n_rows * n_cols;
  uint8_t** observations_train = malloc(n_train * sizeof(uint8_t*));
  uint8_t*        buffer_train = malloc(n_train * taille * sizeof(uint8_t));
  uint8_t*    etiquettes_train = malloc(n_train * sizeof(uint8_t));
  load_data(file_obs_train, file_labels_train, n_train, taille, "Chargement des observations", observations_train, buffer_train, etiquettes_train);
  
  // On n'oublie pas de fermer
  fclose(file_obs_train);
  fclose(file_labels_train);
  
  printf(GREEN "\rObservations chargées en mémoire. Nombre d'images de référence : %d\n" RESET, n_train);
  

  /* 2) On ouvre et lit le test (similaire à 0) et 1) ) */
  
  // Ouverture du fichier
  FILE *file_test = opendb(nom_obs_test, &n_tests, &n_rows, &n_cols);
  if(file_test == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_obs_test);
    free(observations_train);
    free(buffer_train);
    free(etiquettes_train);
    exit(EXIT_FAILURE);
  }
  
  // Vérification de la cohérence de la requête
  if(num_test >= n_tests){
    printf(BOLDRED "Erreur : on veut traiter l'image %d mais il n'existe que %d images\n" RESET, num_test, n_tests);
    fclose(file_test);
    free(observations_train);
    free(buffer_train);
    free(etiquettes_train);
    exit(EXIT_FAILURE);
  }
  
  // On lit le fichier depuis le début jusqu'à trouver 
  // l'image qui nous intéresse. Chaque nouvelle
  // image écrase la précédente dans img.
  uint8_t* img = malloc(taille * sizeof(uint8_t));
  for(int i = 0; i <= num_test; i++) {
    readImage(file_test, taille, img);
  }

  // Fermeture du fichier
  fclose(file_test);
  

  /* 3) On trouve le k-NN associé */

  int* indices_voisins = malloc(k*sizeof(int));
  int* distances = malloc(k*sizeof(int));
  int class = classify_kNN(observations_train, etiquettes_train, img, n_train, taille, k, indices_voisins, distances);


  /* 4) On affichage le résultat */
  
  // Compte-rendu de 1-NN
  printf(GREEN "L'observation "
         BOLDYELLOW "%d" RESET
         GREEN " est classifiée en "
         BOLDMAGENTA "%d" RESET
         GREEN " par %d-NN."
         RESET, 
         num_test, class, k);
  

  /* 5) Fin =) . On libère ! */

  free(observations_train);
  free(buffer_train);
  free(etiquettes_train);
  free(img);
  free(indices_voisins);
  free(distances);

  return;
}


/* matrix : calcule la matrice de confusion par kNN

  Sommaire :
  O) Ouverture des fichiers train et lecture des métadonnées
  1) Lecture de ces deux fichiers
  2) Ouverture et lecture des deux fichiers tests
  3) Classification de chaque test et construction de la matrice
  4) Libération de la mémoire
  5) Affichage de la matrice et fin
*/
void mode_matrix(char const* nom_obs_train, char const* nom_labels_train, char const* nom_obs_test, char const* nom_labels_test, int k,
              uint8_t (*classify_kNN)(uint8_t**, uint8_t const*, uint8_t const*, int, int, int, int*, int*) ) {

  /* 0) Ouverture des fichiers de la base d'entrainement 
					(et lecture de leurs métadonnées) */

  // Variables des métadonnées
  int n_train = 0;
  int  n_rows = 0;
  int  n_cols = 0;
  int n_tests = 0;
  
  // Ouverture des observations d'entrainement
  FILE* file_obs_train = opendb(nom_obs_train, &n_train, &n_rows, &n_cols);
  if(file_obs_train == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_obs_train);
    exit(EXIT_FAILURE);
  }
  
  // Ouverture des étiquettes associées
  FILE *file_labels_train = openlb(nom_labels_train, &n_train);
  if(file_labels_train == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_labels_train);
    fclose(file_obs_train);
    exit(EXIT_FAILURE);
  }


  /* 1) Lecture de ces deux fichiers */
  
  int taille = n_rows * n_cols;
  uint8_t** observations_train = malloc(n_train * sizeof(uint8_t*));
  uint8_t*        buffer_train = malloc(n_train * taille * sizeof(uint8_t));
  uint8_t*    etiquettes_train = malloc(n_train * sizeof(uint8_t));
  load_data(file_obs_train, file_labels_train, n_train, taille, "Chargement des observations", observations_train, buffer_train, etiquettes_train);
  
  // On n'oublie pas de fermer
  fclose(file_obs_train);
  fclose(file_labels_train);
  
  printf(GREEN "\rObservations chargées en mémoire. Nombre d'images de référence : %d\n" RESET, n_train);


  /* 2) On lit les tests (similaire à ce qui précède) */
  
  // Ouverture des observations de test
  FILE *file_obs_test = opendb(nom_obs_test, &n_tests, &n_rows, &n_cols);
  if(file_obs_test == NULL) {
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_obs_test);
    free(observations_train);
    free(buffer_train);
    free(etiquettes_train);
    exit(EXIT_FAILURE);
  }
  
  // Ouverture des étiquettes associées
  FILE *file_labels_test = openlb(nom_labels_test, &n_tests);
  if(file_labels_test == NULL){
    printf(BOLDRED "Erreur : %s : le fichier n'a pas pu être ouvert en lecture\n" RESET, nom_labels_test);
    free(observations_train);
    free(buffer_train);
    free(etiquettes_train);
    fclose(file_obs_test);
    exit(EXIT_FAILURE);
  }
  
  // Lecture des tests et de leurs vrais labels
  // Rmq : la taille est la même que pour le training set
  uint8_t** observations_test = malloc(n_tests * sizeof(uint8_t*));
  uint8_t*        buffer_test = malloc(n_tests * taille * sizeof(uint8_t));
  uint8_t*     etiquettes_test = malloc(n_tests * sizeof(uint8_t*));
  load_data(file_obs_test, file_labels_test, n_tests, taille, "Chargement des tests", observations_test, buffer_test, etiquettes_test);
  
  // On n'oublie pas de fermer
  fclose(file_obs_test);
  fclose(file_labels_test);
  
  printf(GREEN "\rTests chargées en mémoire. Nombre d'images de test : %d\n" RESET, n_tests);
  

  /* 3) Classification des tests et construction de la matrice */

  printf(MAGENTA "Début des tests...\n" RESET);
  
  // Création de la matrice
  #define NB_CLASSES 10
  int matrice[NB_CLASSES * NB_CLASSES] = {};

  // Initialiser de indices_voisins et distances
  int* indices_voisins = malloc(k * sizeof(int));
  int*       distances = malloc(k * sizeof(int));

  // Initialisation de l'affichage de la barre de progression
  int prec = 100;
  int step = n_tests / prec;
  int compteur = step;
  int prog = 0;
  progression(prog, prec, 0, n_tests);
  
  // Classifier le num_test ème test par kNN.
  // Comparer avec la vraie étiquette, et mettre à jour
  // la matrice en conséquence
  for(int num_test = 0; num_test < n_tests; num_test++){
    
    // Petit bout de code pour montrer la progression des calculs (sans détruire l'affichage du terminal)
    compteur--;
    if(compteur == 0){
      prog++;
      compteur = step;
    }
    progression(prog, prec, num_test+1, n_tests);
    
    // Maintenant, plus accessoire, classifier le test actuel
    int etq_kNN = classify_kNN(observations_train, etiquettes_train, observations_test[num_test], n_train, taille, k, indices_voisins, distances);
    int etq_vraie = etiquettes_test[num_test];

    matrice[etq_vraie * NB_CLASSES + etq_kNN] += 1;
  }
  
  printf("\n" GREEN "Tous les tests ont été effectués.\n" RESET);
  

  /* 4) Un peu de ménage */

  printf(YELLOW "Nettoyage de la mémoire..." RESET);
  fflush(stdout);
  free(observations_train);
  free(buffer_train);
  free(etiquettes_train);
  free(observations_test);
  free(buffer_test);
  free(etiquettes_test);
  
  printf("\r" GREEN "Mémoire libérée.           \n\n" RESET);


  /* 5) Affichage de la matrice */

  printf(GREEN "\rMatrice de confusion de %dNN calculée :     \n\n" RESET, k);
  
  printf("    0     1     2     3     4     5     6     7     8     9\n");
  for(int i = 0; i < NB_CLASSES; i++) {
    printf("%d ", i);
    for(int j = 0; j < NB_CLASSES; j++) {
      printf("%5d ", matrice[i * NB_CLASSES + j]);
    }
    printf("\n");

  }
  printf("\n");
  
  return;
}






/* Les fonctions auxiliaires */

/* Charge les données de la base MNIST. */
void load_data(FILE* f_data, FILE* f_labels, int nb_data, int taille, char const* text, 
               uint8_t** observations, uint8_t* buffer, uint8_t* etiquettes) {
  
  // Pour chaque observation : la lire et lire son label
	for(int i = 0; i < nb_data; i++) {
		observations[i] = &(buffer[i * taille]);
		readImage(f_data, taille, observations[i]);
		readUInt8(f_labels, &etiquettes[i]);

		/* Affiche l'état actuellement du chargement.
		   NB : ça ne ralentirt presque pas le programme, 
		   car l'affichage est en fait en même temps que les Read,
		   lesquels sont très lents (ils lisent le disque dur) */
		printf("\r%s" YELLOW " (%5d/%d)" RESET, text, i+1, nb_data);
		fflush(stdout);
	}

	return;
}


/* Fonction cosmétique pour afficher une jolie barre de progression */
void progression(int prog, int prec, int num_test, int tot) {
	printf("\r[");
	for(int i = 0; i < prec; i++) {
		if(i < prog) printf("=");
		else printf(" ");
	}
	printf("] " YELLOW "%5d/%5d" RESET " tests effectués", num_test, tot);
	fflush(stdout);
}
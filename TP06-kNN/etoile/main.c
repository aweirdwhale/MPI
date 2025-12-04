/* Fichier principal du TP kNN */

#include "administratif.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Remarque : la durée totale d'exécution du programme
		dans son mode le plus lent (matrix)	est d'environ 30s
		avec gcc -O3 sur un bon processeur, et plus de 14min30s
		avec gcc -O0 (l'option par défaut).
		
		Pour se rassurer, des affichages sont pré-codés afin
		d'indiquer l'avancement du calcul. Ces affichages ont
		pour la plupart lieu en parallèle d'opération très lentes 
		(lectures du disque dur), et ralentissent donc peu
		le tout. Il est déconseillé d'essayer de les enlever
		avant d'avoir fini le TP. 
*/




/** Calcule la distance euclidienne entre x et y.
	*	Ces deux vecteurs sont de dimension taille. 
	*/
int distance(uint8_t const* x, uint8_t const* y, int taille) {
	// TODO : coder cette fonction
	exit(EXIT_FAILURE); // Enlever cette ligne une fois la fonction codée


  // NB : les processeurs modernes ont des instructions spéciales pour faire
  // des produits scalaire entre tableaux de 64 octets. L'option -O3 essaye
  // (entre autres) de détecter des produits scalaires, et de les faire
  // 64 octets par 64 octets. Elle permet donc un énorme gain de temps
  // sur cette fonction !! (x16 si les casts en int ont été faits)
}




/** Renvoie l'indice de l'observation la plus proche de test.
  *	Si d_ret != NULL, stocke la distance associée dans *d_ret 
	*/
int nearest_obs(uint8_t** observations_train, uint8_t const* test,  int n_train, int taille, 
								int* d_ret){
	// TODO : coder cette fonction
	exit(EXIT_FAILURE); // Enlever cette ligne une fois la fonction codée
}


/** Stocke dans indices_voisins les indices des k plus proches voisins de test.
	* Stocke également dans distances les distances associées.
	*/
void k_nearest_obs(uint8_t** observations_train, uint8_t const* test, int n_train, int taille, int k, 
									 int* indices_voisins, int* distances) {
	// TODO : coder cette fonction
	exit(EXIT_FAILURE); // Enlever cette ligne une fois la fonction codée
}


/** Classifie test à l'aide de kNN.
	* NB : indices_voisins et distances sont passés en argument pour pouvoir
	*			 être réutilisés entre les appels et ne pas être malloc/free 
	*			 à chaque fois (ce qui serait très lent).
	*      Ils doivent pouvoir contenir k entier.
	*/
uint8_t classify_kNN(uint8_t** observations_train, uint8_t const* etiquettes_train, uint8_t const* test, int n_train, int taille, int k,
							   int *indices_voisins, int* distances) {
	// TODO : coder cette fonction
	exit(EXIT_FAILURE); // Enlever cette ligne une fois la fonction codée
}



/** Selon les arguments, on propose trois modes :
	* - display : affiche une image.
  *			Utilisation : ./exe display fichier n zoom 
  *     Affiche la n-ième image de fichier avec le zoom donné.
  * - 1NN : classifie une image de test.
	* 		Utilisation : ./exe 1NN fichier_train labels_train fichier_test num_test
	* 		Classifie la num_test-ème image de fichier_test 
	*			par 1NN (entrainé avec les données train)
  * - kNN : classifie une image de test.
	* 		Utilisation : ./exe kNN fichier_train labels_train fichier_test num_test k
	* 		Classifie la num_test-ème image de fichier_test par kNN (entrainé avec les données train)
  * - matrix : calcule la matrice de confusion 
	* 		Utilisation : ./exe matrix fichier_train labels_train fichier_test labels_test k
	*			Classifie toutes les images de test par kNN
	*			(entrainé avec les images train), puis calcule
	*			la matrice de confusion.
*/
int main(int argc, char* argv[]){
	
  /* On s'assure que le mode a été donné */
	if(argc <= 1){
		printf(BOLDRED "Usage : %s [display | 1NN | kNN | matrix]\n" RESET, argv[0]);
		return EXIT_FAILURE;
	}


	/* Mode display : affichage la num_image-ème image */
	if(strcmp(argv[1], "display") == 0) {
		
		// On vérifie qu'on a le bon nombre d'arguments
		if(argc < 5){
			fprintf(stderr, BOLDRED "Usage : %s display fichier numero_image_a_afficher zoom\n" RESET, argv[0]);
			return EXIT_FAILURE;
		}
		
		// On lit la ligne de commande et lance le mode display
		char* nom_fichier = argv[2];
		int num_image = atoi(argv[3]);
		int zoom = atoi(argv[4]);
		mode_display(nom_fichier, num_image, zoom);

		// Fin
		return EXIT_SUCCESS;
	}
	

	/* Mode 1NN : essaye de classifier la num_test-ème image */
	else if(strcmp(argv[1], "1NN") == 0) {

		// On vérifie qu'on a le bon nombre d'arguments
		if(argc < 6){
			printf(BOLDRED "Usage : %s 1NN obs_train labels_train obs_test numero_test_a_classifier\n" RESET, argv[0]);
			return EXIT_FAILURE;
		}
		
		// On lit la ligne de commande et lance le mode 1NN
		char* nom_obs_train = argv[2];
		char* nom_labels_train = argv[3];
		char* nom_obs_test = argv[4];
		int num_test = atoi(argv[5]);
		mode_1NN(nom_obs_train, nom_labels_train, nom_obs_test, num_test, nearest_obs);

		// Fin
		return EXIT_SUCCESS;
	}


	/* Mode kNN : essaye de classifier la num_test-ème image */
	else if(strcmp(argv[1], "kNN") == 0) {

		// On vérifie qu'on a le bon nombre d'arguments
		if(argc < 7){
			printf(BOLDRED "Usage : %s kNN obs_train labels_train obs_test numero_test_a_classifier k\n" RESET, argv[0]);
			return EXIT_FAILURE;
		}
		
		// On lit la ligne de commande et lance le mode kNN
		char* nom_obs_train = argv[2];
		char* nom_labels_train = argv[3];
		char* nom_obs_test = argv[4];
		int num_test = atoi(argv[5]);
		int k = atoi(argv[6]);
		mode_kNN(nom_obs_train, nom_labels_train, nom_obs_test, num_test, k, classify_kNN);

		// Fin
		return EXIT_SUCCESS;
	}



	/* Mode matrix : calcule la matrice de confusion sur la totalité des tests */
	else if(strcmp(argv[1], "matrix") == 0){

		// On vérifie qu'on a le bon nombre d'arguments
		if(argc < 7){
			printf(BOLDRED "Usage : %s matrix fichier_train labels_train fichier_test labels_test k\n" RESET, argv[0]);
			return EXIT_FAILURE;
		}

		// On lit la ligne de commande et lance le mode matrix
		char* nom_obs_train = argv[2];
		char* nom_labels_train = argv[3];
		char* nom_obs_test = argv[4];
		char* nom_labels_test = argv[5];
		int k = atoi(argv[6]);
		mode_matrix(nom_obs_train, nom_labels_train, nom_obs_test, nom_labels_test, k, classify_kNN);

		// Fin
		return EXIT_SUCCESS;
	}
	

	/* Sinon : le mode est invalide */
	else {
		printf(BOLDRED "Usage : %s [display|test|matrix]\n" RESET, argv[0]);
		return EXIT_FAILURE;
	}
}
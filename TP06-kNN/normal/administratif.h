/* Des fonctions pré-codées qui lisent les bases de données et lancent les calculs dessus */

#ifndef ADMINISTRATIF_H
#define ADMINISTRATIF_H 0

#include "toolbox.h"
#include <stdio.h>
#include <stdlib.h>


/* Les 4 modes proposés */

/** Display : affiche la num-ème image.
  *
  * Entrées :
  * - nom_fichier : le nom d'un fichier d'observations de MNIST,
  *     c'est à dire un fichier d'images de MNIST
  * - num_image : le numéro de l'image à afficher
  * - zoom : coefficient multiplicatif qui redimensionne l'image
  *
  * Effets secondaires :
  * - ouvre une fenêtre qui contient l'image demandée
  */
void mode_display(char const* nom_fichier, int num_image, int zoom);

/** 1NN : classifie la num_ieme image par 1NN.
  *
  * Entrées :
  * - nom_obs_train : le nom du fichier contenant les
  *     observations d'entrainement.
  * - nom_labels_train : le nom du fichier contenant
  *     les labels des observations d'entrainement.
  * - nom_obs_test : le nom du fichier contenant
  *     l'observation à tester
  * - num_test : le numéro du test à classifier
  * - nearest_obs : la fonction qui renvoie l'indice
  *     du plus proche voisin, cf énoncé et main.c
  *     NB : le typage fait peur : ignorez-le.
  */
void mode_1NN(char const* nom_obs_train, char const* nom_labels_train, char const* nom_obs_test, int num_test,
              int (*nearest_obs)(uint8_t**, uint8_t const*, int, int, int*) );

/** kNN : classifie la num_ieme image par kNN.
  *
  * Entrées :
  * - nom_obs_train : le nom du fichier contenant les
  *     observations d'entrainement.
  * - nom_labels_train : le nom du fichier contenant
  *     les labels des observations d'entrainement.
  * - nom_obs_test : le nom du fichier contenant
  *     l'observation à tester
  * - num_test : le numéro du test à classifier
  * - k : le métaparamètre de kNN
  * - classify_kNN : la fonction qui renvoie la classe
  *     majoritaire parmi les k plus proches voisins,
  *     cf énoncé et main.c
  *     NB : le typage fait peur : ignorez-le.
  */
void mode_kNN(char const* nom_obs_train, char const* nom_labels_train, char const* nom_obs_test, int num_test, int k,
              uint8_t (*classify_kNN)(uint8_t**, uint8_t const*, uint8_t const*, int, int, int, int*, int*) );

/** matrix : calcule la matrice de confusion par kNN.
  *
  * Entrées :
  * - nom_obs_train : le nom du fichier contenant les
  *     observations d'entrainement.
  * - nom_labels_train : le nom du fichier contenant
  *     les labels des observations d'entrainement.
  * - nom_obs_test : le nom du fichier contenant
  *     les observations de test.
  * - nom_labels_train : le nom du fichier contenant
  *     les labels des observations de test.
  * - k : le métaparamètre de kNN
  * - classify_kNN : la fonction qui renvoie la classe
  *     majoritaire parmi les k plus proches voisins,
  *     cf énoncé et main.c
  *     NB : le typage fait peur : ignorez-le.
  */
void mode_matrix(char const* nom_obs_train, char const* nom_labels_train, char const* nom_obs_test, char const* nom_labels_test, int k,
              uint8_t (*classify_kNN)(uint8_t**, uint8_t const*, uint8_t const*, int, int, int, int*, int*) );




/* Quelques macros utiles pour faire de l'affichage coloré.
 * Pour utiliser un de ces effets : 
 *        printf(EFFET "txt à afficher" RESET, ...) 
 */
#define RESET   "\033[0m"       /* Désactive tous les effets */
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */



#endif
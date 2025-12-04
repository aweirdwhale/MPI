/* Utilitaires pour le TP MNIST */


#ifndef TOOLBOX_H
#define TOOLBOX_H 0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/* Rappels :
  - les images sont linéarisées en tableaux de pixels
  - chaque pixel est un niveau de gris, stocké sur un uint8_t
*/


/** Affiche l'image img.
  * - n_rows est le nombre de ligne du rendu visuel
  * - n_cols le nombre de colonnes
  * - zoom est un multiplicateur à appliquer à n_rows et n_cols.
  *   En particulier, plus zoom est grand plus l'image affichée 
  *   sera grande. 
  */
void afficheImage(uint8_t* img, int n_rows, int n_cols, int zoom);


/** Lit un uint8_t de f, le stocke dans *c */
void readUInt8(FILE* f, uint8_t* c);


/** Lit toute une image, la stocke dans *buff */
void readImage(FILE* f, int taille, uint8_t* img);


/** Ouvre une database (observations) de MNIST et en lit 
  * uniquement les métadonnées.
  * 
  * Entrée :
  * - file le nom du fichier.
  *
  * Sorties :
  * - renvoie le FILE* associé à la database.
  * - modifie *number_of_images pour y écrire 
  *     le nombre d'images présente dans la database.
  * - modifie *n_rows pour y écrire le nombre 
  *     de lignes de chaque image (c'est le même pour toutes).
  * - modifie *n_cols pour y écrire le nombre 
  *     de colonnes de chaque image (idem).
  */
FILE *opendb(char const* file, int *number_of_images, int *n_rows, int *n_cols);


/** Ouvre une labelbase (étiquettes) de MNIST et en lit
  * uniquement les métadonnées.
  * 
  * Entrée :
  * - file le nom du fichier.
  *
  * Sorties :
  * - renvoie le FILE* associé à la labelbase.
  * - modifie *number_of_images pour y écrire 
  *     le nombre d'images étiquettées par la labelbase.
  */
FILE *openlb(char const* file, int *number_of_images);

#endif
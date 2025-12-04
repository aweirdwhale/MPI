/** Quelques outils pré-codés pour manipuler des PNG */

#ifndef OUTILS_H
#define OUTILS_H 0

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


/** Un pixel est composé de 4 données : Red, Green, Blue, Alpha.
    Les trois premières sont des nuances de Rouge, Vert, Bleu (encodées
    entre 0 et 256). La dernière est une information de transparence (également
    un uin8_t).*/
struct pixel_s {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};
typedef struct pixel_s pixel;


/** Lit le fichier `filename` qui doit être un PNG et : 
    - renvoie un pointeur vers tous ses pixels les uns après les autres.
    - stocke le nombre de lignes du PNG dans `*nb_row`.
    - stocke le nombre de colonnes du PNG dans `*nb_col`.
    
    Attention : si cette fonction ne parvient pas à lire le PNG, elle
    termine de force le programme. Si elle parvient à le lire, RAS. 
*/
pixel* decode_png(const char* filename, int* nb_row, int* nb_col);


/** Écrit le fichier `filename`. Y stocke l'image `img` à `nb_row` lignes et
   `nb_col` colonnes. Utilise le format PNG, en mode RGBA.
    
    Attention : si cette fonction ne parvient pas à écrire le PNG, elle
    termine de force le programme. Si elle parvient à l'écrire, RAS. */
void encode_png(const char* filename, const pixel* img, int nb_row, int nb_col);

#endif
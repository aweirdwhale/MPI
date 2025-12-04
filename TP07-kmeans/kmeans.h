/* Header d'une implémentation de kmean pour des pixels */

#ifndef KMEAN_H
#define KMEAN_H 0

#include "outils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


/** Renvoie le CARRÉ de la distance euclidienne entre deux pixels */
int sqd(pixel p0, pixel p1);


/** Modifie l'isobarycentre de tous les clusters.
    Entrées :
    - `img`, l'image 
    - `cluster`, le tableau qui à l'indice d'un pixel associe le
                numéro de son cluster.
    - `N`, le nombre de pixels.
    - `card`, le tableau qui à un cluster associe son cardinal
    - `isobar`, le tableau qui à un cluster associe son isobarycentre
    - `k` le nombre de custers

    Sortie : Rien
    Effets Secondaires : 
      - modifie `isobar` de sorte que isobar[c] soit l'isobarycentre
        des pixels du cluster c.

    Rappel : isobarycentre n'est qu'un nom fancy pour << moyenne >>.
 */
void maj_isobar(const pixel* img, const int* cluster, int N,
                    const int* card, pixel* isobar, int k);


/** Renvoie le numéro du cluster associé à un pixel.
    Entrées :
    - `p`, le pixel à classer.
    - `isobar`, le tableau qui à un cluster associe son isobarycentre
    - `k` le nombre de custers

    Sortie : Le cluster à associer à `p`
    */
int trouve_cluster(pixel p, const pixel* isobar, int k);


/** Remplit le tableau `isobar` avec k centres initialement choisis au hasard parmi les pixels (deux à deux distincts) 
    Entrées :
    - `k` le nombre de custers
    - `img`, l'image 
    - `N`, le nombre de pixels.
    - `isobar`, le tableau (à remplir) qui à un cluster associe son isobarycentre
*/
void initialise_centres(int k, const pixel* img, int N, pixel* isobar);


/** Algorithme des k moyennes.
    Entrées :
    - `k` le nombre de classes à créer.
    - `img`, l'image.
    - `N`, le nombre de pixels de l'image.
    - `cluster`, un tableau d'entiers à `N` cases où sera stockée l'association
                pixel-> cluster.
    - `isobar`, un tableau de pixels à `k` cases où sera stockée l'association
                cluster -> isobarycentre.
    
    Sorties : renvoie le nombre d'itérations requises pour converger.
    Effets Secondaires :
    - modifie `cluster` de sorte que `cluster[i]` contienne le numéro du
                cluster auquel est associé le pixel d'indice `i`.
    - modifie `isobar` de sorte que `isobar[c]` contienne l'isobarycentre
                du cluster numéro `c`.
*/
int kmeans(int k, const pixel* img, int N, int* cluster, pixel* isobar);

#endif
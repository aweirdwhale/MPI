#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "dynarray.h"
#include "pile.h"
#include "file.h"

struct graphe_s {
  dynarray* lA; //tableau des "listes" d'adjacence du graphe (sous forme de tableaux dynamiques)
  int n; //nombre de sommets du graphe
};
typedef struct graphe_s graphe;


/* Première section */

/** Renvoie le graphe-exemple.
  * NB : vous êtes encouragés à créer un 
  * graphe-exemple supplémentaire 
  */
graphe cree_graphe_exemple(void); // TODO

/** Libère un graphe. */
void graphe_free(graphe G); // TODO

/** Renvoie le graphe transposé de [G] */
graphe graphe_transpose(graphe G); // TODO

/** Affiche les listes d'adjacences d'un graphe */
void graphe_print(graphe G); // DONE


// Ces 4 prochaines fonctions sont du bonus, 
// ne passez pas de temps dessus si
// vous n'en ressentez pas le besoin.

/** Renvoie le degré de [s] dans [G] */
int graphe_deg(graphe G, int s);

/** Teste si [(a,b)] est un arc de [G] */
bool graphe_is_edge(graphe G, int a, int b);

/** Ajoute l'arc [(a,b)] à [G] */
void graphe_add_edge(graphe G, int a, int b);

/** Retire l'arc [(a,b)] de [G] */ // À faire après les parcours !!
void graphe_rem_edge(graphe G, int a, int b);


/* Seconde section : BFS */

/** Écrit dans [ordre] un ordre de BFS de [G] depuis [s0] */
void graphe_bfs_s0(graphe G, int s0, int ordre[]);

/** Écrit dans [ordre] un ordre de BFS complet de [G] */
void graphe_bfs_complet(graphe G, int ordre[]);

// La prochaine fonction est un bonus. Avancez sur DFS d'abord.

/** Écrit dans [dist] les distances depuis [s0] dans [G] (non-pondéré) */
void graphe_dist_s(graphe G, int s0, int dist[]);


/* Troisième section : DFS */

/** Écrit dans [ordre] un ordre de BFS de [G] depuis [s0] */
void graphe_dfs_s0_pile(graphe G, int s0, int ordre[]);

/** Écrit dans [ouverture] et [fermeture] 
  * des dates d'ouverture et de fermeture lors 
  * d'un DFS [G] depuis s0 
  */
void graphe_dfs_s0_rec(graphe G, int s0, int ouverture[], int fermeture[]);


/** Écrit dans [ouverture] et [fermeture] 
  * des dates d'ouverture et de fermeture lors 
  * d'un DFS complet de [G] 
  */
void graphe_dfs_rec(graphe G, int ouverture[], int fermeture[]);

// La prochaine fonction est un bonus. Avancez sur DFS d'abord.

/** Écrit dans [dist] les distances depuis [s0] dans [G] (non-pondéré) */
void graphe_dist_s0(graphe G, int s0, int dist[]);


/** Écrit dans [tri_topo] un ordre topologique valide du graphe (si celui-ci est bien acyclique) */
void graphe_tri_topo(graphe G, int tri_topo[]);



/** Le main des élèves (renommé par le Makefile) */
int test__eleve(int argc, char* argv[]);

type sommet = int
type graphe = sommet list array

type arete = {x : sommet; y : sommet}

type couplage = arete list
type graphe_biparti = { g : graphe; partition : bool array }

val est_dans_couplage : arete -> couplage -> bool

val difference_symetrique : couplage -> couplage -> arete list

val est_couvert : sommet -> couplage -> bool

val graphe_de_couplage : graphe_biparti -> couplage -> graphe

val arbre_parcours : graphe -> sommet -> sommet array

val chemin : graphe -> sommet -> sommet -> arete list

val couplage_maximum_biparti : graphe_biparti -> couplage
type etat = {
  grille : int array array;
  mutable i : int;
  mutable j : int;
  mutable h : int;
}

type direction = Haut | Bas | Gauche | Droite | Rien

val affiche_etat : etat -> unit

val mouvements_possibles : etat -> direction list

val calcule_h : etat -> unit

val delta_h : etat -> direction -> int

val applique : etat -> direction -> unit

val copie : etat -> etat

val successeurs : etat -> etat list

val reconstruit : ('a, 'a) Hashtbl.t -> 'a -> 'a list

val astar : etat -> etat list


open Othello

val compte_couleur : etat -> int*int

val gagnant : etat -> couleur

val eval : etat -> int

val minmax : etat -> int

val dict_minmax :etat -> couleur -> (etat,etat) Hashtbl.t

val heur_naive : etat -> int

val heur_points : etat -> int 

val minmax_heur : (etat -> int) -> int -> etat -> int 
val strat_minmax_heur : (etat -> int) -> int -> etat -> etat

val alpha_beta :  (etat -> int) -> int -> etat -> int 
val strat_alpha_beta :  (etat -> int) -> int -> etat -> etat

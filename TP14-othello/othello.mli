(* Fonctions d'othello déjà programmées pour vous *)


(* Le type que l'on utilise : *)

(** Type des cases du plateau *)
type couleur = Vide | Noir | Blanc

(** Type des configurations *)
type etat = { joueur : couleur; plateau : couleur array array }

exception Coup_Invalide


(* Petites fonctions utilitaires fort pratiques : *)

(** Renvoie la dimension du côté plateau *)
val dim : etat -> int

(** Renvoie la couleur du joueur précédent/suivant *)
val next : couleur -> couleur

(** Renvoie l'état initial. On donne la longueur du plateau en argument. *)
val initial : int -> etat

(** Renvoie une copie profonde d'un tableau *)
val matrix_copy : couleur array array -> couleur array array

(** Affiche le plateau d'un état *)
val affiche_plateau : etat -> unit


(* Fonctions permettant de jouer : *)

(** [cases_retournees etat x y mouv] renvoie les cases retournées dans la direction [mouv] 
    par le coup [x,y] sur [etat].

    [mouv] est un vecteur non-nul de {-1,0,1}² .
    
    Renvoie [[]] si aucune case n'est retournée. *)
val cases_retournees : etat -> int -> int -> int * int -> (int * int) list

(** [joue etat x y] est l'état atteint en jouant [(x,y)] depuis [etat].
    
    Lève [Coup_Invalide] si le coup ne respecte pas les règles. *)
val joue : etat -> int -> int -> etat

(** Renvoie la liste des états atteignables depuis un état. 
    Si le joueur doit passer son tour, [enfants etat] contient 
    la même position que [etat], mais où le joueur est changé. *)
val enfants : etat -> etat list


(** Teste si un état est terminal *)
val est_terminal : etat -> bool


(* Une jolie simulation : *)

(** [simule_partie n stratNoir stratBlanc] simule une partie [n x n] 
    en suivant les deux stratégies données pour les deux joueurs *)
val simule_partie : int -> (etat -> etat) -> (etat -> etat) -> unit

(** Exemple de simulation de partie. 
    Utilise un minmax non-borné comme stratégie.
    NB : ne marchera pas en temps raisonnable au-delà du 4x4 *)
val run_example : int -> unit

type controle = J1 | J2
type config = {joueur : controle; tablette : int array; q : int}
type graphe = (config, config list) Hashtbl.t

exception Invalide
exception Empty

val next : controle -> controle

val print_tablette : config -> unit


val config_initiale : int -> int -> config
val tablette_valide : config -> bool
val chomp : int -> int -> config -> config
val construit_graphe : int -> int -> graphe
val transpose_graphe : graphe -> graphe
val terminaux : int -> int -> controle -> config list
val stocke_deg : (config, int) Hashtbl.t -> config -> config list -> unit
val marque : (config, bool) Hashtbl.t -> config -> unit 
val decremente : ('a, int) Hashtbl.t -> 'a -> unit
val calcul_attracteurs : int -> int -> controle -> (config, bool) Hashtbl.t
(*val rang : int -> int -> controle -> (config, int) Hashtbl.t *)

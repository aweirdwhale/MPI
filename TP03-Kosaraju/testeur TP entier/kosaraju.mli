type sommet = int
type graphe = sommet list array

val transpose : graphe -> graphe

val dfs_post : graphe -> sommet list

val kosaraju : graphe -> sommet list list


type litteral =
    | P of int (* occurrence positive *)
    | N of int (* occurrence negative *)

type clause = litteral * litteral
type deuxcnf = clause list
type valuation = bool array

val eval_litt : litteral -> valuation -> bool

val eval : deuxcnf -> valuation -> bool

exception Last
val incremente_valuation : valuation -> unit

val brute_force : deuxcnf -> valuation option

val graphe_de_cnf : deuxcnf -> graphe

val satisfiable : deuxcnf -> bool

val temoin : deuxcnf -> valuation option
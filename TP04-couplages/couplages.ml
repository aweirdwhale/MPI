type sommet = int
type graphe = sommet list array

type arete = {x : sommet; y : sommet}

type couplage = arete list
type graphe_biparti = { g : graphe; partition : bool array }

(* graphe biparti du cours : *)
let gb =
  let g = [|
    [8; 9];    (* 0  *)
    [9; 10];    (* 1  *)
    [8; 11]; (* 2  *)
    [11];       (* 3  *)
    [7; 8];       (* 4  *)
    [10; 12];    (* 5  *)
    [12];       (* 6  *)
    [4];        (* 7  *)
    [0; 2; 4];   (* 8  *)
    [0; 1];   (* 9  *)
    [1; 5];   (* 10 *)
    [2; 3];   (* 11 *)
    [5; 6]    (* 12 *)
  |]
  in { g = g; partition = [| true; true; true; true; true; true; true; false; false; false; false; false; false |] }


(* graphe à 6 sommets pour lequel le plus grand couplage possible ne couvre que 4 sommets *)
let gb2 =
  let g = [|
   [1];  (* 0 *)
   [0; 2; 4];  (* 1 *)
   [1];  (* 2 *)
   [4];  (* 3 *)
   [1; 3; 5];  (* 4 *)
   [4]  (* 5 *)
  |]
  in { g = g; partition = [| true; false; true; false; true; false |]}


let gb_non_connexe =
  let g = [|
    [8; 9];    (* 0  *)
    [9; 10];    (* 1  *)
    [8; 11]; (* 2  *)
    [11];       (* 3  *)
    [7; 8];       (* 4  *)
    [10; 12];    (* 5  *)
    [12];       (* 6  *)
    [4];        (* 7  *)
    [0; 2; 4];   (* 8  *)
    [0; 1];   (* 9  *)
    [1; 5];   (* 10 *)
    [2; 3];   (* 11 *)
    [5; 6];    (* 12 *)

    [14];  (* 13 *)
    [13; 15; 17];  (* 14 *)
    [14];  (* 15 *)
    [17];  (* 16 *)
    [14; 6; 18];  (* 17 *)
    [17]  (* 18 *)
  |]
  in { g = g; partition = [| true; true; true; true; true; true; true; false; false; false; false; false; false; true; false; true; false; true; false  |] }





(* ----- Fonctions ----- *)

let rec est_dans_couplage ar c =
  match c with
  | t :: q ->
      if (t.x = ar.x && t.y = ar.y) || (t.x = ar.y && t.y = ar.x) then true
      else est_dans_couplage ar q
  | [] -> false


(*pour éviter ANB : renvoie les arretes qui ne sont pas dans ANB (a et b) *)
let prive_de a b =
  List.fold_left
    (fun tri arete ->
       if est_dans_couplage arete b then tri
       else arete :: tri
    )
    [] a


let difference_symetrique c1 c2 =
  let a = prive_de c1 c2 in
  let b = prive_de c2 c1 in
  (* concat. evite doublons *)
  let res = a @ b in
  res



(* renvoie vrai si s est une extrémité d'une des arêtes du couplage, faux sinon *)
let rec est_couvert s c =
  match c with
  |t::q -> if t.x = s || t.y = s then true else est_couvert s q
  | [] -> false


(** renvoie le graphe (orienté) du couplage gc,
  les sommets s et t sont les sommets d'indice n et n+1. *)
let graphe_de_couplage gb c =
  let n = Array.length gb.g in (* taille de la liste d'adjacence *)
  let s_idx = n in (* index de s *)
  let t_idx = n + 1 in (* index de t *)

  let gbc = Array.make (n + 2) [] in
  (* 1) arêtes s -> x pour tout x ∈ X (partition(s) = true) non couvert *)
  for v = 0 to n - 1 do
    if gb.partition.(v) = true && not (est_couvert v c) then
      gbc.(s_idx) <- v :: gbc.(s_idx)
  done;
  (* 2) pour toute arête (u,v) de gb :
       - si u ∈ X et v ∈ Y :
           si (u,v) ∈ C alors on ajoute l'arête v -> u (arête inversée)
           sinon on ajoute l'arête u -> v (arête non couplée)
       (si liste d'adjacence contient deux sens, la partition assure que chacun est traité une fois)
  *)
  (* pour toute arête du couplage, ajouter v->u *)
  List.iter
    (fun ar ->
       (* dét. qui est dans X *)
       if gb.partition.(ar.x) then
         (* ar.x in X, ar.y in Y *)
         gbc.(ar.y) <- ar.x :: gbc.(ar.y)
       else if gb.partition.(ar.y) then
         (* ar.y in X, ar.x in Y *)
         gbc.(ar.x) <- ar.y :: gbc.(ar.x)
       else
         () )
    c;
  (* pour toute arête de G, add arêtes non couplées u->v lorsque u∈X, v∈Y et (u,v)∉C *)
  for u = 0 to n - 1 do
    if gb.partition.(u) then
      List.iter
        (fun v ->
           (* uniquement voisins en Y *)
           if not gb.partition.(v) then
             let ar = { x = u; y = v } in
             if not (est_dans_couplage ar c) then
               gbc.(u) <- v :: gbc.(u)
        )
        gb.g.(u)
  done;
  (* y -> t pour tout y ∈ Y non couvert *)
  for v = 0 to n - 1 do
    if not gb.partition.(v) && not (est_couvert v c) then
      gbc.(v) <- t_idx :: gbc.(v)
  done;
  (* graphe orienté construit *)
  gbc


(* renvoie le tableau des prédécesseurs dans un parcours (quelconque) de g depuis s *)
(*classico parcours*)
let arbre_parcours g s =
  let n = Array.length g in
  let pred = Array.make n (-1) in
  let q = Queue.create () in
  pred.(s) <- s; (* parent de racine = racine *)
  Queue.add s q;
  while not (Queue.is_empty q) do
    let u = Queue.take q in
    List.iter
      (fun v ->
          if pred.(v) = -1 then (
            pred.(v) <- u;
            Queue.add v q
          )
      )
      g.(u)
  done;
  pred


(* Construis un chemin de s à t dans g, s'il existe, sous la forme d'une liste d'arêtes.
  S'il n'en existe pas, on renvoie le chemin vide []. *)
  (* idée : regarder à l'envers tout les prédécesseurs successifs de t jusqu'à trv s *)
let chemin g s t =
  let pred = arbre_parcours g s in
    if pred.(t) = -1 then [] else (* pas de predecesseur = pas de chemin possible *)
    (* liste de sommets de t à s *)

    let rec build acc v =
      if v = s then s :: acc else build (v :: acc) pred.(v)
    in
    let sommets = build [] t in
    (* transformer en liste d'arêtes (x -> y) suivant l'ordre de sommets *)
    let rec aux acc = function
      | [] | [_] -> List.rev acc
      | u :: v :: rest -> aux ({ x = u; y = v } :: acc) (v :: rest)
    in
    aux [] sommets



let couplage_maximum_biparti gb =
  let n = Array.length gb.g in
  let s_idx = n in
  let t_idx = n+1 in

  let rec loop c =
    let gc = graphe_de_couplage gb c in
    let p = chemin gc s_idx t_idx in
    if p = [] then c
    else
      (*recup art de gb sans celles de s et t*)
      (* u->v dans p corr. une art de gb (u<n, v<n) *)
      let path_edg =
        List.fold_left
        (fun art_sans_st art ->
          if art.x < n && art.y <n
          then ({x = art.x; y=art.y} :: art_sans_st)
          else art_sans_st
        ) [] p in

    let c' = difference_symetrique c path_edg in
    loop c'
  in loop []

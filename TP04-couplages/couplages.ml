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
  |> List.rev


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

  let res = Array.make (n + 2) [] in
  (* 1) arêtes s -> x pour tout x ∈ X non couvert *)
  for v = 0 to n - 1 do
    if gb.partition.(v) = true && not (est_couvert v c) then
      res.(s_idx) <- v :: res.(s_idx)
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
         res.(ar.y) <- ar.x :: res.(ar.y)
       else if gb.partition.(ar.y) then
         (* ar.y in X, ar.x in Y *)
         res.(ar.x) <- ar.y :: res.(ar.x)
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
               res.(u) <- v :: res.(u)
        )
        gb.g.(u)
  done;
  (* y -> t pour tout y ∈ Y non couvert *)
  for v = 0 to n - 1 do
    if not gb.partition.(v) && not (est_couvert v c) then
      res.(v) <- t_idx :: res.(v)
  done;
  (* graphe orienté construit *)
  res


(* renvoie le tableau des prédécesseurs dans un parcours (quelconque) de g depuis s *)
let arbre_parcours g s =
  let n = Array.length g in
  let pred = Array.make n (-1) in
  let q = Queue.create () in
  pred.(s) <- s; (* racine a pour prédécesseur elle-même *)
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
let chemin g s t =
  failwith "TODO"



let couplage_maximum_biparti gb =
  failwith "TODO"


open Printf

(* --- Fonctions utilitaires pour afficher --- *)

let string_of_arete a = Printf.sprintf "(%d,%d)" a.x a.y

let string_of_couplage c =
  "[" ^ String.concat "; " (List.map string_of_arete c) ^ "]"

let print_couplage c =
  Printf.printf "%s\n" (string_of_couplage c)







(* --- Tests unitaires --- *)

let () =
  (* Test est_dans_couplage *)
  let c = [{x=0;y=1}; {x=2;y=3}] in
  assert (est_dans_couplage {x=1;y=0} c);
  assert (est_dans_couplage {x=2;y=3} c);
  assert (not (est_dans_couplage {x=0;y=2} c));
  Printf.printf "✓ est_dans_couplage OK\n";

  (* Test difference_symetrique *)
  let c1 = [{x=0;y=1}; {x=2;y=3}] in
  let c2 = [{x=2;y=3}; {x=4;y=5}] in
  let d = difference_symetrique c1 c2 in
  Printf.printf "difference_symetrique c1 c2 = %s\n" (string_of_couplage d);
  assert (List.length d = 2);  (* (0,1) et (4,5) *)
  Printf.printf "✓ difference_symetrique OK\n";

  (* Test est_couvert *)
  assert (est_couvert 0 c1);
  assert (est_couvert 3 c1);
  assert (not (est_couvert 5 c1));
  Printf.printf "✓ est_couvert OK\n";

  (* Test arbre_parcours et chemin *)
  let g = [| [1]; [0;2]; [1;3]; [2] |] in
  let pred = arbre_parcours g 0 in
  assert (pred.(3) <> -1);
  let p = chemin g 0 3 in
  Printf.printf "chemin 0->3 = %s\n" (string_of_couplage p);
  assert (p = [{x=0;y=1}; {x=1;y=2}; {x=2;y=3}]);
  Printf.printf "✓ chemin OK\n";

  (* Test couplage_maximum_biparti sur gb *)
  let cmax = couplage_maximum_biparti gb in
  Printf.printf "Couplage maximum gb = %s\n" (string_of_couplage cmax);
  assert (List.length cmax = 6);

  (* Test couplage_maximum_biparti sur gb2 *)
  let cmax2 = couplage_maximum_biparti gb2 in
  Printf.printf "Couplage maximum gb2 = %s\n" (string_of_couplage cmax2);
  assert (List.length cmax2 = 2);

  (* Test couplage_maximum_biparti sur gb_non_connexe *)
  let cmax3 = couplage_maximum_biparti gb_non_connexe in
  Printf.printf "Couplage maximum gb_non_connexe = %s\n" (string_of_couplage cmax3);
  assert (List.length cmax3 >= 2);

open Hashtbl

let n = 4

type etat = {
  grille : int array array;
  mutable i : int;
  mutable j : int;
  mutable h : int;
}

let affiche_etat etat =
  for i = 0 to n-1 do
    for j = 0 to n-1 do
      Printf.printf("%2d") etat.grille.(i).(j)
    done;
    print_newline ()
  done;
  print_newline ()


type direction = Haut | Bas | Gauche | Droite | Rien

let delta = function
  | Haut -> (-1, 0)
  | Bas -> (1, 0)
  | Gauche -> (0, -1)
  | Droite -> (0, 1)
  | Rien -> assert false

let string_of_direction = function
  | Haut -> "Haut"
  | Bas -> "Bas"
  | Gauche -> "Gauche"
  | Droite -> "Droite"
  | Rien -> "Aucun mouvement"



(* Graphe du Taquin *)

let mouvements_possibles etat =
  let mvt = ref [] in
  if etat.i > 0 then mvt := Haut :: !mvt;
  if etat.i < n - 1 then mvt := Bas :: !mvt;
  if etat.j > 0 then mvt := Gauche :: !mvt;
  if etat.j < n - 1 then mvt := Droite :: !mvt;
  !mvt

let distance i j value =
  let i_target = value / n in
  let j_target = value mod n in
  abs (i - i_target) + abs (j - j_target)

(* crée un tableau des positions de chaque tuile *)
let construit_positions e =
  Array.init (n * n) (fun _ -> (0,0)) |> fun positions ->
  for i = 0 to n-1 do
    for j = 0 to n-1 do
      let v = e.grille.(i).(j) in
      positions.(v) <- (i,j)
    done
  done;
  positions

let calcule_h e =
  let positions = construit_positions e in
  let h = ref 0 in
  for v = 0 to (n * n - 2) do  (* exclut la tuile vide *)

      let (i, j) = positions.(v) in
      if (i, j) <> (e.i, e.j) then begin
        h := !h + distance i j v end
      else
        h := !h
  done;
  e.h <- !h


let delta_h etat direction =
  let di, dj = delta direction in

  let ni = etat.i + di in
  let nj = etat.j + dj in

  let v = etat.grille.(ni).(nj) in  (* tuile déplacée *)

  let d_old = distance ni nj v in
  let d_new = distance etat.i etat.j v in

  d_new - d_old


let applique etat direction =
  let di, dj = delta direction in

  let ni = etat.i + di in
  let nj = etat.j + dj in

  let v = etat.grille.(ni).(nj) in

  (* calcul du changement d'heuristique *)
  let dh = delta_h etat direction in

  (* échange 0 <-> v dans la grille *)
  etat.grille.(etat.i).(etat.j) <- v;

  (* mise à jour de la position du vide *)
  etat.i <- ni;
  etat.j <- nj;

  (* mise à jour de l’heuristique *)
  etat.h <- etat.h + dh



let copie etat =
  {
    grille = Array.init n (fun i -> Array.copy etat.grille.(i));
    i = etat.i;
    j = etat.j;
    h = etat.h;
  }




(* Quelques exemples pour les tests *)

(* état cible *)
let final =
  let m = Array.make_matrix n n 0 in
  for i = 0 to n - 1 do
    for j = 0 to n - 1 do
      m.(i).(j) <- i * n + j
    done
  done;
  {grille = m; i = n - 1; j = n - 1; h = 0}


(* Génération d'état par une suite aléatoire de nb_moves mouvements, *)
(* en partant de l'état final. Renvoi un état s tel que *)
(*  d(initial, s) <= nb_moves (de manière évidente). *)
let random_etat nb_moves =
  let etat = copie final in
  for i = 0 to nb_moves - 1 do
    let moves = mouvements_possibles etat in
    let n = List.length moves in
    applique etat (List.nth moves (Random.int n))
  done;
  etat



(*Décommenter ce qui suit quand les fonctions nécessaires ont été codées !*)

(*
(* distance 10 *)
let dix =
  let moves = [Haut; Haut; Gauche; Gauche; Haut; Droite; Bas; Bas; Gauche; Gauche] in
  let etat = copie final in
  List.iter (applique etat) moves;
  etat
*)


(* distance 20 *)
let vingt =
  {grille =
    [| [|0; 1; 2; 3|];
      [|12; 4; 5; 6|];
      [|8; 4; 10; 11|];
      [|13; 14; 7; 9|] |];
   i = 1; j = 1; h = 14}

(* distance 30 *)
let trente =
  {grille =
     [| [|8; 0; 3; 1|];
       [|8; 5; 2; 13|];
       [|6; 4; 11; 7|];
       [|12; 10; 9; 14|] |];
   i = 0; j = 0; h = 22}

(* distance 40 *)
let quarante =
  {grille =
     [| [|7; 6; 0; 10|];
       [|1; 12; 11; 3|];
       [|8; 4; 2; 5|];
       [|8; 9; 13; 14|] |];
   i = 2; j = 0; h = 30}

(*
(* distance 50 *)
let cinquante =
  let s =
    {grille =
       [| [| 2; 3; 1; 6 |];
          [| 14; 5; 8; 4 |];
          [| 15; 12; 7; 9 |];
          [| 10; 13; 11; 0|] |];
     i = 2;
     j = 3;
     h = 0} in
  calcule_h s;
  s

(* distance 64 *)
let soixante_quatre =
  let s =
    {grille =
       [| [| 15; 14; 11; 7|];
          [| 5; 9; 12; 4|];
          [| 3; 10; 13; 8|];
          [| 2; 6; 0; 1|] |];
     i = 0;
     j = 0;
     h = 0} in
  calcule_h s;
  s
*)

(* Part 2 *)


let successeurs etat =
  let mvt = mouvements_possibles etat in
  List.map (fun d ->
    let e2 = copie etat in
    applique e2 d;
    e2
  ) mvt



let reconstruit (parents: ('a, 'a) Hashtbl.t) (x: 'a) =
  let out = ref [x] in

  while Hashtbl.find parents (List.hd !out) <> (List.hd !out) do
    out := Hashtbl.find parents (List.hd !out) :: !out
  done;

  !out



exception Aucun_chemin

(* Version corrigée de astar *)
let astar initial = failwith "TODO"







(* Algorithme IDA* *)

exception Found of int

let idastar_length initial =
  failwith "TODO"


let idastar initial =
  failwith "TODO"

let print_direction_vector t =
  for i = 0 to Vector.length t - 1 do
    Printf.printf "%s " (string_of_direction (Vector.get t i))
  done;
  print_newline ()

let print_idastar etat =
  match idastar etat with
  | None -> print_endline "No path"
  | Some t ->
    Printf.printf "Length %d\n" (Vector.length t);
    print_direction_vector t


(*
let main () =
  Printexc.record_backtrace true

let () = main ()
*)

(* Tests pour le jeu du Taquin *)

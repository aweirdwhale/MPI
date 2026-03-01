(* Beaucoup de fonctions sont déjà codées et vous sont fournies dans
   le module Othello. Allez voir othello.cmi pour leur interface. *)
open Othello

exception Invalid_argument


let compte_couleur e =
  let n = ref 0 and b = ref 0 in
    let len = Array.length e.plateau in
      for i = 0 to len - 1 do
        for j = 0 to len - 1 do
          match e.plateau.(i).(j) with
          | Noir -> n := !n + 1
          | Blanc -> b := !b + 1
          | Vide -> n := !n
        done
      done;
  (!n, !b)

let eval e =
  let (n,b) = compte_couleur e in
  n - b

let gagnant e =
  (* if not (est_terminal e) then raise Invalid_argument; *)
  let diff = eval e in if diff < 0 then Blanc else if diff > 0 then Noir else Vide
    (* match eval e with
    | s when s > 0 -> Noir
    | s when s < 0 -> Blanc
    | _ -> Vide *)


let minmax s0 =
  let memo = Hashtbl.create 676767 in
  let rec aux s =
    if Hashtbl.mem memo s then Hashtbl.find memo s
    else
      let v =
        if est_terminal s then eval s
        else
          let enfants_s = enfants s in
          let vals = List.map aux enfants_s in
          if s.joueur = Noir then List.fold_left max min_int vals
          else List.fold_left min max_int vals
      in
      Hashtbl.add memo s v; v
  in aux s0

let minmax_heur a b c = failwith "TODO"


let strat_minmax_heur a b c = failwith "TODO"
let alpha_beta a b c = failwith "TODO"
let strat_alpha_beta a b c = failwith "TODO"

let dict_minmax s0 j =
  let v = Hashtbl.create 10000 in
  let tab_asso = Hashtbl.create 10000 in

  let rec calcul_minmax e j_act =
    try Hashtbl.find v e with
    | Not_found ->
        let valeur =
          match enfants e with
          | [] -> eval e
          | l_enfants ->
              let next_j = if j_act = Noir then Blanc else Noir in
              let score_enfant = List.map (fun enf -> calcul_minmax enf next_j, enf) l_enfants in
              let (best_score, best_enf) =
                if j_act = Noir then
                  List.fold_left
                    (fun (sv, se) (v, e) -> if v > sv then (v, e) else (sv, se))
                    (max_int, List.hd l_enfants) score_enfant
                else
                  List.fold_left
                    (fun (sv, se) (v, e) -> if v < sv then (v, e) else (sv, se))
                    (min_int, List.hd l_enfants) score_enfant
              in
              if j = j_act then Hashtbl.add tab_asso e best_enf;
              best_score
        in
        Hashtbl.add v e valeur;
        valeur
  in
  calcul_minmax s0 Noir;
  tab_asso

(* Pour les tests/simulations :
   (on a besoin de donner les stratégies sous forme de fonction) *)

(** Joue suivant un dictionnaire *)
let strat dict =
  Hashtbl.find dict



let dict_minmax_heur h pmax s0 =
  failwith "TODO"


(** Juste compter les points *)
let heur_naive e =
  if est_terminal e then
    match gagnant e with
      | Noir -> max_int
      | Blanc -> min_int
      | Vide -> 0
      else eval e

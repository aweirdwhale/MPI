(* Mettre à la fin du fichier *)

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

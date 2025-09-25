let read_graph () =
  let n, p = Scanf.scanf "%d %d\n" (fun x y -> (x, y)) in
  let g = Array.make n [] in
  for i = 0 to p - 1 do
    Scanf.scanf "%d %d\n" (fun x y -> g.(x) <- y :: g.(x))
  done;
  g

type sommet = int
type graphe = sommet list array


(* Soit x un graphe ? *)
let transpose x =
  (* Classico parcours + Array.make en O(|S|) => O( |S| + |A| ) *)
  let len = Array.length x in
  let t = Array.make len [] in
    for u = 0 to len - 1 do
      (* Pour chaque voisin v de u, ajoute u à la liste des voisins de v dans le graphe transposé *)
      List.iter (fun v -> t.(v) <- u :: t.(v)) x.(u)
    done;
  t


let dfs_post x =
    (* Classico parcours + Array.make => O( |S| + |A| ) *)
    let n = Array.length x in
    let vus = Array.make n false in (*tableau de marquage*)
    let res = ref [] in (* sommets dans l'ordre post ouverture *)
    let rec dfs u =
      if not vus.(u) then begin
        vus.(u) <- true;
        List.iter dfs x.(u);
        res := u :: !res
      end
    in (*dfs complet*)
    for u = 0 to n - 1 do
      dfs u
    done;
    !res


let kosaraju x =
  let n = Array.length x in (*O(n)*)
  let post_order = dfs_post x in (*O(n + |A|)*)
  let tx = transpose x in
  let vus = Array.make n false in (*O(n)*)
  let composantes = ref [] in (*Arr des CC*)

  (* parcourt en profondeur le graphe transposé à partir de u + grab sommet cfc : O(|S|+|A|)*)
  let rec dfs_collect u comp =
    if not vus.(u) then begin
      vus.(u) <- true;
      let comp' = u :: comp in
      (* applique la fonction dfs_collect à chaque voisin v du sommet u dans le graphe transposé tx.*)
      List.fold_left (fun acc v -> dfs_collect v acc) comp' tx.(u)
    end else comp
  in

  (* pour tt u dans l'ordre post, on grab sa cfc si non visitée *)
  List.iter (fun u ->
    if not vus.(u) then
      let comp = dfs_collect u [] in
      composantes := comp :: !composantes
  ) post_order;

  List.rev_map (List.rev) !composantes



type litteral = P of int | N of int
type clause = litteral * litteral
type deuxcnf = clause list
type valuation = bool array



let eval_litt l valuation =
  match l with
  | P i -> valuation.(i)
  | N i -> not valuation.(i)

let eval deuxcnf valuation =
  List.for_all (fun (l1, l2) ->
    eval_litt l1 valuation || eval_litt l2 valuation
  ) deuxcnf

exception Last

let incremente_valuation valuation =
  let rec incr n =
    if n = (-1) then
      raise Last
    else if valuation.(n) then begin
      valuation.(n) <- false;
      incr (n-1)
    end else
      valuation.(n) <- true
  in
  incr (Array.length valuation - 1)


let brute_force x =

  (* Trouve l'indice maximal de variable dans la formule *)
  let max_var x =
    let max_litt l =
      match l with
      | P i -> i
      | N i -> i
    in

    List.fold_left (fun acc (l1, l2) ->
      max acc (max (max_litt l1) (max_litt l2))
    ) 0 x
    in
    let n = max_var x + 1 in
    let valuation = Array.make n false in
    let rec try_all () =
      if eval x valuation then Some (Array.copy valuation)
      else
        try
          incremente_valuation valuation;
          try_all ()
        with Last -> None
    in
    try_all ()



let graphe_de_cnf deux_cnf =
  (*
    Traduit une formule 2-CNF en un graphe d'implication.
    Pour chaque littéral Xi, le sommet est 2*i si Xi, 2*i+1 si non(Xi).
    Chaque clause (l1, l2) donne deux implications :
      - non(l1) -> l2
      - non(l2) -> l1
    On construit le graphe par liste d'adjacence.
  *)
  let max_var x =
    let max_litt l =
      match l with
      | P i -> i
      | N i -> i
    in
    List.fold_left (fun acc (l1, l2) ->
      max acc (max (max_litt l1) (max_litt l2))
    ) 0 x
  in
  let n = max_var deux_cnf + 1 in (*1/2 taille du graphe*)
  let g = Array.make (2*n) [] in

  let node_of_litt l =
    match l with
    | P i -> 2*i
    | N i -> 2*i+1
  in
  let node_of_not_litt l =
    match l with
    | P i -> 2*i+1
    | N i -> 2*i
  in

  List.iter (fun (l1, l2) ->
    (* non(l1) -> l2 *)
    g.(node_of_not_litt l1) <- node_of_litt l2 :: g.(node_of_not_litt l1);
    (* non(l2) -> l1 *)
    g.(node_of_not_litt l2) <- node_of_litt l1 :: g.(node_of_not_litt l2);
  ) deux_cnf;
  g


let satisfiable deuxcnf =
  let g = graphe_de_cnf deuxcnf in
  let cfc = kosaraju g in
  let len = Array.length g in (* on peut pas diviser par deux ici à cause du tab de marquage*)
  let marques = Array.make len (-1) in

  List.iteri (fun i composante -> begin
    List.iter (fun s -> begin
      if marques.(s) == -1 then
        marques.(s) <- i
      else
        failwith "What ?!"
      end
    ) composante;
    end
  ) cfc;

  let out = ref true in
  for i = 0 to (len / 2) do
    out := !out && (marques.(i) <> marques.(i+1))
  done;

  !out


(*
let main () =
  let g = read_graph () in
  let g_prime = transpose g in
  print_string "--- Test de lecture du graphe. ---\nLe sommet d'indice 0 du graphe a les voisins suivants :\n";
  List.iter (Printf.printf "%d \n") g.(0);
  print_newline ();
  List.iter (Printf.printf "%d \n") g_prime.(0);
  let parcours = dfs_post g in
  List.iter (fun s -> Printf.printf "%d " s) parcours;
  print_newline ();
  print_string "--- Test de kosaraju ---\n";
  let cfc = kosaraju g in
  Printf.printf "Nombre de composantes fortement connexes : %d\n" (List.length cfc);
  List.iteri (fun i comp ->
    Printf.printf "CFC #%d : " (i+1);
    List.iter (fun s -> Printf.printf "%d " s) comp;
    print_newline ()
  ) cfc

*)

(* let main =
  let deux_cnf_test = [(P 0, N 2); (P 1, P 3); (N 1, P 2); (N 2, P 3); (P 3, N 0)] in
  let deux_cnf_test_false = [(P 0, N 1); (P 0, P 1)]
in
let g = graphe_de_cnf deux_cnf_test in
print_string "--- Test de graphe_de_cnf ---\n";
for i = 0 to Array.length g - 1 do
  Printf.printf "Sommet %d : " i;
  List.iter (Printf.printf "%d ") g.(i);
  print_newline ()
done;
print_string "--- Test de satisfiable ---\n";
if satisfiable deux_cnf_test then
  Printf.printf "La formule est satisfiable.\n"
else
  Printf.printf "La formule n'est pas satisfiable.\n";
  if satisfiable deux_cnf_test_false then
    Printf.printf "La formule est satisfiable.\n"
  else
    Printf.printf "La formule n'est pas satisfiable.\n" *)


(* let () = main *)

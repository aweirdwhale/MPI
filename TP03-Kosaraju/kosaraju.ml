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



let eval_litt x = failwith "not implemented"
let eval x = failwith "not implemented"
exception Last
let incremente_valuation x = failwith "not implemented"
let brute_force x = failwith "not implemented"
let graphe_de_cnf x = failwith "not implemented"
let satisfiable x = failwith "not implemented"
let temoin x = failwith "not implemented"



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



let () = main ()

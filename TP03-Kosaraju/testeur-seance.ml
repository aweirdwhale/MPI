let () = Printf.printf "\n----- Testeur automatique -----\n\n"; flush(stdout)

(*TODO : mettre ici la liste des fonctions des élèves testées 
	 (pour que le testeur marche même si l'élève n'a pas traité la question) *)
let transpose x = failwith "not implemented"
let dfs_post x = failwith "not implemented"

let kosaraju x = failwith "not implemented"
let eval_litt x = failwith "not implemented"

let eval x = failwith "not implemented"
let incremente_valuation x = failwith "not implemented"
let brute_force x = failwith "not implemented"
let graphe_de_cnf x = failwith "not implemented"
let satisfiable x = failwith "not implemented"
let temoin x = failwith "not implemented"



(* ----- Code général du testeur automatique, ne pas modifier (rdv plus bas) ----- *)

(* variables globales utiles *)
let nb_test = ref 0
let reset r = r := 0
let score = ref 0.
let score_total = ref 0.

(* NE PAS REPETER CHEZ VOUS, nous sommes des professionnels *)
(* Application de fonction à un nombre quelconque d'argument *)
let [@warning "-20"] rec apply f arguments = match arguments with
	[] -> failwith "no argument"
  | [h] -> (Obj.magic f) h
  | h::t -> apply ((Obj.magic f) h) t


(* Tests générique de fonction, où l'on accorde les points sur le critère d'un booléen.
	Utile pour tester autre chose que l'égalité de la sortie d'une fonction avec une valeur prédéterminée.*)
let test_bool func_name cor_use b nb_points =
	incr nb_test;
	(*Printf.printf "%s : " func_name;*)
	if b && not (!cor_use) then begin
		Printf.printf "\027[32m(%.2f/%.2f) [OK]\027[0m" nb_points nb_points;
		score := !score +. nb_points
	end
	else if not b then begin
		Printf.printf "\027[31m(%.2f/%.2f) [KO] \027[0m" 0. nb_points;
		Printf.printf " La fonction \"%s\" a échoué sur le test n°%d." func_name (!nb_test);
	end
	else begin (* Détection d'utilisation de corrigé *)
		Printf.printf "\027[33m(%.2f/%.2f) [KO] \027[0m" 0. nb_points;
		Printf.printf " La fonction \"%s\" utilise son corrigé sur le test n°%d." func_name (!nb_test);
	end;
	print_newline ();
	cor_use := false;
	score_total := !score_total +. nb_points

(* Test de fonction à un nombre quelconque d'argument *)
(* Entrées : le nom de la fonction à tester, 
	 la liste de ses arguments,
	 la sortie attendue
	 et le nombre de points accordés à ce test*)
let test func_name func cor_use input expected_output nb_points =
	test_bool func_name cor_use (apply func input = expected_output) nb_points


let non_implemente func_name nb_points =
	Printf.printf "\027[36m(0/%.2f) La fonction \"%s\" n'est pas implémentée.\n \027[0m" nb_points func_name;
	score_total := !score_total +. nb_points

let plantage func_name nb_points =
	Printf.printf "\027[35m(0/%.2f) La fonction \"%s\" plante à l'exécution.\n \027[0m" nb_points func_name;
	score_total := !score_total +. nb_points






(* TODO : modifier le nom du module élève à ouvrir. *)
open Kosaraju
(*Ici le fichier testé doit s'appeler "kosaraju.ml", sinon adapter le nom du module à ouvrir.
	Le fichier corrige.ml fournit les fonctions demandés aux élèves (pour qu'ils puissent sauter une question), 
	mais ces fonctions mettent un booléen "cor_use" à true et les points ne seront pas accordés pour la questions. *)
(* Pour compiler : ocamlopt [fichier des élèves].ml testeur.ml -o test
ATTENTION : ne marche que si on compile ! *)

let name_use = ref ""
let cor_use = ref false


(* ----- Fonction utiles pour les test : ----- *)

(* Mettre ici les fonctions et variables maison appelées par vos tests *)
(* Exemple de fonction utile : test d'égalités entre les éléments de deux listes *)
let equal_set l1 l2 =
	let l1 = List.sort compare l1 in
	let l2 = List.sort compare l2 in
	l1 = l2

let equal_list_of_list l1 l2 =
	let n = List.length l1 in
	if List.length l2 <> n then false
	else begin
		let l1_ordre = List.map (List.sort compare) l1 in
		let l2_ordre = List.map (List.sort compare) l2 in 
		equal_set l1_ordre l2_ordre
	end

let	equal_graphes g1 g2 =
	Array.for_all2 equal_set g1 g2


let eval_litt__testeur l v =
	match l with
	| P i -> v.(i)
	| N i -> not v.(i)

let rec eval__testeur f v =
	match f with
	| [] -> true
	| (l, l') :: tail ->
		(eval_litt__testeur l v || eval_litt__testeur l' v) && eval__testeur tail v

			
let est_tri_topologique l g =
	let n = Array.length g in
	let vus = Array.make n false in
	let rec verifie_liste = function
		[] -> true
		| h :: t -> if vus.(h) = true then false(* garantie l'injectivité, donc la bijectivité avec les cardinaux *)
			else begin 
				vus.(h) <- true;
				List.for_all (fun v -> not vus.(v)) g.(h)
				&& verifie_liste t
			end
	in List.length l = n && verifie_liste l

(* NE MARCHE PAS !!!
exception Incorrect
let est_post_dfs_decroissant l g =
	let n = Array.length g in
	let deja_vus = Array.make n false in
	let reste = ref l in
	let next = ref (List.hd l) in
	let rec parcours_guide s =
		if deja_vus.(s) then raise Incorrect;(* garantie l'injectivité, donc la bijectivité avec les cardinaux *)
		deja_vus.(s) <- true;
		while (List.mem (!next) g.(s)) do
			let v = !next in
			reste := List.tl (!reste);
			next := List.hd (!reste);
			parcours_guide v
		done;
		(*if not (List.for_all (fun v -> deja_vus.(v)) g.(s)) then raise Incorrect;*)
		()
	in try
		parcours_guide (!next);
		List.length l = n (* ici on a terminé le parcours sans rencontrer d'"erreur" vis-à-vis de l'ordre en profondeur*)
		&& Array.for_all (fun b -> true) deja_vus 
	with Incorrect -> false
*)

let g_vide = [| |]

let (g_1_v : int list array) = [| [] |]

let g_no_edge = [| []; []|]

let g1 = (* Graphe Exemple du cours *)
	let n = 12 in
	let g = Array.make n [] in
	g.(0) <- [1;2]; g.(1) <- [2;3;4]; g.(2) <- [];
	g.(3) <- [0;5]; g.(4) <- [1;2]; g.(5) <- [10];
	g.(6) <- [1;9]; g.(7) <- [8]; g.(8) <- [6];
	g.(9) <- [7;10]; g.(10) <- [11]; g.(11) <- [5];
	g

let g__arxiv = 
	let ic = Scanf.Scanning.open_in "arxiv.txt" in
	let n, p = Scanf.bscanf ic "%d %d\n" (fun x y -> (x, y)) in
	let g = Array.make n [] in
	Printf.printf "Lecture du graphe arxiv.txt...\n"; flush(stdout);
	for i = 0 to p - 1 do
		Scanf.bscanf ic "%d %d\n" (fun x y -> g.(x) <- y :: g.(x))
	done;
	Scanf.Scanning.close_in ic;
	g

let g_dag = [| []; [2;3]; [0;4]; [5;6]; [6]; []; [9]; [8]; [9]; [10]; [] |]

let g_exemple = [| [1]; [0]; [4]; [2;7]; [3;1]; [6;7]; [5;7]; [5;6] |]



let exemple_f =
  [(P 0, N 2); (P 1, P 3); (N 1, P 2); (N 2, P 3); (P 3, N 0)]

let phi__testeur =
	[(P 0, N 1); (N 0, P 1); (N 0, N 1); (P 0, N 2)]

let phi__testeur2 =
	[(P 0, N 1); (N 0, P 1); (N 0, N 1); (P 0, N 2); (P 1, P 3)]

let phi__unsat =
	[(P 0, P 1); (P 1, N 0); (P 0, N 1); (N 0, N 1);]

let phi__coq =
	[(P 0, P 1); (P 0, N 2); (N 1, P 2)]


(* Fonctions du corrigé :*)

let transpose__testeur g =
  let n = Array.length g in
  let gprime = Array.make n [] in
  for s = 0 to n - 1 do
    let ajoute_voisin v = gprime.(v) <- s :: gprime.(v) in
    List.iter ajoute_voisin g.(s)
  done;
  gprime

let dfs_post__testeur g =
  let n = Array.length g in
  let post = ref [] in
  let vus = Array.make n false in
  let rec explore x =
    if not vus.(x) then begin
      vus.(x) <- true;
      List.iter explore g.(x);
      post := x :: !post
    end in
  for x = 0 to n - 1 do
    explore x
  done;
  !post


let kosaraju__testeur g =
  (* initialisation : *)
  let n = Array.length g in
  let order = dfs_post__testeur g in
  let composantes = ref [] in
  let vus = Array.make n false in
  let gprime = transpose__testeur g in

  (* parcours (dfs récursif) de la CFC de x dans le miroir de G *)
  let rec ajouter cfc x =
    if not vus.(x) then begin
      vus.(x) <- true;
      List.iter (ajouter cfc) gprime.(x);
      cfc := x :: !cfc
    end 
  in

  (* calcul successif des composantes connexes (parcours de "order")*)
  let cfc = ref [] in
  let traite_sommet x =
    if not vus.(x) then begin
      cfc := [];
      ajouter cfc x;
      composantes := !cfc :: !composantes (* on ajoute le CONTENU de la réf, pas de problème de partage *)
    end 
  in
  List.iter traite_sommet order;

  (* renvoi dans l'ordre topologique : *)
  List.rev (!composantes)

let cfc__testeur g =
	let n = Array.length g in
	let cfc = kosaraju__testeur g in
	let res = Array.make n (-1) in
	let traite_cfc i c =
		let traite_sommet s = res.(s) <- i
		in List.iter traite_sommet c
	in List.iteri traite_cfc cfc;
	res


let est_tri_topologique_modulo_cfc l g =
	let comp = cfc__testeur g in
	let n = Array.length g in
	let vus = Array.make n false in
	let rec verifie_liste = function
		[] -> true
		| h :: t -> if vus.(h) = true then false(* garantie l'injectivité, donc la bijectivité avec les cardinaux *)
			else begin 
				vus.(h) <- true;
				List.for_all (fun v -> not vus.(v) || comp.(v) = comp.(h)) g.(h)
				&& verifie_liste t
			end
	in List.length l = n && verifie_liste l

let var__testeur = function
| P i | N i -> i

let rec max_var__testeur = function
  | [] -> -1
  | (l, l') :: f -> max (max_var__testeur f) (max (var__testeur l) (var__testeur l'))

let neg__testeur = function
  | P i -> N i
  | N i -> P i

let graphe_de_cnf__testeur f =
	let n = max_var__testeur f in
	let g = Array.make (2 * n + 2) [] in
	let somm = function
		| P i -> 2 * i
		| N i -> 2 * i + 1 in
	let ajoute_clause (l, l') =
		g.(somm (neg__testeur l)) <- somm l' :: g.(somm (neg__testeur l));
		g.(somm (neg__testeur l')) <- somm l :: g.(somm (neg__testeur l')) in
	List.iter ajoute_clause f;
	g



(* ----- TESTS ----- *)
(* Tout d'abord, copier-coller la structure et la modifier accordément, puis écrire les tests comme suit :
testf [liste des arguments] [résultat attendu] [nb_de_points que vaut le test]
ou
testb [booléen vrai/faux qui doit être vrai si l'élève a réussi] [nb_de_points que vaut le test]
*)

(*tests merge*)
let func_name = "transpose" (*TODO : mettre le nom de la fonction *)
let () = reset nb_test
let () = try
	let f = transpose in (* TODO : mettre la fonction élève testée *)
	name_use := func_name;
	let [@warning "-26"] testf = test func_name f cor_use in
	let [@warning "-26"] testb = test_bool func_name cor_use in
	Printf.printf "\n----- Test de %s -----\n" func_name;

	(*tests :*)
	testb (equal_graphes (transpose g1) [| [3]; [0;4;6]; [0;1;4]; [1]; [1]; [3;11]; [8]; [9]; [7]; [6]; [5;9]; [10]|]) 0.5;
	testf [g_vide] [||] 0.125;
	testf [g_1_v] [| [] |] 0.125;
	testf [g_no_edge] [| []; [] |] 0.25;
	testb (equal_graphes (transpose g_exemple) (transpose__testeur g_exemple)) 1.;
	testb (equal_graphes (transpose g_dag) (transpose__testeur g_dag)) 1.;
	testb (equal_graphes (transpose g__arxiv) (transpose__testeur g__arxiv)) 1.

with Failure s when s = "not implemented" -> non_implemente func_name 4.
	| _ -> plantage func_name 4.


(*tests dfs_post*)
(*ATTENTION, pour cette fonction je teste juste que ce que renvoie les élèves est bien un ordre topologique,
c'est tout ce dont on a besoin pour Kosaraju, peu importe qu'il le trouve avec un autre algo (j'en doute).*)
let func_name = "dfs_post" (*TODO : mettre le nom de la fonction *)
let () = reset nb_test
let () = try
	let f = dfs_post in (* TODO : mettre la fonction élève testée *)
	name_use := func_name;
	let [@warning "-26"] testf = test func_name f cor_use in
	let [@warning "-26"] testb = test_bool func_name cor_use in
	Printf.printf "\n----- Test de %s -----\n" func_name;

	(* que sur quelques graphes ACYCLIQUES ici.\nIl est délicat de tester si votre liste %est un ordre correct parmi TOUS les parcours en profondeurs récursifs possibles d'un graphe quelconque.\n/!\\Testez bien attentivement cette fonction CHEZ VOUS pour la suite, en connaissant l'ordre dans lequel vous avez fait votre parcours ! /!\\ \n; *)

	(*tests :*)
	testb (est_tri_topologique_modulo_cfc (f g1) g1) 1.;
	testb (est_tri_topologique (f g_vide) g_vide) 0.125;
	testb (est_tri_topologique (f g_1_v) g_1_v) 0.125;
	testb (est_tri_topologique (f g_no_edge) g_no_edge) 0.25;
	testb (est_tri_topologique (f g_dag) g_dag) 1.;
	testb (est_tri_topologique_modulo_cfc (f g_exemple) g_exemple) 1.;
	testb (est_tri_topologique_modulo_cfc (f g__arxiv) g__arxiv) 1.5;

	Printf.printf "ATTENTION : Les tests de cette fonction ne sont pas exhaustifs.\nC'est possible de passer tous les tests ci-dessus avec une fonction incorrecte.\nIl est délicat de retrouver l'ordre de parcours effectué à partir de seules fins de fermeture. Ici, je teste un ensemble de propriétés nécessaires mais non suffisantes. Testez bien attentivement chez vous pour la suite !\n"
	

with Failure s when s = "not implemented" -> non_implemente func_name 5.
	| _ -> plantage func_name 5.


(*tests kosaraju*)
let func_name = "kosaraju" (*TODO : mettre le nom de la fonction *)
let () = reset nb_test
let () = try
	let f = kosaraju in (* TODO : mettre la fonction élève testée *)
	name_use := func_name;
	let [@warning "-26"] testf = test func_name f cor_use in
	let [@warning "-26"] testb = test_bool func_name cor_use in
	Printf.printf "\n----- Test de %s -----\n" func_name;

	(*tests :*)
	testb (equal_list_of_list (f g1) (kosaraju__testeur g1)) 1.25;
	testb (equal_list_of_list (f g_vide) (kosaraju__testeur g_vide)) 0.25;
	testb (equal_list_of_list (f g_1_v) (kosaraju__testeur g_1_v)) 0.25;
	testb (equal_list_of_list (f g_no_edge) (kosaraju__testeur g_no_edge)) 0.5;
	testb (equal_list_of_list (f g_exemple) (kosaraju__testeur g_exemple)) 1.25;
	testb (equal_list_of_list (f g_dag) (kosaraju__testeur g_dag)) 1.;
	testb (equal_list_of_list (f g__arxiv) (kosaraju__testeur g__arxiv)) 1.5;

with Failure s when s = "not implemented" -> non_implemente func_name 6.
	| _ -> plantage func_name 6.



(*tests eval_litt*)
let func_name = "eval_litt" (*TODO : mettre le nom de la fonction *)
let () = reset nb_test
let () = try
	let f = eval_litt in (* TODO : mettre la fonction élève testée *)
	name_use := func_name;
	let [@warning "-26"] testf = test func_name f cor_use in
	let [@warning "-26"] testb = test_bool func_name cor_use in
	Printf.printf "\n----- Test de %s -----\n" func_name;

	(*tests :*)
	testf [ P 3; Obj.magic [| true; true; false; false; true |] ] false 0.125;
	testf [ P 1; Obj.magic [| true; true; false; false; true |] ] true 0.125;
	testf [ N 3; Obj.magic [| true; true; false; false; true |] ] true 0.125;
	testf [ N 1; Obj.magic [| true; true; false; false; true |] ] false 0.125;

with Failure s when s = "not implemented" -> non_implemente func_name 0.5
	| _ -> plantage func_name 0.5

(*tests eval*)
let func_name = "eval" (*TODO : mettre le nom de la fonction *)
let () = reset nb_test
let () = try
	let f = eval in (* TODO : mettre la fonction élève testée *)
	name_use := func_name;
	let [@warning "-26"] testf = test func_name f cor_use in
	let [@warning "-26"] testb = test_bool func_name cor_use in
	Printf.printf "\n----- Test de %s -----\n" func_name;

	(*tests :*)
	testf [ exemple_f; Obj.magic [| true; false; false; true |] ] true 0.25;
	testf [ phi__testeur ; Obj.magic [| false; false; false |] ] true 0.125;
	testf [ phi__testeur2 ; Obj.magic [| true; true; false; true |] ] false 0.125;
	testf [ phi__unsat ; Obj.magic [| true; true |] ] false 0.25;
	testf [ phi__coq ; Obj.magic [| true; true; false |] ] false 0.25;

with Failure s when s = "not implemented" -> non_implemente func_name 1.
	| _ -> plantage func_name 1.


(*tests incremente_valuation*)
let func_name = "incremente_valuation" (*TODO : mettre le nom de la fonction *)
let () = reset nb_test
let () = try
	let f = incremente_valuation in (* TODO : mettre la fonction élève testée *)
	name_use := func_name;
	let [@warning "-26"] testf = test func_name f cor_use in
	let [@warning "-26"] testb = test_bool func_name cor_use in
	Printf.printf "\n----- Test de %s -----\n" func_name;

	(*tests :*)
	let v = [| false; false; false|] in
	f v;
	testb (v = [| false; false; true |]) 0.5;

	let v = [| true; false; false; true |] in
	f v;
	testb (v = [| true; false; true; false |]) 0.25;

	let v = [| false; false; true; true; |] in
	f v;
	testb (v = [| false; true; false; false |]) 0.25;

	let v = [| true; true; true; false |] in
	f v;
	testb (v = [| true; true; true; true |]) 0.5;

	let v = [| true; true; true; true |] in
	try
		f v;
	with Last -> testb true 0.5;


with Failure s when s = "not implemented" -> non_implemente func_name 2.
	| _ -> plantage func_name 2.


(*tests brute_force*)
let func_name = "brute_force" (*TODO : mettre le nom de la fonction *)
let () = reset nb_test
let () = try
	let f = brute_force in (* TODO : mettre la fonction élève testée *)
	name_use := func_name;
	let [@warning "-26"] testf = test func_name f cor_use in
	let [@warning "-26"] testb = test_bool func_name cor_use in
	Printf.printf "\n----- Test de %s -----\n" func_name;

	(*tests :*)
	let v_opt = f exemple_f in
	begin match v_opt with None -> testb false 0.25
		| Some v -> testb (eval__testeur exemple_f v = true) 0.25
	end;
	let v_opt = f phi__testeur in
	begin match v_opt with None -> testb false 0.125
		| Some v -> testb (eval__testeur phi__testeur v = true) 0.125
	end;
	let v_opt = f phi__testeur2 in
	begin match v_opt with None -> testb false 0.125
		| Some v -> testb (eval__testeur phi__testeur2 v = true) 0.125
	end;
	let v_opt = f phi__unsat in
	begin match v_opt with None -> testb true 0.75
		| Some v -> testb (eval__testeur phi__unsat v = true) 0.75
	end;
	;
	let v_opt = f phi__coq in
	begin match v_opt with None -> testb false 0.25
		| Some v -> testb (eval__testeur phi__coq v = true) 0.25
	end

with Failure s when s = "not implemented" -> non_implemente func_name 1.5
	| _ -> plantage func_name 1.5




	
(* ----- RESULTAT ----- *)
let () = Printf.printf "\nScore obtenu : %.2f / %.2f\n" !score !score_total(*;
let oc = open_out_gen [Open_append] 666 "../Notes.txt" in
Printf.fprintf oc "%.3f\n" !score;
close_out oc*)



(* Jeu de Chomp *)

type controle = J1 | J2
type config = {joueur : controle; tablette : int array; q : int}
  (** Pour représenter une tablette, on stocke la longueur de chaque ligne
     et la longueur initiale [q] *)

type graphe = (config, config list) Hashtbl.t
  (** Un graphe est représenté par le tableau associatif de ses listes d'adjacences *)

exception Invalide
exception Empty

(** Renvoie le prochain joueur *)
let next j =
  match j with
  | J1 -> J2
  | J2 -> J1
  (* | _ -> Failwith "Ce goulu gourmand n'est pas un joueur !"  *)


let print_tablette c =
  let p = Array.length c.tablette in
  for i = 0 to p-1 do
    for j = 0 to c.tablette.(i)-1 do
      print_string "🟫"
    done;
    for j = c.tablette.(i) to c.q-1 do
      print_string "⬜"
    done;
    print_newline ()
  done


let config_initiale p q =
  let choco = Array.make p q in
  {joueur = J1; tablette = choco; q = q}


(** Vérifie si la tablette de [c] est valide *)
let tablette_valide c =
  let t = c.tablette in
  let p = Array.length t in
  let q = c.q in
  let valide = ref true in

  (* bornes *)
  for i = 0 to p - 1 do
    if t.(i) < 0 || t.(i) > q then
      valide := false
  done;

  (* décroissance *)
  for i = 0 to p - 2 do
    if t.(i) < t.(i + 1) then
      valide := false
  done;

  !valide


(** Mange la case de coordonées [(i,j)] dans [c].
    Renvoie la nouvelle config, ou lève [Invalide] si le coup est Invalide *)
    (*
    ℓ′ᵢ = ℓᵢ              si i < x
    ℓ′ᵢ = min(ℓᵢ, y)      si i ≥ x
     *)
let chomp x y c =

  let p = Array.length c.tablette in
  let new_tablette = Array.copy c.tablette in

  for i = x to p - 1 do
    new_tablette.(i) <- min new_tablette.(i) y
  done;

  let chomped = {
    joueur = next c.joueur;
    tablette = new_tablette;
    q = c.q
  } in

  if not (tablette_valide chomped) || x<0 ||y<0
  then raise Invalide
  else chomped


(** Construit graphe du Chomp [p*q] *)
let construit_graphe (p : int) (q : int) : graphe =
  let g = Hashtbl.create (p*q) in (* Dénombrer la valeur exacte est un travail
                                   d'étudiants en maths; pas de prof d'info *)
  let rec parcours_remplit_g (c : config) =
    (** Parcours le graphe pour remplir [g]
        NB : PAS un DFS !! *)
    let p = Array.length c.tablette in
    for x = 0 to p-1 do
      for y = 0 to c.tablette.(x)-1 do
        try
          let c' = chomp x y c in
          Hashtbl.replace g c (c' :: (Hashtbl.find g c));
          if not (Hashtbl.mem g c') then begin
            Hashtbl.replace g c' [];
            parcours_remplit_g c'
          end
        with Invalide -> ()
      done
    done
  in
  let c = config_initiale p q in
  Hashtbl.replace g c [];
  parcours_remplit_g c;
  g


(** Transpose un graphe [g] représenté par une Hashtbl de listes d'adjacences *)
let transpose_graphe (g : graphe) =
  let tG = Hashtbl.create 10 in
  let rec renverse_adja c l = match l with
    (** Ajoute à tG  [c', c] tel que [c, c'] arc de [g]. Notez que [c] est fixé ici. *)
    | [] -> ()
    | c' :: q -> begin
                 Hashtbl.replace tG c' (c :: Hashtbl.find tG c');
                 renverse_adja c q
                 end
  in
  Hashtbl.iter (fun c' _ -> Hashtbl.replace tG c' []) g;
  Hashtbl.iter renverse_adja g;
  tG

(* let est_terminal c = *)



(** Renvoie les états terminaux gagnants du joueur [j] au Chomp [p*q] *)
let terminaux p q j =
    [{
      joueur = j;   (* celui qui doit jouer perd *)
      tablette = Array.make p 0;
      q = q
    }]





(** Si [c] est une configuration et que [l] est la liste de ses voisins,
    stocke le degré de [c] dans la Hashtbl [nb_voisins]. *)
let stocke_deg nb_voisins c l =
  (* TODO *) ()

(** Associe [c] à [true] dans [attr] *)
let marque attr c =
  (* TODO *) ()

(** Diminue de 1 la valeur associée à [c] dans [nb_voisins].
    C'est à dire en pseudo-code : [nb_voisins[c] -= 1] *)
let decremente nb_voisins c =
  let v = Hashtbl.find nb_voisins c in
  Hashtbl.replace nb_voisins c (v - 1)


let calcul_attracteurs p q j =

  let g = construit_graphe p q in
  let gt = transpose_graphe g in

  let nb_voisins = Hashtbl.create 1000 in
  let attr = Hashtbl.create 1000 in
  let file = Queue.create () in

  (* step 1 : initialiser nb_voisins *)
  Hashtbl.iter
    (fun c succs -> stocke_deg nb_voisins c succs)
    g;

  (* step 2 : initialiser attr *)
  Hashtbl.iter
    (fun c _ -> Hashtbl.add attr c false)
    g;

  (* initialisation : vrais terminaux du graphe *)
  Hashtbl.iter
    (fun c _ ->
       let t = c.tablette in
       let p = Array.length t in
       let est_terminal =
         t.(0) = 1 &&
         (p = 1 ||
          Array.for_all (fun x -> x = 0)
            (Array.sub t 1 (p - 1)))
       in
       if est_terminal && c.joueur = next j then begin
         marque attr c;
         Queue.add c file
       end)
    g;

  (* step 3 : propagation *)
  while not (Queue.is_empty file) do
    let c = Queue.pop file in
    let preds =
      try Hashtbl.find gt c
      with Not_found -> []
    in
    List.iter
      (fun p ->
         if not (Hashtbl.find attr p) then
           if p.joueur = j then begin
             marque attr p;
             Queue.add p file
           end else begin
             decremente nb_voisins p;
             if Hashtbl.find nb_voisins p = 0 then begin
               marque attr p;
               Queue.add p file
             end
           end)
      preds
  done;

  attr

type 'a regex =
  | Empty
  | Eps
  | Letter of 'a
  | Union of 'a regex * 'a regex
  | Concat of 'a regex * 'a regex
  | Star of 'a regex


(* ----- Affichages de regex ----- *)

(* Parses a string into an int regex.
 * The alphabet is assumed to be a subset of a..z, and is converted
 * to [0..25] (a -> 0, b -> 1...),
 * Charcater '&' stands for "epsilon", and character '#' for "empty".
 * Spaces are ignored, and the usual priority rules apply.
 *)

let parse string =
  let open Printf in
  let to_int c =
    assert ('a' <= c && c <= 'z');
    int_of_char c - int_of_char 'a' in
  let s = Stream.of_string string in
  let rec peek () =
    match Stream.peek s with
    | Some ' ' -> Stream.junk s; peek ()
    | Some c -> Some c
    | None -> None in
  let eat x =
    match peek () with
    | Some y when y = x -> Stream.junk s; ()
    | Some y -> failwith (sprintf "expected %c, got %c" x y)
    | None -> failwith "incomplete" in
  let rec regex () =
    let t = term () in
    match peek () with
    | Some '|' -> eat '|'; Union (t, regex ())
    | _ -> t
  and term () =
    let f = factor () in
    match peek () with
    | None | Some ')' | Some '|' -> f
    | _ -> Concat (f, term ())
 and factor () =
    let rec aux acc =
      match peek () with
      | Some '*' -> eat '*'; aux (Star acc)
      | _ -> acc in
    aux (base ())
  and base () =
    match peek () with
    | Some '(' -> eat '('; let r = regex () in eat ')'; r
    | Some '&' -> eat '&'; Eps
    | Some '#' -> eat '#'; Empty
    | Some (')' | '|' | '*' as c) -> failwith (sprintf "unexpected '%c'" c)
    | Some c -> eat c; Letter (to_int c)
    | None -> failwith "unexpected end of string" in
  let r = regex () in
  try Stream.empty s; r
  with _ -> failwith "trailing ')' ?"


let rec string_of_regex e =
  let open Printf in
  let to_char i =
    char_of_int (i + int_of_char 'a') in
  let priorite = function
    | Union (_, _) -> 1
    | Concat (_, _) -> 2
    | Star _ -> 3
    | _ -> 4 in
  let parenthese expr parent =
    if priorite expr < priorite parent then
      sprintf "(%s)" (string_of_regex expr)
    else string_of_regex expr in
  match e with
  | Empty -> "#"
  | Eps -> "&"
  | Letter x -> sprintf "%c" (to_char x)
  | Union (f, f') -> sprintf "%s|%s" (parenthese f e) (parenthese f' e)
  | Concat (f, f') -> sprintf "%s%s" (parenthese f e) (parenthese f' e)
  | Star f -> sprintf "%s*" (parenthese f e)




(* ----- Affichage d'automates (graphviz) ----- *)

type state = int

type nfa =
  {delta : state list array array;
  accepting : bool array}

let graphviz_nfa a filename =
  let open Printf in
  let n = Array.length a.delta in
  let m = Array.length a.delta.(0) in
  let out = open_out filename in
  fprintf out "digraph a {\nrankdir = LR;\n";
  (* noms des états *)
  let lettre i = String.make 1 (char_of_int (i + int_of_char 'a')) in
  (* etats *)
  for q = 0 to n - 1 do
    let shape = if a.accepting.(q) then "doublecircle" else "circle" in
    fprintf out "node [shape = %s, label = %d] %d;\n" shape q q
  done;
  (* etat initial *)
  fprintf out "node [shape = point]; I\n";
  fprintf out "I -> %i;\n" 0;
  (* transitions *)
    let labels = Array.make_matrix n n [] in
  for q = 0 to n - 1 do
    for x = m - 1 downto 0 do
      let ajoute q' = labels.(q).(q') <- lettre x :: labels.(q).(q') in
      List.iter ajoute a.delta.(q).(x)
    done
  done;
  for q = 0 to n - 1 do
    for q' = 0 to n - 1 do
      let s = String.concat "," labels.(q).(q') in
      if s <> "" then
        fprintf out "%i -> %i [ label = \"%s\" ];\n" q q' s
    done
  done;
  fprintf out "}\n";
  close_out out

let genere_pdf input_file output_file =
  Sys.command (Printf.sprintf "dot -Tpdf %s -o %s" input_file output_file)


type dfa =
  {delta_d : state array array;
  accepting_d : bool array}

let to_nfa a =
  let n = Array.length a.delta_d in
  let m = Array.length a.delta_d.(0) in
  let delta = Array.make_matrix n m [] in
  for q = 0 to n - 1 do
    for x = 0 to m - 1 do
      delta.(q).(x) <- [a.delta_d.(q).(x)]
    done
  done;
  {delta = delta; accepting = a.accepting_d}

let graphviz_dfa a = graphviz_nfa (to_nfa a)





(* ----- Exemples de regex à utiliser ----- *)


let e1 = parse "(a|b)#" (* langage vide *)
let e2 = parse "(b|ca)*d"
let e3 = parse "&|a(b|ca)*"
let e4 = parse "baba"

(* exemple d'affichage de regex :*)
let () = print_string "\n----- affichage des regex :----\ne1 = ";
  print_string (string_of_regex e1);
  print_string "\ne2 = ";
  print_string (string_of_regex e2);
  print_string "\ne3 = ";
  print_string (string_of_regex e3);
  print_newline ()

let exemple =
  Concat (Union (Letter 'c', Letter 'a'),
          Star (Concat (Letter 'b',
                        Union (Letter 'a', Letter 'c'))))








(* ----- A vous de jouer ! ----- *)


let rec merge u v =
  match u, v with
  | [], l -> l
  | l, [] -> l
  | x1 :: r1, x2 :: r2 ->
      if x1 < x2 then
        x1 :: merge r1 v
      else if x1 > x2 then
        x2 :: merge u r2
      else
        x1 :: merge r1 r2

let rec is_empty = function
  | Empty -> true
  | Eps -> false
  | Letter _ -> false
  | Union (r1, r2) ->
      is_empty r1 && is_empty r2
  | Concat (r1, r2) ->
      is_empty r1 || is_empty r2
  | Star _ -> false


let rec contains_epsilon = function
  | Empty -> false
  | Eps -> true
  | Letter _ -> false
  | Union (r1, r2) ->
      contains_epsilon r1 || contains_epsilon r2
  | Concat (r1, r2) ->
      contains_epsilon r1 && contains_epsilon r2
  | Star _ -> true

let rec prefix x =
  if is_empty x then [] else
  match x with
    | Empty -> []
    | Eps -> []
    | Letter l -> [l]
    | Union (r1, r2) -> merge (prefix r1) (prefix r2)
    | Concat (r1, r2) ->
        if contains_epsilon r1 then
          merge (prefix r1) (prefix r2)
        else
          prefix r1
    | Star e -> prefix e


let rec suffix x =
  if is_empty x then [] else
  match x with
    | Empty -> []
    | Eps -> []
    | Letter l -> [l]
    | Union (r1, r2) -> merge (suffix r1) (suffix r2)
    | Concat (r1, r2) ->
        if contains_epsilon r2 then
          merge (suffix r1) (suffix r2)
        else
          suffix r2
    | Star e -> suffix e

let rec combine l1 l2 =
  match l1 with
  | [] -> []
  | a :: r1 ->
      let rec aux a l2 =
        match l2 with
        | [] -> []
        | b :: r2 -> (a,b) :: aux a r2
      in
      aux a l2 @ combine r1 l2

let rec factor x =
  match x with
  | Empty -> []
  | Eps -> []
  | Letter _ -> []
  | Union (r1, r2) -> merge (factor r1) (factor r2)
  | Concat (r1, r2) ->
      let suf = suffix r1 in
      let pre = prefix r2 in
      let pairs = combine suf pre in
      merge (factor r1) (merge (factor r2) pairs)
  | Star r ->
      let suf = suffix r in
      let pre = prefix r in
      let pairs = combine suf pre in
      merge (factor r) pairs

let rec number_of_letters x = match x with
      |Eps -> 0
      |Empty -> 0
      |Letter l -> 1
      |Union(r1,r2) -> number_of_letters r1 + number_of_letters r2
      |Concat(r1,r2) -> number_of_letters r1 + number_of_letters r2
      |Star r1 -> number_of_letters r1

let linearize x =
  let rec aux x n =
    match x with
    | Empty -> (Empty, n)
    | Eps -> (Eps, n)
    | Letter l -> (Letter (l, n), n + 1)
    | Union (r1, r2) ->
        let r1', n1 = aux r1 n in
        let r2', n2 = aux r2 n1 in
        (Union (r1', r2'), n2)
    | Concat (r1, r2) ->
        let r1', n1 = aux r1 n in
        let r2', n2 = aux r2 n1 in
        (Concat (r1', r2'), n2)
    | Star r ->
        let r', n' = aux r n in
        (Star r', n')
  in
  fst (aux x 1)

let rec max_letter x =
  match x with
  | Eps -> -1
  | Empty -> -1
  | Letter l -> l
  | Union (r1, r2) -> max (max_letter r1) (max_letter r2)
  | Concat (r1, r2) -> max (max_letter r1) (max_letter r2)
  | Star r1 -> max_letter r1

let glushkov e = failwith "not implemented"



let delta_set x = failwith "not implemented"

let has_accepting_state x = failwith "not implemented"
let nfa_accept x = failwith "not implemented"
let build_set x = failwith "not implemented"
let powerset x = failwith "not implemented"

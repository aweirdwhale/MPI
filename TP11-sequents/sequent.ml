type 'a prop =
  | Top
  | Bot
  | V of 'a
  | Not of 'a prop
  | And of 'a prop * 'a prop
  | Or of 'a prop * 'a prop
  | Impl of 'a prop * 'a prop


type 'a sequent = {
  gamma : 'a prop list;
  delta : 'a prop list;
  gamma_var : 'a prop list;
  delta_var : 'a prop list
}

let create_sequent gamma delta =
  {
    gamma = gamma;
    delta = delta;
    gamma_var = [];
    delta_var = [];
  }


let member item list =
  let rec aux l =
    match l with
    | [] -> false
    | x :: xs -> if x = item then true else aux xs
  in
  aux list


let bot (seq : 'a sequent) : bool =
  let rec contains_bot lst =
    match lst with
    | [] -> false
    | Bot :: _ -> true
    | _ :: xs -> contains_bot xs
  in
  contains_bot seq.gamma || contains_bot seq.gamma_var

let top (seq : 'a sequent) : bool =
  let rec contains_top lst =
    match lst with
    | [] -> false
    | Top :: _ -> true
    | _ :: xs -> contains_top xs
  in
  contains_top seq.delta || contains_top seq.delta_var

let axiom (seq : 'a sequent) : bool =
  let all_gamma = seq.gamma @ seq.gamma_var in
  let all_delta = seq.delta @ seq.delta_var in
  let rec exists_in lst x =
    match lst with
    | [] -> false
    | y :: ys -> if y = x then true else exists_in ys x
  in
  let rec check lst1 lst2 =
    match lst1 with
    | [] -> false
    | x :: xs -> if exists_in lst2 x then true else check xs lst2
  in
  check all_gamma all_delta

exception Wrong_rule of string


let and_gamma (seq : 'a sequent) : 'a sequent =
  match seq.gamma with
  | (And (phi, psi)) :: gamma_rest ->
      { seq with gamma = phi :: psi :: gamma_rest }
  | _ -> raise (Wrong_rule "And Gamma")

let or_gamma (seq : 'a sequent) : 'a sequent * 'a sequent =
  match seq.gamma with
  | (Or (phi, psi)) :: gamma_rest ->
      ({ seq with gamma = phi :: gamma_rest },
       { seq with gamma = psi :: gamma_rest })
  | _ -> raise (Wrong_rule "Or Gamma")

let impl_gamma (seq : 'a sequent) : 'a sequent * 'a sequent =
  match seq.gamma with
  | (Impl (phi, psi)) :: gamma_rest ->
      let left = { seq with gamma = gamma_rest; delta = phi :: seq.delta } in
      let right = { seq with gamma = psi :: gamma_rest } in
      (left, right)
  | _ -> raise (Wrong_rule "Impl Gamma")


let not_gamma (seq : 'a sequent) : 'a sequent =
  match seq.gamma with
  | (Not phi) :: gamma_rest ->
      { seq with gamma = gamma_rest; delta = phi :: seq.delta }
  | _ -> raise (Wrong_rule "Not Gamma")

let and_delta (seq : 'a sequent) : 'a sequent * 'a sequent =
  match seq.delta with
  | (And (phi, psi)) :: delta_rest ->
      ({ seq with delta = phi :: delta_rest },
       { seq with delta = psi :: delta_rest })
  | _ -> raise (Wrong_rule "And Delta")

let or_delta (seq : 'a sequent) : 'a sequent =
  match seq.delta with
  | (Or (phi, psi)) :: delta_rest ->
      { seq with delta = phi :: psi :: delta_rest }
  | _ -> raise (Wrong_rule "Or Delta")

let impl_delta (seq : 'a sequent) : 'a sequent =
  match seq.delta with
  | (Impl (phi, psi)) :: delta_rest ->
      { seq with delta = psi :: delta_rest; gamma = phi :: seq.gamma }
  | _ -> raise (Wrong_rule "Impl Delta")

let not_delta (seq : 'a sequent) : 'a sequent =
  match seq.delta with
  | (Not phi) :: delta_rest ->
      { seq with delta = delta_rest; gamma = phi :: seq.gamma }
  | _ -> raise (Wrong_rule "Not Delta")



let proof_search sequent = failwith "not implemented"

let print_proof_result gamma delta =
  let result = proof_search (create_sequent gamma delta) in
  if result then Printf.printf "Séquent valide.\n%!"
  else Printf.printf "Séquent non valide.\n%!"


let test () =
  (* Exemples invalides *)
  print_proof_result [] [Or(V 1, V 1)];
  print_proof_result [] [Impl(Impl(Impl(V 1, V 2),V 1),V 2)];

  (* Exemples valides *)
  print_proof_result [] [Impl(Impl(Impl(V 1, V 2),V 1),V 1)];
  print_proof_result [And(And(V 1, V 2), V 3)] [And(V 1, And(V 2, V 3))];
  print_proof_result [Or(Or(V 1, V 2), V 3)] [Or(V 1, Or(V 2, V 3))];
  print_proof_result [V 1; V 2] [Impl(V 3, And(V 1, V 3))]

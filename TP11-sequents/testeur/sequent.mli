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


val create_sequent : 'a prop list -> 'a prop list -> 'a sequent

val member : 'a -> 'a list -> bool

val bot : 'a sequent -> bool
val top : 'a sequent -> bool
val axiom : 'a sequent -> bool

exception Wrong_rule of string
val and_gamma : 'a sequent -> 'a sequent
val or_gamma : 'a sequent -> 'a sequent * 'a sequent
val impl_gamma : 'a sequent -> 'a sequent * 'a sequent
val not_gamma : 'a sequent -> 'a sequent
val and_delta : 'a sequent -> 'a sequent * 'a sequent
val or_delta : 'a sequent -> 'a sequent
val impl_delta : 'a sequent -> 'a sequent
val not_delta : 'a sequent -> 'a sequent

val proof_search : 'a sequent -> bool

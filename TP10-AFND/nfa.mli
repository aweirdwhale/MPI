type 'a regex =
  | Empty
  | Eps
  | Letter of 'a
  | Union of 'a regex * 'a regex
  | Concat of 'a regex * 'a regex
  | Star of 'a regex

type state = int

type nfa =
  {delta : state list array array;
  accepting : bool array}

type dfa =
  {delta_d : state array array;
  accepting_d : bool array}

val parse : string -> state regex
val string_of_regex : state regex -> string
val graphviz_nfa : nfa -> string -> unit
val genere_pdf : string -> string -> state
val to_nfa : dfa -> nfa
val graphviz_dfa : dfa -> string -> unit

val merge : 'a list -> 'a list -> 'a list
val is_empty : 'a regex -> bool
val contains_epsilon : 'a regex -> bool
val prefix : 'a regex -> 'a list
val suffix : 'a regex -> 'a list
val combine : 'a list -> 'b list -> ('a * 'b) list
val factor : 'a regex -> ('a * 'a) list
val number_of_letters : 'a regex -> state
val linearize : 'a regex -> ('a * state) regex
val max_letter : state regex -> state
val glushkov : state regex -> nfa

val delta_set : nfa -> bool array -> state -> bool array
val has_accepting_state : nfa -> bool array -> bool
val nfa_accept : nfa -> state list -> bool
val build_set : nfa -> state list -> state -> state list
val powerset : nfa -> dfa

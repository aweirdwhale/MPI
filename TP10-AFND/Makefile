.PHONY: all, clean

TESTEUR := testeur.ml
TESTE := nfa.ml
CORRIGE := corrige.ml

all:
	ocamlc -c nfa.mli -o nfa.cmi
	ocamlc $(TESTE) testeur.cmo -o exec
	./exec

clean:
	rm -f *.cmx *.cmi *.o *.cmo *.out exec

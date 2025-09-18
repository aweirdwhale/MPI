.PHONY: all, clean

TESTEUR := testeur-seance.ml
TESTE := kosaraju
CORRIGE := corrige.ml

all:
	ocamlc -c $(TESTE).mli -o $(TESTE).cmi
	ocamlc -c $(TESTEUR) -o testeur.cmo
	ocamlc $(TESTE).ml testeur.cmo -o exec
	./exec

clean:
	rm -f *.cmx *.cmi *.o *.cmo *.out exec

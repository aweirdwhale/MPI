.PHONY: all, clean

TESTEUR := testeur.ml
TESTE := kosaraju
CORRIGE := corrige.ml

all:
	ocamlc -c $(TESTE).mli -o $(TESTE).cmi
	ocamlc $(TESTE).ml testeur.cmo -o exec
	./exec

clean:
	rm -f *.cmx *.cmi *.o *.cmo *.out exec

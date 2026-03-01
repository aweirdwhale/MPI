.PHONY: all, clean

TESTEUR := testeur.ml
TESTE := tpOthello
CORRIGE := corrige.ml

all:
	ocamlc -c $(TESTE).mli -o $(TESTE).cmi
	ocamlc othello.cmo $(TESTE).ml testeur.cmo -o exec
	./exec

clean:
	rm -f *.cmx *.cmi *.o *.out exec

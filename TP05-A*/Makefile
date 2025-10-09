.PHONY: all, clean

TESTEUR := testeur.ml
TESTE := taquin
CORRIGE := corrige.ml

all:
	ocamlc -c vector.mli -o vector.cmi
	ocamlc -c heap.mli -o heap.cmi
	ocamlc -c $(TESTE).mli -o $(TESTE).cmi
	ocamlc vector.mli vector.ml heap.mli heap.ml $(TESTE).ml testeur.cmo -o exec
	./exec

clean:
	rm -f *.cmx *.cmi *.o *.cmo *.out exec

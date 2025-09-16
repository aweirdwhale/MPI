.PHONY: all, clean

TESTEUR := testeur.c
TESTE := graphes
CFLAGS := -Wall -Wextra -fsanitize=undefined -g

all:
	sed -i 's/main/test__eleve/g' $(TESTE).c
	gcc -c dynarray.c -o dynarray.o $(CFLAGS)       # bibliothèque prérequise
	gcc -c file.c -o file.o $(CFLAGS)       # bibliothèque prérequise
	gcc -c pile.c -o pile.o $(CFLAGS)       # bibliothèque prérequise
	gcc -c $(TESTE).c -o $(TESTE).o $(CFLAGS)
	gcc -o exec dynarray.o file.o pile.o $(TESTE).o testeur.o $(CFLAGS) -Wl,--unresolved-symbols=ignore-all
	./exec
	sed -i 's/test__eleve/main/g' $(TESTE).c


clean:
	rm -f *.o exec

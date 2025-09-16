.PHONY: all, clean

TESTEUR := testeur.c
TESTE := kruskal
CFLAGS := -Wall -Wextra -fsanitize=undefined -g

all:
	sed -i 's/main/test__eleve/g' $(TESTE).c
	gcc -c union_find.c -o union_find.o $(CFLAGS)       # bibliothèque prérequise
	gcc -c $(TESTE).c -o $(TESTE).o $(CFLAGS)
	gcc -o exec union_find.o $(TESTE).o testeur.o $(CFLAGS) -Wl,--unresolved-symbols=ignore-all
	./exec
	sed -i 's/test__eleve/main/g' $(TESTE).c


clean:
	rm -f *.o exec

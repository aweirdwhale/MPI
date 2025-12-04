.PHONY: all, clean

TESTEUR := testeur.c
TESTE := afd

all:
	sed -i 's/main/test__eleve/g' $(TESTE).c
	gcc -c afdio.c -o afdio.o -Wall -Wextra -fsanitize=undefined         # bibliothèque prérequise
	gcc -c $(TESTE).c -o $(TESTE).o -Wall -Wextra -fsanitize=undefined
	gcc -o exec afdio.o $(TESTE).o testeur.o -Wall -Wextra -fsanitize=undefined -Wl,--unresolved-symbols=ignore-all -g
	./exec 
	sed -i 's/test__eleve/main/g' $(TESTE).c


clean:
	rm -f afdio.o afd.o exec

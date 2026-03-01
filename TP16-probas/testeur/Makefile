.PHONY: all, clean

TESTEUR := testeur.c
TESTE := dames

all:
	sed -i 's/main/test__eleve/g' $(TESTE).c
	sed -i 's/main/test__eleve2/g' freivalds.c
	gcc -c $(TESTE).c -o $(TESTE).o -Wall -Wextra -fsanitize=undefined
	gcc -c freivalds.c -o freivalds.o -Wall -Wextra -fsanitize=undefined
	gcc -o exec $(TESTE).o freivalds.o testeur.o -Wall -Wextra -fsanitize=undefined -Wl,--unresolved-symbols=ignore-all -g
	./exec 
	sed -i 's/test__eleve/main/g' $(TESTE).c
	sed -i 's/test__eleve2/main/g' freivalds.c


clean:
	rm -f *.o exec

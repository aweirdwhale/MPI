EXEC=exe
SRC=kmeans.c
OBJ=$(SRC:%.c=%.o)
GCH=$(SRC:%.c=%.gch)
CFLAGS += -Wall -Wextra -g -O2 -fsanitize=undefined
LINK += -L. -loutils

all: $(EXEC)

$(EXEC) :
	sed -i 's/main/test__eleve/g' kmeans.c
	gcc -o kmeans.o -c kmeans.c $(CFLAGS)
	gcc -o exec kmeans.o testeur.o $(LINK) $(CFLAGS)
	LD_LIBRARY_PATH=. ./exec compress poitiers.png poitiers-comp.png 10
	sed -i 's/test__eleve/main/g' kmeans.c
	
clean :
	rm -f exec
	rm -f $(OBJ)
	rm -f $(GCH)

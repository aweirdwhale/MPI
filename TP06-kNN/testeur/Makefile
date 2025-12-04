
# On indique les fichiers .c, les fichiers .o, et l'exécutable
EXEC = exec
SRC = administratif.c main.c testeur.c
OBJ = $(SRC:%.c=%.o)
TESTE := main

# Les options données à gcc
CFLAGS += -Wall -Wextra -g -O3 -Wl,--unresolved-symbols=ignore-all

# Les noms des fichiers de MNIST
OBS_TRAIN = train-images-idx3-ubyte
LABEL_TRAIN = train-labels-idx1-ubyte
OBS_TEST = t10k-images-idx3-ubyte
LABEL_TEST = t10k-labels-idx1-ubyte
	
all :
	sed -i 's/main/test__eleve/g' $(TESTE).c
	gcc -c administratif.c -o administratif.o $(CFLAGS)
	gcc -c main.c -o main.o $(CFLAGS)
	gcc $(OBJ) -L. -ltoolbox -o $(EXEC) $(CFLAGS)
	LD_LIBRARY_PATH=. ./exec 
	sed -i 's/test__eleve/main/g' $(TESTE).c
	@echo "Exécution : 'make run' ou 'LD_LIBRARY_PATH=. ./$(EXEC) arguments'"
	
clean :
	rm main.o administratif.o 
	rm -f $(EXEC)

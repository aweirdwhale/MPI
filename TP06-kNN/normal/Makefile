
# On indique les fichiers .c, les fichiers .o, et l'exécutable
EXEC = exec
SRC = administratif.c main.c
OBJ = $(SRC:%.c=%.o)

# Les options données à gcc
CFLAGS += -Wall -Wextra -g -O0

# Les noms des fichiers de MNIST
OBS_TRAIN = train-images-idx3-ubyte
LABEL_TRAIN = train-labels-idx1-ubyte
OBS_TEST = t10k-images-idx3-ubyte
LABEL_TEST = t10k-labels-idx1-ubyte

$(EXEC) : $(OBJ)
	gcc $(OBJ) -L. -ltoolbox -o $(EXEC) $(CFLAGS)
	@echo "Exécution : 'make run' ou 'LD_LIBRARY_PATH=. ./$(EXEC) arguments'"

run : $(EXEC)
	LD_LIBRARY_PATH=. ./$(EXEC) display $(OBS_TRAIN) 6000 28
#./$(EXEC) matrix $(OBS_TRAIN) $(LABEL_TRAIN) $(OBS_TEST) $(LABEL_TEST) 3

%.o : %.c
	gcc -c $^ -o $@ $(CFLAGS)

clean :
	rm -f $(OBJ)
	rm -f $(EXEC)

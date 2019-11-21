########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -ansi -std=c99
EJS = transforma
########################################################
OBJECTSP3_E1 = transforma.o intermedia.o afnd.o
########################################################

all: $(EJS) clear

transforma: $(OBJECTSP3_E1)
	$(CC) $(CFLAGS) -g -o transforma $(OBJECTSP3_E1)

transforma.o: transforma.c transforma.h afnd.h
	$(CC) $(CFLAGS) -g -c transforma.c

intermedia.o: intermedia.c
	$(CC) $(CFLAGS) -g -c intermedia.c

afnd.o: afnd.c
	$(CC) -g -c afnd.c

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS) *.dot *.png

png:
	dot af11.dot -Tpng -o af11.png


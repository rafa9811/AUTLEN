########################################################
CC=gcc
CFLAGS= -g -Wall
EJS = minimiza
########################################################
OBJECTSP3_E1 = minimiza.o intermedia.o afnd.o
########################################################

all: $(EJS) clear

minimiza: $(OBJECTSP3_E1)
	$(CC) $(CFLAGS) -g -o minimiza $(OBJECTSP3_E1)

minimiza.o: minimiza.c minimiza.h afnd.h
	$(CC) $(CFLAGS) -g -c minimiza.c

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

CC=g++
CFLAGS=-I.
LIBS=-lncurses -lGL -lGLU -lglut

torus: torus.o
	$(CC) -o torus torus.o $(LIBS)

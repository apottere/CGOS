CFILES = run_gol.c gol_square.h map.h snake.h
OBJS = run_gol.o gol_square.o map.o snake.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -fopenmp -c $(DEBUG) `pkg-config --cflags --libs gtk+-2.0 gthread-2.0`
LIBS = `pkg-config --libs gtk+-2.0 gthread-2.0`
LFLAGS = -Wall -fopenmp $(DEBUG) `pkg-config --libs gtk+-2.0 gthread-2.0`
RESULT = a.out

$(RESULT) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(RESULT)

run_gol.o : $(CFILES)
	$(CC) $(CFLAGS) run_gol.c

gol_square.o : gol_square.h gol_square.c
	$(CC) $(CFLAGS) gol_square.c

map.o : map.h map.c
	$(CC) $(CFLAGS) map.c

snake.o : snake.h snake.c
	$(CC) $(CFLAGS) snake.c

clean:
	\rm *.o $(RESULT)

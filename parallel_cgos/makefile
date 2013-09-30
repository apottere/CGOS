CFILES = cgos_main.c map.h snake.h head.h player.h worker.h tags.h
OBJS = cgos_main.o map.o snake.o head.o player.o worker.o 
CC = mpicc
DEBUG = -g
CFLAGS = -Wall -fopenmp -c $(DEBUG) `pkg-config --cflags --libs gtk+-2.0`
LIBS = `pkg-config --libs gtk+-2.0`
LFLAGS = -Wall -fopenmp $(DEBUG) `pkg-config --libs gtk+-2.0`
RESULT = a.out

$(RESULT) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(RESULT)
	./mpicp a.out

cgos_main.o : $(CFILES)
	$(CC) $(CFLAGS) cgos_main.c

map.o : map.h map.c
	$(CC) $(CFLAGS) map.c

snake.o : snake.h snake.c
	$(CC) $(CFLAGS) snake.c

player.o : player.h player.c 
	$(CC) $(CFLAGS) player.c

head.o : head.h head.c
	$(CC) $(CFLAGS) head.c

worker.o : worker.h worker.c
	$(CC) $(CFLAGS) worker.c

clean:
	\rm *.o $(RESULT)

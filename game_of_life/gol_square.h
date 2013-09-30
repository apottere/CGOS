#ifndef gol_sq_h
#define gol_sq_h

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEAD 0
#define ALIVE 1
enum SETUPS_t { BLACK, WHITE, RANDOM, BLOCK, BEEHIVE, BLINKER2, TOAD2, GLIDER, LWSS, PULSAR, GLIDER_GUN };

typedef struct{
	int X, Y;
	int isAlive;	
	int nextState;
} gol_sq;

extern gol_sq *** CELLS;
extern GtkWidget ***FIELD;
extern const guint R;
extern const guint C;

void alloc_CELLS();
gol_sq * make_square();
void think( gol_sq * sq, gol_sq * nbrs );
void update_cell( gol_sq * sq );
void init_cells( enum SETUPS_t setup );
void sq_cpy( gol_sq * dest, gol_sq * src );
void free_CELLS();
void update_cells();

#endif

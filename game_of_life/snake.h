#ifndef snake_h
#define snake_h

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "gol_square.h"

#define DEAD 0
#define ALIVE 1
#define MAX_SNAKE_LENGTH 150

enum DIRS { UP, DOWN, LEFT, RIGHT, STOP };

typedef struct{
	int isDying;
	int isAlive;
	int len;
	int ** body; /* body[0][0,1] is head X,Y, body[len-1] = tail */
	int timeOfDeath;
	char * deathMsg;
	enum DIRS curr_dir;
} snake;

extern gol_sq *** CELLS;
extern GtkWidget ***FIELD;
extern const guint R;
extern const guint C;
extern gol_sq ***CELLS;
extern enum DIRS *PDIR;
extern snake **SNAKES;
extern const int NUM_SNAKES;
extern int NUM_LIVE_SNAKES;
extern GtkWidget **INDICATOR;
extern int NUM_TICKS;
extern GdkColor red;

void init_snakes();

/* return a snake with length len and head at (start_X, start_Y), 
	trailing off to the left of the head, and heading RIGHT */
snake * make_snake(int len, int start_X, int start_Y, enum DIRS dir);

void move_all_snakes();

/* move the snake s in the direction dir */
void move_snake( snake * s, enum DIRS dir ); 

/* free the memory associated with snake */
void free_snake(snake *s);

void test_all_snakes();

/* tests snake for collision against cells 
TODO change the name to be more descriptive
*/
void test_snake(snake *s);

/* Goes through all the snakes in SNAKES and checks 
if any have collided -- their head is in another snake's body.
 Sets any dead snakes' isAlive to 0
and modifes NUM_LIVE_SNAKES counter appropriately */
void test_snake2snake_collisions();

/* Increases the length of every live snake by 1.
	Appends to the same location as the current last guy.
	This works due to the way move_snake is implemented
	(each is copied over to the location of the previous segment)
*/
void incr_snakes();



#endif

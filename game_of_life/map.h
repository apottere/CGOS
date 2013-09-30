#ifndef map_h
#define map_h

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "gol_square.h"
#include "snake.h"
#include <unistd.h>
#include <string.h>

/* List extern variables, whatever they are.
	These are the global variables specified in the main file, run_gol.c */

extern const guint R;
extern const guint C;
extern GtkWidget *message_label;
extern GtkWidget *window;
extern GtkWidget *table;
extern GdkPixbuf *IMAGE[19];
extern const int NUM_IMAGES;
extern GtkWidget ***FIELD;
extern GtkWidget **INDICATOR;
extern GtkWidget **POINTS;
extern gol_sq *** CELLS;
extern int RUNNING;
extern int STEP;
extern int EXIT;
extern int START;
extern int QUIT;
extern int WORKER_FINISHED;
extern snake **SNAKES;
extern enum DIRS *PDIR;
enum QUIT {WINDOW, Q_KEY, P1, P2, GAME_OVER};
extern const int NUM_SNAKES;
extern GdkColor gray;
extern GdkColor dgray;
extern GdkColor green;

void close_game(enum QUIT qt);
void draw_snakes();
void set_gui();
void countdown();
void toggle_state();
void key_event(GtkWidget *widget, GdkEventKey *kevent, gpointer data);
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
GtkWidget* make_gui();
void wipe_board();
void show_winner(int winner);
void load_images();

#endif

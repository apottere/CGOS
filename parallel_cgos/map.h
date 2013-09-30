#ifndef map_h
#define map_h

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include <unistd.h>
#include <string.h>
#include <mpi.h>

extern int R;
extern int C;
extern int NUM_TICKS;
extern int NUM_SNAKES;
extern int NUM_LIVE_SNAKES;
extern int ** LOGICAL_CELLS;

extern GdkColor red; 
extern GdkColor gray;
extern GdkColor dgray;
extern GdkColor green;
extern GtkWidget *message_label;
extern GtkWidget *window;
extern GtkWidget *table;
extern GdkPixbuf *IMAGE[19];
extern const int NUM_IMAGES; 
extern GtkWidget ***FIELD;
extern GtkWidget **INDICATOR;
extern GtkWidget **POINTS;
extern int RUNNING;
extern int STEP;
extern int EXIT;
extern int START;
extern int QUIT;
extern int WORKER_FINISHED;
extern enum DIRS *PDIR;
enum QUIT {WINDOW = 120, GAME_OVER};

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

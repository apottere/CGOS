#ifndef player_h
#define player_h

#include <gtk/gtk.h>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "snake.h"
#include "tags.h"

#define HEAD 0

extern int R;
extern int C;
extern int NUM_TICKS;
extern int NUM_SNAKES;
extern int NUM_LIVE_SNAKES;
extern int ** board_color_ixs;
extern enum DIRS dir;

void player_instrs();
enum TAGS player_get_head_msg();
void rcv_board();
void rcv_snakeinfo(int * life_status);
void display_board();
void get_send_move();
void display_final_board();

#endif

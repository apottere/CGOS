#ifndef head_h 
#define head_h

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "snake.h"
#include "tags.h"

#define DEAD 0
#define ALIVE 1

extern int R;
extern int C;
extern int NUM_TICKS;
extern int NUM_SNAKES;
extern int NUM_LIVE_SNAKES;
extern snake ** SNAKES;
extern int ** board_color_ixs;
extern int ** LOGICAL_CELLS;
extern int NUM_PLAYERS;
extern int NUM_WORKERS;
extern int BLOCK_SIZE;

void initialize_board();
void send_board_to_workers();
void send_board_to_players();
void send_begin_msgs();
void head_instrs();
void send_worker_request_msgs(); 
void send_player_request_msgs();
void receive_worker_replies();
void receive_player_replies();
int game_logic();
void send_end_msgs();

#endif

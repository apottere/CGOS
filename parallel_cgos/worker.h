#ifndef worker_h
#define worker_h

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "tags.h"

#define HEAD 0
#define ALIVE 1
#define DEAD 0

extern int R;
extern int C;
extern int NUM_TICKS;
extern int NUM_SNAKES;
extern int NUM_LIVE_SNAKES;
extern int ** LOGICAL_CELLS_worker_portion;
extern int BLOCK_SIZE;
extern int RANK;
extern int NUM_WORKERS;

void worker_instrs();
void rcv_board_section();
void send_neighbor_segments();
void get_neighbor_segments();
void update_section();
void send_update();
enum TAGS worker_get_head_msg();

#endif

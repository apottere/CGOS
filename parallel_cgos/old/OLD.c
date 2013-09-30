#include "map.h"
#include "gol_square.h"
#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <omp.h>
#include <unistd.h>

#define MASTER 0
#define SLAVE 1

// GLobal constants controlling map size.
const guint R = 50;
const guint C = 100;

// Global program flow controllers.
int RUNNING = 0;			// Controls game state: 0 - Stopped, 1 - Running.
int STEP = 0;				// Controls game state: 0 - Stopped, 1 - Step once.
int EXIT = 0;				// Exit signal to threads: 0 - Running, 1 - Terminate. 
int START = 0;
int QUIT = 0;
int WORKER_FINISHED = 0;	// Reply signal for worker: 0 - Working, 1 - Finished.

// Global GUI item pointers.
GtkWidget *window;
GtkWidget *table;
GtkWidget *message_label;
GdkPixbuf *IMAGE[19];
const int NUM_IMAGES = 19;
GtkWidget ***FIELD;
GtkWidget **INDICATOR;
GtkWidget **POINTS;
gol_sq *** CELLS;
GdkColor red = { 1, 0xD600, 0x9B00, 0x9B00 };
GdkColor gray = { 1, 0xE800, 0xE800, 0xE800 };
GdkColor dgray = { 1, 0xD200, 0xD200, 0xD200 };
GdkColor green = { 1, 0x6300, 0xDC00, 0x8300 };

// Snake controls.
snake **SNAKES;
const int NUM_SNAKES = 3;
int NUM_LIVE_SNAKES = 3;
enum DIRS *PDIR;
int NUM_TICKS = 0;

// Main.
int main(int argc, char *argv[]){
	// Obligatory gtk stuff.
	g_thread_init(NULL);
	gdk_threads_init();
	gtk_init(&argc, &argv);

	// Allocate memory and initialize CELLS.
	// WARNING: Must call before make_gui().
	alloc_CELLS();
	if(argc == 2) {
		init_cells( atoi(argv[1]) );
	} else {
		init_cells( GLIDER_GUN );
	}

	// Malloc and initialize snakes.
	init_snakes();

	// Set up and return window (Also sets up FIELD).
	window = make_gui();

	// Assumes valid start position.
	draw_snakes();

	// Initialize thread variables.
	int tid;
	
	//g_main_context_iteration(NULL, 0);
	//Spawn game thread.
	#pragma omp parallel private(tid) num_threads(2)
	{
		// Find thread ID for each thread.
		tid = omp_get_thread_num();

		//Main thread.
		if(tid == MASTER) {
			printf("Main thread here, %d total.\n", omp_get_num_threads());

			// Show window and hand over to gtk with locking.
			gtk_widget_show(window);
			gdk_threads_enter();
			gtk_main();
			gdk_threads_leave();
			
		// Worker thread.
		} else if(tid == SLAVE) {
			printf("Worker thread here!\n");
			
			// Run until EXIT signalled.
			//while(!EXIT) {
				
				countdown();

				// Run the GUI while RUNNING, or step once.
				//(RUNNING takes precedence and consumes STEP).
				while(RUNNING /*|| STEP*/) {
					NUM_TICKS++;
					update_cells();
					//every so often, increase snake length
					if(NUM_TICKS % 5 == 0){
						incr_snakes();
						incr_snakes();
					}
					move_all_snakes();
					gdk_threads_enter();
					set_gui();
					draw_snakes();
					gdk_threads_leave();
					test_all_snakes();
					if(NUM_LIVE_SNAKES < 2) {
						close_game( GAME_OVER);
					}
				}	
			
			// Signal worker is ready to terminate.
			WORKER_FINISHED = 1;
		}
	}
	return 0;
}


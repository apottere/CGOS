#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "player.h"
#include "worker.h"
#include "head.h"
#include "tags.h"

int R;
int C;
int NUM_TICKS;
int NUM_SNAKES;
int NUM_LIVE_SNAKES;
int ** LOGICAL_CELLS;
int ** LOGICAL_CELLS_worker_portion;
int ** board_color_ixs;
snake ** SNAKES;
int NUM_PLAYERS;
int NUM_WORKERS = 6;
int BLOCK_SIZE;
int RANK;
enum DIRS dir;

/* for map.h ... */
GdkColor red = { 1, 0xD600, 0x9B00, 0x9B00 };
GdkColor gray = { 1, 0xE800, 0xE800, 0xE800 };
GdkColor dgray = { 1, 0xD200, 0xD200, 0xD200 };
GdkColor green = { 1, 0x6300, 0xDC00, 0x8300 };
GtkWidget *message_label;
GtkWidget *window;
GtkWidget *table;
GdkPixbuf *IMAGE[19];
const int NUM_IMAGES = 19;
GtkWidget ***FIELD;
GtkWidget **INDICATOR;
GtkWidget **POINTS;
int RUNNING;
int STEP;
int EXIT;
int START;
int QUIT;
int WORKER_FINISHED;
enum DIRS *PDIR;

int main(int argc, char *argv[]) 
{
	const int TAG_GENERAL = 1;

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen;

	int numTasks;
	int rank;
	int source;
	int dest, src;
	int rc;
	int count;
	int dataWaitingFlag = 1;

	char inMsg;
	char outMsg;

	enum TAGS snake_move;
	int i,j,k;

	MPI_Status Stat;

	// Initialize the MPI stack and pass 'argc' and 'argv' to each worker node
	MPI_Init(&argc,&argv);

	// Gets number of tasks/processes that this program is running on
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
	NUM_PLAYERS = numTasks - NUM_WORKERS - 1; //don't count workers, don't count head
	NUM_SNAKES = NUM_PLAYERS;
 	NUM_LIVE_SNAKES = NUM_PLAYERS;
	NUM_TICKS = 0;

	//TODO compute R and C here based on the number of players, so that everybody knows it
	//Ensure R is divisible by NUM_WORKERS
	R = 300;
	C = 600;
	BLOCK_SIZE = R/NUM_WORKERS;
	if(R%NUM_WORKERS != 0){
		printf("Error, R doesn't divide evenly between workers\n");
		exit(1);
	}
	//allocate memory for board_color_ixs
	//and LOGICAL_CELLS
	if(board_color_ixs == NULL){
		board_color_ixs = malloc(R*sizeof(int*));
		if(board_color_ixs == NULL){
			printf("Error, not enough memory for board_color_ixs\n");
			exit(1);	
		}
		for(i=0;i<R;i++){
			board_color_ixs[i] = malloc(C*sizeof(int));
			if(board_color_ixs[i] == NULL){
				printf("Error, not enough memory for board_color_ixs[%i]\n",i);
				exit(1);	
			}
		}
	}	
	if(LOGICAL_CELLS == NULL){
		LOGICAL_CELLS = malloc(R*sizeof(int*));
		if(LOGICAL_CELLS == NULL){
			printf("Error, not enough memory for LOGICAL_CELLS\n");
			exit(1);	
		}
		for(i=0;i<R;i++){
			LOGICAL_CELLS[i] = malloc(C*sizeof(int));
			if(LOGICAL_CELLS[i] == NULL){
				printf("Error, not enough memory for LOGICAL_CELLS [%i]\n",i);
				exit(1);	
			}
		}
	}	
	if(LOGICAL_CELLS_worker_portion == NULL){
		LOGICAL_CELLS_worker_portion = malloc(R*sizeof(int*));
		if(LOGICAL_CELLS_worker_portion == NULL){
			printf("Error, not enough memory for LOGICAL_CELLS_worker_portion \n");
			exit(1);	
		}
		for(i=0;i<BLOCK_SIZE+2;i++){
			LOGICAL_CELLS_worker_portion[i] = malloc(C*sizeof(int));
			if(LOGICAL_CELLS_worker_portion[i] == NULL){
				printf("Error, not enough memory for LOGICAL_CELLS_worker_portion [%i]\n",i);
				exit(1);	
			}
		}
	}	

	// Gets the rank (process/task number) that this program is running on 
	MPI_Comm_rank(MPI_COMM_WORLD, &RANK);

	// If the head node
	if (RANK == HEAD) {
		printf("Head here: rank %i\n",RANK);
		head_instrs();
	}
	// Else if a worker node
	else if ( RANK > HEAD && RANK <= HEAD + NUM_WORKERS) {
		printf("Worker here: rank %i \n",RANK);
		worker_instrs();
	}
	// The rest are players
	else{
		printf("Player here: rank %i \n",RANK);
		player_instrs();
	}
	MPI_Finalize();
	return 0;
}

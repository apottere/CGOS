#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define NUM_SLAVES 6

//for snake moves
enum DIRS { UP=100, DOWN, LEFT, RIGHT, STOP };
//for message tags, so that nodes know what to expect next
enum TAGS { BEGIN=110, END, SNAKE_MOVE, BOARD_DIMS, BOARD_UPDATE_REQUEST, W_BOARD_UPDATE, P_BOARD_UPDATE, W_W_INFO };

int main(int argc, char *argv[]) 
{
  const int MASTER = 0;
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
	int i;
	
  MPI_Status Stat;

  // Initialize the MPI stack and pass 'argc' and 'argv' to each slave node
  MPI_Init(&argc,&argv);

  // Gets number of tasks/processes that this program is running on
  MPI_Comm_size(MPI_COMM_WORLD, &numTasks);

  // Gets the rank (process/task number) that this program is running on 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // If the master node
  if (rank == MASTER) {
	
    // Receive move messages from each node
    for(i=0;i<10;i++){
	    for (src= 1; src < numTasks; src++) {
	/*
		dataWaitingFlag = 0;
		    do {
		      MPI_Iprobe(src, SNAKE_MOVE, MPI_COMM_WORLD, &dataWaitingFlag, MPI_STATUS_IGNORE);
		      //printf("Waiting\n");
		    } while (!dataWaitingFlag);
	*/
		    // Get the message and put it in 'inMsg'
		    rc = MPI_Recv(&snake_move, 1, MPI_INT, src, SNAKE_MOVE, MPI_COMM_WORLD, &Stat);
	      printf("Move %i received from node %i with tag %i: it was %i\n",i,src,SNAKE_MOVE,snake_move);
	    }
	}
  } 
  // Else a slave node
  else  {
	//send LEFT to the head/manager
	if(rank < 2)
		snake_move = LEFT;
	else
		snake_move = RIGHT;
	for(src=0;src<10;src++){
	MPI_Send( &snake_move, 1, MPI_INT, MASTER, SNAKE_MOVE, MPI_COMM_WORLD );
	printf("Slave %i sent message %i to head\n",rank,snake_move);
	}
  }
  MPI_Finalize();
}



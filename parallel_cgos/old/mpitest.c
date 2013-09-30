#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define NUM_SLAVES 6

int main(int argc, char *argv[]) 
{
  const int MASTER = 0;
  const int TAG_GENERAL = 1;

  char processor_name[MPI_MAX_PROCESSOR_NAME];	
  int namelen;

  int numTasks;
  int rank;
  int source;
  int dest;
  int rc;
  int count;
  int dataWaitingFlag;

  char inMsg;
  char outMsg;
	
  MPI_Status Stat;

  // Initialize the MPI stack and pass 'argc' and 'argv' to each slave node
  MPI_Init(&argc,&argv);

  // Gets number of tasks/processes that this program is running on
  MPI_Comm_size(MPI_COMM_WORLD, &numTasks);

  // Gets the rank (process/task number) that this program is running on 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // If the master node
  if (rank == MASTER) {
	
printf("My rank is %d\n",rank);
printf("My size is %d\n",numTasks);
    // Send out messages to all the sub-processes
    for (dest = 1; dest < numTasks; dest++) {
      outMsg = rand() % 256;	// Generate random message to send to slave nodes

      // Send a message to the destination	
      rc = MPI_Send(&outMsg, 1, MPI_CHAR, dest, TAG_GENERAL, MPI_COMM_WORLD);			
      printf("Task %d: Sent message %d to task %d with tag %d\n",
             rank, outMsg, dest, TAG_GENERAL);
    }
    MPI_Get_processor_name(processor_name,&namelen);
printf("Master aka %s\n",processor_name);
  } 

  // Else a slave node
  else  {
	printf("My rank is %d\n",rank);
    // Wait until a message is there to be received	
    do {
      MPI_Iprobe(MASTER, 1, MPI_COMM_WORLD, &dataWaitingFlag, MPI_STATUS_IGNORE);
      //printf("Waiting\n");
    } while (!dataWaitingFlag);

    // Get the message and put it in 'inMsg'
    rc = MPI_Recv(&inMsg, 1, MPI_CHAR, MASTER, TAG_GENERAL, MPI_COMM_WORLD, &Stat);

    // Get how big the message is and put it in 'count'
    rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
    MPI_Get_processor_name(processor_name,&namelen);
    printf("Task %d: %s received %d char(s) (%d) from task %d with tag %d \n",
            rank, processor_name, count, inMsg, Stat.MPI_SOURCE, Stat.MPI_TAG);
		
  }

  MPI_Finalize();
}



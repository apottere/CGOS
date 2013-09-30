#include "worker.h"

void worker_instrs(){
	rcv_board_section();
	int isDone = 0;
	while(!isDone){
		send_neighbor_segments();
		get_neighbor_segments();
		update_section();
		enum TAGS msg = worker_get_head_msg();
		switch(msg){
			case H_BEGIN:
				break;
			case H_W_BOARD_UPDATE_REQUEST:
				send_update();
				break;
			case H_END:
				isDone = 1;
				break;
			default:
				printf("Error, unexpected prep message %i from head\n",msg);
				isDone = 1;
				break;
		}
	}
	return;
}

/* populate LOGICAL_CELLS_worker_portion */
void rcv_board_section(){
	int i;
	MPI_Request * reqs = malloc((BLOCK_SIZE+2)*sizeof(MPI_Request));
	MPI_Status * stats = malloc((BLOCK_SIZE+2)*sizeof(MPI_Status));
	for(i=0;i<BLOCK_SIZE+2;i++){
		MPI_Irecv(LOGICAL_CELLS_worker_portion[i],C,MPI_INT,0,H_W_BOARD_SECTION+i,MPI_COMM_WORLD,&reqs[i]);
	}
	//wait for each message to clear before returning
	for(i=0;i<BLOCK_SIZE+2;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
}

/* Receive one row below our stuff and one row above our stuff */
void get_neighbor_segments(){
	int i;
	MPI_Request * reqs = malloc(2*sizeof(MPI_Request));
	MPI_Status * stats = malloc(2*sizeof(MPI_Status));
	int neighbor_U = RANK+1; //neighbor UP
	int neighbor_D = RANK-1; //neighbor DOWN
	if (neighbor_U > NUM_WORKERS){
		neighbor_U -= NUM_WORKERS;
	}
	if (neighbor_D < 1){
		neighbor_D += NUM_WORKERS;
	}
	MPI_Irecv(LOGICAL_CELLS_worker_portion[0],C,MPI_INT,neighbor_D,W_W_BOARD_SECTION,MPI_COMM_WORLD,&reqs[0]);
	MPI_Irecv(LOGICAL_CELLS_worker_portion[BLOCK_SIZE+1],C,MPI_INT,neighbor_U,W_W_BOARD_SECTION,MPI_COMM_WORLD,&reqs[1]);
	//wait for each message to clear before returning
	for(i=0;i<2;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
}

/* Send the bottom of our real cells and the top of our real cells to the neighbors */
void send_neighbor_segments(){
	int i;
	MPI_Request * reqs = malloc(2*sizeof(MPI_Request));
	MPI_Status * stats = malloc(2*sizeof(MPI_Status));
	int neighbor_U = RANK+1; //neighbor UP
	int neighbor_D = RANK-1; //neighbor DOWN
	if (neighbor_U > NUM_WORKERS){
		neighbor_U -= NUM_WORKERS;
	}
	if (neighbor_D < 1){
		neighbor_D += NUM_WORKERS;
	}
	MPI_Isend(LOGICAL_CELLS_worker_portion[1],C,MPI_INT,neighbor_D,W_W_BOARD_SECTION,MPI_COMM_WORLD,&reqs[0]);
	MPI_Isend(LOGICAL_CELLS_worker_portion[BLOCK_SIZE],C,MPI_INT,neighbor_U,W_W_BOARD_SECTION,MPI_COMM_WORLD,&reqs[1]);
	//wait for each message to clear before returning
	for(i=0;i<2;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
}

/* Apply Conway's rules to our block */
void update_section(){
	int i,j;
	int ** nexts = malloc(R*sizeof(int*));
	//allocate memory for the next iteration
	if(nexts == NULL){
		printf("Error, not enough memory for nexts\n");
		exit(1);	
	}
	for(i=0;i<BLOCK_SIZE+2;i++){
		nexts[i] = malloc(C*sizeof(int));
		if(nexts[i] == NULL){
			printf("Error, not enough memory for nexts[%i]\n",i);
			exit(1);	
		}
	}

	int U1, D1, L1, R1; //i,j value for up 1, down 1, left 1, and right 1
	int num_live_neighbors;
	//all the columns 
	for(i=1;i<=C;i++){
		//all the rows for which we are responsible
		for(j=1;j<=BLOCK_SIZE;j++){
			num_live_neighbors = 0;
			//define the indices for neighbors
			if (i==0) D1 = C-1;
			else D1 = i-1;
			if (i == C-1) U1 = 0;
			else U1 = i+1;			
			if (j == 0) L1 = R-1;
			else L1 = j-1;
			if (j == R-1) R1 = 0;
			else R1 = j+1;
			//count # live neighbors
			if(LOGICAL_CELLS_worker_portion[U1][L1] == ALIVE)
				num_live_neighbors++;
			if(LOGICAL_CELLS_worker_portion[U1][j] == ALIVE)
				num_live_neighbors++;
			if(LOGICAL_CELLS_worker_portion[U1][R1] == ALIVE)
				num_live_neighbors++;
			if(LOGICAL_CELLS_worker_portion[i][L1] == ALIVE)
				num_live_neighbors++;
			if(LOGICAL_CELLS_worker_portion[i][R1] == ALIVE)
				num_live_neighbors++;
			if(LOGICAL_CELLS_worker_portion[D1][L1] == ALIVE)
				num_live_neighbors++;
			if(LOGICAL_CELLS_worker_portion[D1][j] == ALIVE)
				num_live_neighbors++;
			if(LOGICAL_CELLS_worker_portion[D1][R1] == ALIVE)
				num_live_neighbors++;

			printf("[i][j] = [%i][%i] :: Num live neighbors = %i\n",i,j,num_live_neighbors);
			//apply rules
			if (LOGICAL_CELLS_worker_portion[i][j] == ALIVE){
				//Rule 1
				if (num_live_neighbors < 2)
				    nexts[i][j] = DEAD;
				//Rule 3
				else if (num_live_neighbors > 3)
				    nexts[i][j] = DEAD;
				//Rule 2
				else 
				    nexts[i][j] = ALIVE;
			}
			//Rule 4
			else if (LOGICAL_CELLS_worker_portion[i][j] == DEAD && num_live_neighbors == 3)
				nexts[i][j] = ALIVE;
		}
	}
	//now that everyone has thought, keep the thoughts
	//we don't need to copy over [0][*] or [NUM_BLOCKS+1][*] 
	//because we need an update now anyway
	for(i=0;i<BLOCK_SIZE+2;i++){
		free(LOGICAL_CELLS_worker_portion[i]);
	}
	free(LOGICAL_CELLS_worker_portion);
	LOGICAL_CELLS_worker_portion = nexts;

}

enum TAGS worker_get_head_msg(){
	int msg;
	MPI_Status stat;
	MPI_Recv( &msg, 1, MPI_INT, HEAD, MPI_ANY_TAG, MPI_COMM_WORLD, &stat );
	if (stat.MPI_TAG == H_W_BOARD_UPDATE_REQUEST || stat.MPI_TAG == H_W_BOARD_SECTION || stat.MPI_TAG == H_END || stat.MPI_TAG == H_BEGIN ){
		return stat.MPI_TAG;	
	}
	else{
		printf("Error, invalid MPI_TAG %i received from head...\n",stat.MPI_TAG);
		exit(1);
	}
	return -1;
}
void send_update(){
	int i;
	MPI_Request * reqs = malloc(BLOCK_SIZE*sizeof(MPI_Request));
	MPI_Status * stats = malloc(BLOCK_SIZE*sizeof(MPI_Status));
	
	for(i=0;i<BLOCK_SIZE;i++){
		MPI_Isend( LOGICAL_CELLS_worker_portion[i+1],C,MPI_INT,HEAD,W_H_BOARD_UPDATE+i,MPI_COMM_WORLD,&reqs[i]);
	}
	//wait for each message to clear before returning
	for(i=0;i<BLOCK_SIZE;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
}


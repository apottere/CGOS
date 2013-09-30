#include "head.h"

int * PLAYER_MOVES = NULL;

void head_instrs(){
	int isDone = 0;

	initialize_board();
	send_board_to_workers();
	send_begin_msgs();
	send_board_to_players();
	
	while( !isDone ){
		send_worker_request_msgs(); //blocking
		send_player_request_msgs(); //blocking
		receive_worker_replies(); //non-blocking
		receive_player_replies(); //non-blocking
		isDone = game_logic();
		send_board_to_players(); //non-blocking
	}
	send_end_msgs();
	send_board_to_players(); //final board
	return;
}

void send_board_to_workers(){
	int i,j;
	//block size (w/o the row above and the row below)
	MPI_Request * reqs = malloc((BLOCK_SIZE+2)*NUM_WORKERS*sizeof(MPI_Request));
	MPI_Status * stats = malloc((BLOCK_SIZE+2)*NUM_WORKERS*sizeof(MPI_Status));
	if(reqs == NULL || stats == NULL){
		printf("Error, cannot allocate mem for reqs or stats in send_board_to_workers\n");
		exit(1);
	}
	if( R%NUM_WORKERS != 0 ){
		printf("Error, the board does not evenly go into the workers and I am lazy.\n");
		exit(1);
	}

	//send LOGICAL_CELLS to workers
	//ensure each (rank,tag) is unique
	int row_ix;
	int num_sent = 0;
	for(i=0;i<NUM_WORKERS;i++){
		row_ix = i*BLOCK_SIZE- 1; //row below block
		if(row_ix < 0){
			row_ix += R;
		}
		MPI_Isend( LOGICAL_CELLS[row_ix], C, MPI_INT, i, H_W_BOARD_SECTION+num_sent, MPI_COMM_WORLD, &reqs[num_sent]);
		num_sent++;
		//send all the cells in BLOCK
		for(j=0;j<BLOCK_SIZE;j++){
			MPI_Isend( LOGICAL_CELLS[i*BLOCK_SIZE+j], C, MPI_INT, i, H_W_BOARD_SECTION+num_sent, MPI_COMM_WORLD, &reqs[num_sent]);
		}
		num_sent++;
		row_ix = i*BLOCK_SIZE; //row above block
		if(row_ix >= R){
			row_ix -= R;
		}
		MPI_Isend( LOGICAL_CELLS[row_ix], C, MPI_INT, i, H_W_BOARD_SECTION+num_sent, MPI_COMM_WORLD, &reqs[num_sent]);
		num_sent++;
	}
	//wait for each message to clear before returning
	for(i=0;i<(BLOCK_SIZE+2)*NUM_WORKERS;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
	return;
}

void send_board_to_players(){
	int i,j;
	MPI_Request * reqs = malloc(NUM_PLAYERS*sizeof(MPI_Request));
	MPI_Status * stats = malloc(NUM_PLAYERS*sizeof(MPI_Status));
	if(reqs == NULL || stats == NULL){
		printf("Error, cannot allocate mem for reqs or stats in send_board_to_players\n");
		exit(1);
	}
	//load board and snakes into board_color_ixs
	/* Start with Conway */
	for(i=0;i<R;i++){
		for(j=0;j<C;j++){
			/*
			if( CELLS[i][j]->isAlive ){
				board_color_ixs[i][j] = BLACK;
			}
			else{
				board_color_ixs[i][j] = WHITE;
			}
			*/
			if( LOGICAL_CELLS[i][j] == ALIVE )
				board_color_ixs[i][j] = BLACK;
			else
				board_color_ixs[i][j] = WHITE;
				
		}
	}
	/* Copy over with snakes */
	int head_color_ix_start = 3; //not BLACK, not WHITE, not DGREEN
	snake * s;
	for(i=0;i<NUM_SNAKES;i++){
		s = SNAKES[i];
		if( s->isAlive == DEAD ){
			continue;
		}
		for(j=1; j < s->len;j++){
			board_color_ixs[ s->body[j][0] ][ s->body[j][1] ] = DGREEN; 
		}
		board_color_ixs[ s->body[0][0] ][ s->body[0][1] ] = head_color_ix_start+i;
	}

	//non-blocking send matrix to each
	for(i=0;i<NUM_PLAYERS;i++){
		MPI_Isend( &board_color_ixs[0][0], R*C, MPI_INT, i+NUM_WORKERS+1, H_P_BOARD, MPI_COMM_WORLD, &reqs[i]);
	}
	//wait for each message to clear before returning
	for(i=0;i<NUM_PLAYERS;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
	return;
}

/*send begin message to all workers and all players*/
void send_begin_msgs(){
	int i;
	int msg=0;
	for(i=1;i<=NUM_PLAYERS+NUM_WORKERS;i++){
		MPI_Send( &msg, 1, MPI_INT, i, H_BEGIN, MPI_COMM_WORLD );
	}
}

void send_worker_request_msgs(){
	int i;
	int msg=0;
	MPI_Request * reqs = malloc(NUM_WORKERS*sizeof(MPI_Request));
	MPI_Status * stats = malloc(NUM_WORKERS*sizeof(MPI_Status));
	if(reqs == NULL || stats == NULL){
		printf("Error, cannot allocate mem for reqs or stats in send_board_to_players\n");
		exit(1);
	}

	for(i=1;i<=NUM_WORKERS;i++){
		MPI_Isend( &msg, 1, MPI_INT, i, H_W_BOARD_UPDATE_REQUEST, MPI_COMM_WORLD, &reqs[i]);
	}
	//wait for each message to clear before returning
	for(i=0;i<NUM_WORKERS;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
	return;
}

void send_player_request_msgs(){
	int i;
	int msg=0;
	MPI_Request * reqs = malloc(NUM_PLAYERS*sizeof(MPI_Request));
	MPI_Status * stats = malloc(NUM_PLAYERS*sizeof(MPI_Status));
	if(reqs == NULL || stats == NULL){
		printf("Error, cannot allocate mem for reqs or stats in send_board_to_players\n");
		exit(1);
	}

	for(i=1;i<=NUM_PLAYERS;i++){
		MPI_Isend( &msg, 1, MPI_INT, i+NUM_WORKERS, H_P_MOVE_REQUEST, MPI_COMM_WORLD, &reqs[i]);
	}
	//wait for each message to clear before returning
	for(i=0;i<NUM_PLAYERS;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
	return;
}

/* Receive the updated board sections from each worker row by row */
void receive_worker_replies(){ 
	int i,j;
	MPI_Request * reqs = malloc(NUM_WORKERS*BLOCK_SIZE*sizeof(MPI_Request));
	MPI_Status * stats = malloc(NUM_WORKERS*BLOCK_SIZE*sizeof(MPI_Status));
	int count = 0;
	//each worker
	for(i=0;i<NUM_WORKERS;i++){
		//each row 
		for(j=0;j<BLOCK_SIZE;j++){
			//we line up all the MPI_Irecv messages for each worker
			//and ensure each (rank,tag) combination is unique
			//assumes worker ranks are {1,2,...,NUM_WORKERS}
			MPI_Irecv(LOGICAL_CELLS[i*BLOCK_SIZE + j],C,MPI_INT,i+1,W_H_BOARD_UPDATE+j,MPI_COMM_WORLD,&reqs[count]);
			count++;
		}
	}
	//wait for each message to clear before returning
	for(i=0;i<NUM_WORKERS*BLOCK_SIZE;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
}

/* Receive the moves from the players and store in array 
 * TODO assumes even dead players send moves. this is technically
 * 	OK so long as we keep the logic good.
 * */
void receive_player_replies(){ 
	if( PLAYER_MOVES == NULL ){
		PLAYER_MOVES = malloc(NUM_PLAYERS*sizeof(int));
		if(PLAYER_MOVES == NULL){
			printf("Error, insufficient mem. for PLAYER_MOVES\n");
			exit(1);
		}
	}
	int i;
	MPI_Request * reqs = malloc(NUM_PLAYERS*sizeof(MPI_Request));
	MPI_Status * stats = malloc(NUM_PLAYERS*sizeof(MPI_Status));
	if(reqs == NULL || stats == NULL){
		printf("Error, cannot allocate mem for reqs or stats in send_board_to_players\n");
		exit(1);
	}

	for(i=1;i<=NUM_PLAYERS;i++){
		MPI_Irecv( &(PLAYER_MOVES[i-1]), 1, MPI_INT, i+NUM_WORKERS, P_H_MOVE, MPI_COMM_WORLD, &reqs[i]);
	}
	//wait for each message to clear before returning
	for(i=0;i<NUM_PLAYERS;i++){
		MPI_Wait( &reqs[i], &stats[i] );
	}
	return;
}

void send_end_msgs(){
	int i;
	int msg=0;
	for(i=1;i<=NUM_PLAYERS+NUM_WORKERS;i++){
		MPI_Send( &msg, 1, MPI_INT, i, H_END, MPI_COMM_WORLD );
	}
}

/* Carry out the game logic, update all the snakes, etc. */
int game_logic(){
	return 0;
}

void initialize_board(){
}

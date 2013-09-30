#include "player.h"

/* logic the players follow */
void player_instrs(){
	int * snake_life_statuses = malloc(NUM_SNAKES*sizeof(int));
	int isDone = 0;
	int i,j;
	for(i=0;i<NUM_SNAKES;i++){
		snake_life_statuses[i] = 1;
	}
	
	/*
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_main(); */
	while(!isDone){
		enum TAGS msg = player_get_head_msg();
		switch(msg){
			case H_P_MOVE_REQUEST:
				get_send_move();
				break;
			case H_P_BOARD:
				rcv_board();
				rcv_snakeinfo(snake_life_statuses);
				NUM_TICKS++;
				display_board();
				break;
			case H_BEGIN:
				rcv_board();
				rcv_snakeinfo(snake_life_statuses);
				display_board();
			case H_END:
				rcv_board();
				rcv_snakeinfo(snake_life_statuses);
				display_final_board();
				isDone = 1;
				break;
			default:
				printf("Error, unexpected prep message %i from head\n",msg);
				isDone = 1;
				break;
		}
	}
	free(snake_life_statuses);
	return;
}

/* wait for a signal from the head, then return the message */
enum TAGS player_get_head_msg(){
	int msg;
	MPI_Status stat;
	MPI_Recv(&msg,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&stat);
	if( stat.MPI_SOURCE != HEAD ){
		printf("Error in player::get_msg_from_head -- unexpected source\n");
		exit(1);
	}
	else if(stat.MPI_TAG != H_P_MOVE_REQUEST && stat.MPI_TAG != H_P_BOARD && 
			stat.MPI_TAG != H_P_SNAKES && stat.MPI_TAG != H_END && stat.MPI_TAG != H_BEGIN){
		printf("Error in player::get_msg_from_head -- unexpected tag\n");
		exit(1);
	}
	return stat.MPI_TAG;
}

/* the head has indicated that he is sending a copy of the board color indices over.
 receive it row by row with tag H_P_BOARD */
void rcv_board(){
	MPI_Status stat;
	MPI_Recv( &board_color_ixs[0][0], R*C, MPI_INT, HEAD, H_P_BOARD, MPI_COMM_WORLD, &stat );
}

/* the head has indicated that he is sending snake information over */
void rcv_snakeinfo(int * life_status){
	MPI_Status stat;
	MPI_Recv( life_status, NUM_SNAKES, MPI_INT, HEAD, H_P_SNAKES, MPI_COMM_WORLD, &stat );
}

/* gtk stuff TODO */
void display_board(){
}

/* obtain move from user and send it in TODO */
void get_send_move(){
	dir = UP;
	MPI_Send( &dir, 1, MPI_INT, HEAD, P_H_MOVE, MPI_COMM_WORLD );
}

/* the head has ended the game. display the board and any "final" messages we want TODO */
void display_final_board(){
}


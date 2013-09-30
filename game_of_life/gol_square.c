#include "gol_square.h"

/* Allocate the space for cells */
void alloc_CELLS(){
	int i,j;
	CELLS = malloc( sizeof(gol_sq**)*C );	
	for(i=0;i<C;i++){
		CELLS[i] = malloc( sizeof(gol_sq*)*R );
		for(j=0;j<R;j++){
			CELLS[i][j] = malloc( sizeof(gol_sq) );
		}
	}
	return;
}

/* Copies dest into src */
void sq_cpy( gol_sq * dest, gol_sq * src ){
	dest->X = src->X;
	dest->Y = src->Y;
	dest->isAlive = src->isAlive;
	dest->nextState = src->nextState;
	return;
}

/* Creates a game of life square with coordinates X and Y and alive/dead status life */
gol_sq * make_square(int X, int Y, int life){
    if (life != DEAD && life != ALIVE){
        printf("Invalid life argument to make_square");
        exit(1);
    }
    gol_sq * sq = malloc(sizeof(gol_sq));
    if (sq == NULL){ 
        printf("Insufficient memory to make a gol_sq"); 
        exit(1);
    }
    sq->X = X;
    sq->Y = Y;
    sq->isAlive = life;
    sq->nextState = -1;
    return sq;
}

/* The game of life square sq decides whether it is dead or alive in the next round 
    Follows Conway's rules from the wiki 
    nbrs is an array of length 8 containing the 8 neighbors of sq. Order does not matter.
     */
void think( gol_sq * sq, gol_sq * nbrs ){
    int live_count = 0, i; 
    for(i=0; i<8; i++){
		//printf("Neighbors %i is %i\n",i,nbrs->isAlive);
        if( nbrs->isAlive == ALIVE ){
            live_count++;
		}
        nbrs++;
    }
	if (live_count > 0 )
		//printf("Livecount = %i\n",live_count);
    //Rule 4
    if (sq->isAlive == DEAD && live_count == 3)
        sq->nextState = ALIVE;
    if (sq->isAlive == DEAD && live_count != 3)
        sq->nextState = DEAD;
    if (sq->isAlive == ALIVE){
        //Rule 1
        if (live_count < 2)
            sq->nextState = DEAD;
        //Rule 3
        else if (live_count > 3)
            sq->nextState = DEAD;
        //Rule 2
        else 
            sq->nextState = ALIVE;
    }
    return;
}

/* The game of life square sq updates by setting its life status to the 
    value of nextState 
*/
void update_cell( gol_sq * sq ){
    if (sq->nextState != DEAD && sq->nextState != ALIVE){
        printf("Error, square has invalid next state");
        exit(1);
    }
    sq->isAlive = sq->nextState;
    sq->nextState = -1;
}

/* Initialize FIELD to one of several different SETUPs
   SETUP value		Setup
   WHITE 		field is all dead
   BLACK 		field is all alive
   RANDOM	    cells in field are randomly dead or alive
   BLOCK		wiki page
   BEEHIVE 		wiki page
   BLINKER2		wiki page
   TOAD2		wiki page
   GLIDER		wiki page
   LWSS			wiki page
   PULSAR		wiki page
   GLIDER_GUN	wiki page
*/
void init_cells( enum SETUPS_t setup ){
	int i, j;
	int rcent = (int) R/2;
	int ccent = (int) C/2;
	gol_sq * cell = make_square(0,0,0);
	srand( time(NULL) );
	switch(setup){
		case WHITE:
			cell->isAlive = 0;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			break;
		case BLACK:
			cell->isAlive = 1;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			break;
		case RANDOM:
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					cell->isAlive = rand()%2;
					sq_cpy( CELLS[i][j], cell );
				}
			}
			break;
		case BLOCK:
			//start all 0s, then fix the central ones
			cell->isAlive = 0;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			//4 central ones are alive
			CELLS[ccent][rcent]->isAlive = 1;
			CELLS[ccent+1][rcent]->isAlive = 1;
			CELLS[ccent][rcent+1]->isAlive = 1;
			CELLS[ccent+1][rcent+1]->isAlive = 1;
			break;
		case BEEHIVE:
			//start all 0s, then fix the central ones
			cell->isAlive = 0;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			//make a beehive
			CELLS[ccent][rcent]->isAlive = 1;
			CELLS[ccent+1][rcent]->isAlive = 1;
			CELLS[ccent-1][rcent+1]->isAlive = 1;
			CELLS[ccent+2][rcent+1]->isAlive = 1;
			CELLS[ccent][rcent+2]->isAlive = 1;
			CELLS[ccent+1][rcent+2]->isAlive = 1;
			break;
		case BLINKER2:
			//start all 0s, then fix the central ones
			cell->isAlive = 0;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			//make a blinker of period 2
			CELLS[ccent][rcent]->isAlive = 1;
			CELLS[ccent+1][rcent]->isAlive = 1;
			CELLS[ccent+2][rcent]->isAlive = 1;
			break;
		case TOAD2:
			//start all 0s, then fix the central ones
			cell->isAlive = 0;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			//make a toad of period 2
			CELLS[ccent][rcent]->isAlive = 1;
			CELLS[ccent+1][rcent]->isAlive = 1;
			CELLS[ccent+2][rcent]->isAlive = 1;
			CELLS[ccent-1][rcent+1]->isAlive = 1;
			CELLS[ccent][rcent+1]->isAlive = 1;
			CELLS[ccent+1][rcent+1]->isAlive = 1;
			break;
		case GLIDER:
			//start all 0s, then fix the central ones
			cell->isAlive = 0;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			//make a glider
			CELLS[ccent][rcent]->isAlive = 1;
			CELLS[ccent+1][rcent+1]->isAlive = 1;
			CELLS[ccent+1][rcent+2]->isAlive = 1;
			CELLS[ccent+2][rcent]->isAlive = 1;
			CELLS[ccent+2][rcent+1]->isAlive = 1;
			break;
		case LWSS:
			//start all 0s, then fix the central ones
			cell->isAlive = 0;
			for(i=0;i<C;i++){
				for(j=0;j<R;j++){
					sq_cpy( CELLS[i][j], cell );
				}
			}
			//make a LWSS
			CELLS[ccent][rcent]->isAlive = 1;
			CELLS[ccent+3][rcent]->isAlive = 1;
			CELLS[ccent+4][rcent+1]->isAlive = 1;
			CELLS[ccent][rcent+2]->isAlive = 1;
			CELLS[ccent+4][rcent+2]->isAlive = 1;
			CELLS[ccent+1][rcent+3]->isAlive = 1;
			CELLS[ccent+2][rcent+3]->isAlive = 1;
			CELLS[ccent+3][rcent+3]->isAlive = 1;
			CELLS[ccent+4][rcent+3]->isAlive = 1;
			break;
		case PULSAR:
            //start all 0s, then fix the central ones
            cell->isAlive = 0;
            for(i=0;i<C;i++){
                for(j=0;j<R;j++){
                    sq_cpy( CELLS[i][j], cell );
                }
            }
            //make a PULSAR
			//BR quarter
            CELLS[ccent+2][rcent+1]->isAlive = 1;
            CELLS[ccent+3][rcent+1]->isAlive = 1;
            CELLS[ccent+4][rcent+1]->isAlive = 1; // North
            CELLS[ccent+1][rcent+2]->isAlive = 1;
            CELLS[ccent+1][rcent+3]->isAlive = 1;
            CELLS[ccent+1][rcent+4]->isAlive = 1; // West
            CELLS[ccent+2][rcent+6]->isAlive = 1;
            CELLS[ccent+3][rcent+6]->isAlive = 1;
            CELLS[ccent+4][rcent+6]->isAlive = 1; // South
            CELLS[ccent+6][rcent+2]->isAlive = 1;
            CELLS[ccent+6][rcent+3]->isAlive = 1;
            CELLS[ccent+6][rcent+4]->isAlive = 1; // East
			//BL quarter
            CELLS[ccent-2][rcent+1]->isAlive = 1;
            CELLS[ccent-3][rcent+1]->isAlive = 1;
            CELLS[ccent-4][rcent+1]->isAlive = 1; // North
            CELLS[ccent-1][rcent+2]->isAlive = 1;
            CELLS[ccent-1][rcent+3]->isAlive = 1;
            CELLS[ccent-1][rcent+4]->isAlive = 1; // West
            CELLS[ccent-2][rcent+6]->isAlive = 1;
            CELLS[ccent-3][rcent+6]->isAlive = 1;
            CELLS[ccent-4][rcent+6]->isAlive = 1; // South
            CELLS[ccent-6][rcent+2]->isAlive = 1;
            CELLS[ccent-6][rcent+3]->isAlive = 1;
            CELLS[ccent-6][rcent+4]->isAlive = 1; // East
			//TL quarter
            CELLS[ccent-2][rcent-1]->isAlive = 1;
            CELLS[ccent-3][rcent-1]->isAlive = 1;
            CELLS[ccent-4][rcent-1]->isAlive = 1; // North
            CELLS[ccent-1][rcent-2]->isAlive = 1;
            CELLS[ccent-1][rcent-3]->isAlive = 1;
            CELLS[ccent-1][rcent-4]->isAlive = 1; // West
            CELLS[ccent-2][rcent-6]->isAlive = 1;
            CELLS[ccent-3][rcent-6]->isAlive = 1;
            CELLS[ccent-4][rcent-6]->isAlive = 1; // South
            CELLS[ccent-6][rcent-2]->isAlive = 1;
            CELLS[ccent-6][rcent-3]->isAlive = 1;
            CELLS[ccent-6][rcent-4]->isAlive = 1; // East
			//TR quarter
            CELLS[ccent+2][rcent-1]->isAlive = 1;
            CELLS[ccent+3][rcent-1]->isAlive = 1;
            CELLS[ccent+4][rcent-1]->isAlive = 1; // North
            CELLS[ccent+1][rcent-2]->isAlive = 1;
            CELLS[ccent+1][rcent-3]->isAlive = 1;
            CELLS[ccent+1][rcent-4]->isAlive = 1; // West
            CELLS[ccent+2][rcent-6]->isAlive = 1;
            CELLS[ccent+3][rcent-6]->isAlive = 1;
            CELLS[ccent+4][rcent-6]->isAlive = 1; // South
            CELLS[ccent+6][rcent-2]->isAlive = 1;
            CELLS[ccent+6][rcent-3]->isAlive = 1;
            CELLS[ccent+6][rcent-4]->isAlive = 1; // East
            break;
		case GLIDER_GUN:
            //start all 0s, then fix the central ones
            cell->isAlive = 0;
            for(i=0;i<C;i++){
                for(j=0;j<R;j++){
                    sq_cpy( CELLS[i][j], cell );
                }
            }
            //make a GLIDER_GUN
			//Right side
            CELLS[ccent+2][rcent+0]->isAlive = 1;
            CELLS[ccent+2][rcent-1]->isAlive = 1;
            CELLS[ccent+2][rcent-2]->isAlive = 1;
            CELLS[ccent+3][rcent+0]->isAlive = 1;
            CELLS[ccent+3][rcent-1]->isAlive = 1;
            CELLS[ccent+3][rcent-2]->isAlive = 1;
            CELLS[ccent+4][rcent+1]->isAlive = 1;
            CELLS[ccent+4][rcent-3]->isAlive = 1;
            CELLS[ccent+6][rcent+1]->isAlive = 1;
            CELLS[ccent+6][rcent+2]->isAlive = 1;
            CELLS[ccent+6][rcent-3]->isAlive = 1;
            CELLS[ccent+6][rcent-4]->isAlive = 1;
			//Right square
            CELLS[ccent+16][rcent-1]->isAlive = 1;
            CELLS[ccent+17][rcent-1]->isAlive = 1;
            CELLS[ccent+16][rcent-2]->isAlive = 1;
            CELLS[ccent+17][rcent-2]->isAlive = 1;
			//Left side	
            CELLS[ccent-1][rcent+1]->isAlive = 1;
            CELLS[ccent-2][rcent+1]->isAlive = 1;
            CELLS[ccent-2][rcent+0]->isAlive = 1;
            CELLS[ccent-2][rcent+2]->isAlive = 1;
            CELLS[ccent-3][rcent-1]->isAlive = 1;
            CELLS[ccent-3][rcent+3]->isAlive = 1;
            CELLS[ccent-4][rcent+1]->isAlive = 1;
            CELLS[ccent-5][rcent+4]->isAlive = 1;
            CELLS[ccent-5][rcent-2]->isAlive = 1;
            CELLS[ccent-6][rcent+4]->isAlive = 1;
            CELLS[ccent-6][rcent-2]->isAlive = 1;
            CELLS[ccent-7][rcent+3]->isAlive = 1;
            CELLS[ccent-7][rcent-1]->isAlive = 1;
            CELLS[ccent-8][rcent+0]->isAlive = 1;
            CELLS[ccent-8][rcent+1]->isAlive = 1;
            CELLS[ccent-8][rcent+2]->isAlive = 1;
			//Left square
            CELLS[ccent-17][rcent+0]->isAlive = 1;
            CELLS[ccent-17][rcent+1]->isAlive = 1;
            CELLS[ccent-18][rcent+0]->isAlive = 1;
            CELLS[ccent-18][rcent+1]->isAlive = 1;
			break;
		default:
			printf("Error, was not expecting to get here in init_field\n");
			free(cell);
			exit(1);
	}
	free(cell);
	return;
}

/* Save the memories */
void free_CELLS(){
	int i, j;
	for(i=0;i<R;i++){
		for(j=0;j<C;j++){
			free(CELLS[i][j]);
		}
	}
	return;
}

/* Has each cell think and then has each cell update.
	To be multithreaded and written better. */
void update_cells(){
	int i, j;
	gol_sq * nbrs = malloc(sizeof(gol_sq)*8);
	if(nbrs==NULL){
		printf("Error, malloc returned NULL.\n");
		exit(1);
	}
	//think
	int U1, D1, L1, R1; //i,j value for up 1, down 1, left 1, and right 1
    for(i=0;i<C;i++){
		for(j=0;j<R;j++){
			if (i==0)
				D1 = C-1;
			else 
				D1 = i-1;
			if (i == C-1)
				U1 = 0;
			else
				U1 = i+1;			
			if (j == 0)
				L1 = R-1;
			else
				L1 = j-1;
			if (j == R-1)
				R1 = 0;
			else
				R1 = j+1;

		//	printf("U1 %i D1 %i L1 %i R1 %i X %i Y %i\n",U1,D1,L1,R1,i,j);

			sq_cpy( &nbrs[0], CELLS[U1][L1]);
			sq_cpy( &nbrs[1], CELLS[U1][j]);
			sq_cpy( &nbrs[2], CELLS[U1][R1]);
			sq_cpy( &nbrs[3], CELLS[i][L1]);
			sq_cpy( &nbrs[4], CELLS[i][R1]);
			sq_cpy( &nbrs[5], CELLS[D1][L1]);
			sq_cpy( &nbrs[6], CELLS[D1][j]);
			sq_cpy( &nbrs[7], CELLS[D1][R1]);

			think( CELLS[i][j], nbrs );
		}
	}
	//update
    for(i=0;i<C;i++){
		for(j=0;j<R;j++){
		    update_cell( CELLS[i][j] );
		}
	}
	return;
}

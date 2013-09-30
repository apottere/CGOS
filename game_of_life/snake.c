#include "snake.h"

void init_snakes() {
	int i;
	SNAKES = malloc(sizeof(snake*)*NUM_SNAKES);
	PDIR = malloc(sizeof(enum DIRS)*NUM_SNAKES);
	for(i = 0; i < NUM_SNAKES; i++) {
		PDIR[i] = UP;
		SNAKES[i] = make_snake(5, (i*4)+2, R-10, UP);
	}
}


/* return a snake with length len and head at (start_X, start_Y), 
	trailing off to the left of the head, and heading RIGHT */
snake * make_snake(int len, int start_X, int start_Y, enum DIRS dir){
	//error checking
	if( len > C ){
		printf("Error, snake is too large for the playing field. len > C\n");
		exit(1);
	}
	if( start_Y > R ){
		printf("Error, snake's start_Y is greater than the number of rows in the field.\n");
		exit(1);
	}

	int i;
	snake * s = malloc(sizeof(snake));
	if(s == NULL){
		printf("Error, insufficient memory for a snake.\n");
		exit(1);
	}
	s->isAlive = 1;
	s->len = len;
	//allocate ALL the space we could possibly need
	s->body = malloc(MAX_SNAKE_LENGTH*sizeof(int*));
	if(s->body == NULL){
		printf("Error, insufficient memory for a snake's body.\n");
		exit(1);
	}
	//also for the body segments
	for(i=0;i<MAX_SNAKE_LENGTH;i++){
		s->body[i] = malloc(2*sizeof(int));
		if(s->body[i] == NULL){
			printf("Error, insufficient memory for a snake's body segment.\n");
			exit(1);
		}
	}
	//now set up the body of the snake as it starts out
	for(i=0;i<len;i++){
		s->body[i][0] = start_X; //X
		s->body[i][1] = start_Y+i; //Y
	}
	// if the snake is now in negative space due to starting position, wrap it around 
	if( start_Y - len + 1 < 0 ){
		int dist = len - start_Y - 1; 
		for(i=0;i<dist;i++){
			s->body[len-1-i][1] += R;
		}
	}
	s->curr_dir = dir;
	s->isDying = 0;
	s->timeOfDeath = -1;
	s->deathMsg = malloc(15*sizeof(char));
	if( s->deathMsg == NULL ){
		printf("Error, insufficient memory for death message\n");
		exit(1);
	}
	return s;
}

void move_all_snakes() {
	int n;
	for(n = 0; n < NUM_SNAKES; n++) {
		if(SNAKES[n]->isAlive) {
			move_snake(SNAKES[n], PDIR[n]);
		}
	}
}


/* move the snake s in the direction dir */
void move_snake( snake *s, enum DIRS dir ){

	/* If not supposed to move, don't move */
	if(dir==STOP)
		return;

	int i;
	/* //not sure what this does...?
	if(s->curr_dir == STOP) {
		s->curr_dir = dir;
	}
	*/
	
	enum DIRS cdir = s->curr_dir;
	
	if(cdir != STOP){ 
		/* if move is opposite of current direction, 
			result is no change in direction */
		if( cdir == RIGHT && dir == LEFT){
			dir = RIGHT;
		}
		else if( cdir == LEFT && dir == RIGHT ){
			dir = LEFT;
		}
		else if( cdir == DOWN && dir == UP ){
			dir = DOWN;
		}
		else if( cdir == UP && dir == DOWN ){
			dir = UP;
		}
		//or we've legitimately changed (or kept) our direction
		else{
			s->curr_dir = dir;
		}
	
		/* copy the snake's body (not head) down 1 */
		for(i=s->len-1; i > 0; i--){
			s->body[i][0] = s->body[i-1][0];
			s->body[i][1] = s->body[i-1][1];
		}
		/* now move the head */
		switch(dir){
			case UP:
				s->body[0][0] = s->body[1][0];
				s->body[0][1] = s->body[1][1]-1;
				break;
			case DOWN:
				s->body[0][0] = s->body[1][0];
				s->body[0][1] = s->body[1][1]+1;
				break;
			case LEFT:
				s->body[0][0] = s->body[1][0]-1;
				s->body[0][1] = s->body[1][1];
				break;
			case RIGHT:
				s->body[0][0] = s->body[1][0]+1;
				s->body[0][1] = s->body[1][1];
				break;
			default:
				printf("Invalid value for dir, snake not moving\n");
				break;
			}	
	
		//left overflow
		if( s->body[0][0] < 0 ){
			//printf("Left overflow, adjusting head\n");
			s->body[0][0] = C-1;
		}
		//right overflow
		if( s->body[0][0] >= C ){
			//printf("Right overflow, adjusting head\n");
			s->body[0][0] = 0;
		}
		//down overflow
		if( s->body[0][1] < 0 ){
			//printf("Down overflow, adjusting head\n");
			s->body[0][1] = R-1;
		}
		//up overflow
		if( s->body[0][1] >= R ){
			//printf("Up overflow, adjusting head\n");
			s->body[0][1] = 0;
		}
	}
}


/* free the snake's memory */
void free_snake(snake * s){
	int i;
	for(i=0; i < s->len; i++){
		free(s->body[i]);
	}
	free(s->body);
}

void test_all_snakes() {
	int i;
	int num_dying_snakes = 0;
	snake * s;
	for(i = 0; i < NUM_SNAKES; i++) {
		if(SNAKES[i]->isAlive) {
			test_snake(SNAKES[i]);
		}
	}
	test_snake2snake_collisions();
	//any snakes dead in this round have had
	//isDying set to 1. clean up the bodies.
	for(i=0; i < NUM_SNAKES; i++){
		s = SNAKES[i];
		if( s->isDying == 1 ){
			num_dying_snakes++;
			s->isAlive = DEAD;
			s->timeOfDeath = NUM_TICKS;
			//printf("  Dead %d: %i, Left: %d\n", i, s->timeOfDeath, NUM_LIVE_SNAKES);
			s->isDying = 0;
			gtk_widget_modify_bg(INDICATOR[i], GTK_STATE_NORMAL, &red);
		}
	}
	NUM_LIVE_SNAKES -= num_dying_snakes;
}

// Test snake positions against CELLS, 1 is good, 0 collision
void test_snake(snake *s) {
	int i;
	for(i = 0; i < s->len; i++) {
		if(CELLS[s->body[i][0]][s->body[i][1]]->isAlive == 1) {
			s->isDying = 1;
		}
	}
}

void test_snake2snake_collisions(){
	int i,j,k;
	snake * curr_snake;
	snake * s;
	//for each snake
	for(i=0; i < NUM_SNAKES; i++){
		curr_snake = SNAKES[i];
		//don't bother with dead snakes
		if( curr_snake->isAlive == DEAD )
			continue;
		//for each OTHER snake (including itself: self collision) 
		for(j=0; j < NUM_SNAKES; j++){
			s = SNAKES[j];	
			//don't compare against dead snakes
			if( s->isAlive == DEAD )
				continue;
			//is the head of curr_snake somewhere in snake s?
			for(k=0; k < s->len; k++){
				//if curr_snake IS s, then the head is allowed to 
				//be "in" the head -- it is itself
				if(i==j && k==0)
					continue;
				//note that they're dying, don't kill them
				//otherwise they disappear from the running
				else if ( curr_snake->body[0][0] == s->body[k][0] && 
					 curr_snake->body[0][1] == s->body[k][1] ){
					curr_snake->isDying = 1; 
				}
			}
		}
	}
}

void incr_snakes(){
	int i, len;
	snake * s;
	for(i=0; i < NUM_SNAKES; i++){
		s = SNAKES[i];
		//only increment if it's not too long and it's alive
		if( s->len < MAX_SNAKE_LENGTH && s->isAlive == ALIVE ){
			s->len++;
			len = s->len;
			s->body[len-1][0] = s->body[len-2][0];
			s->body[len-1][1] = s->body[len-2][1];
		}
	}
}


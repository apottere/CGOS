#include "map.h"

// Draw the snake, assumes position has been checked.
void draw_snakes() {
	int n;

	// Draw bodies.
	for(n = 0; n < NUM_SNAKES; n++) {
		if(SNAKES[n]->isAlive) {
			int i;
			int len = SNAKES[n]->len;
			for(i = 1; i < len; i++) {
				gtk_image_set_from_pixbuf(GTK_IMAGE(FIELD[SNAKES[n]->body[i][0]][SNAKES[n]->body[i][1]]), IMAGE[2]);
			}
		}
	}
	
	char *buffer = malloc(8*sizeof(int) + 1);
	sprintf(buffer, "%d", NUM_TICKS);
	// Draw heads.
	for(n = 0; n < NUM_SNAKES; n++) {
		if(SNAKES[n]->isAlive) {
			gtk_image_set_from_pixbuf(GTK_IMAGE(FIELD[SNAKES[n]->body[0][0]][SNAKES[n]->body[0][1]]), IMAGE[(n%(NUM_IMAGES-3))+3]);
			
			gtk_label_set_text(GTK_LABEL(POINTS[n]), buffer);
		}
	}
	
}



// Sets and displays pictures in FIELD based on the values in CELLS.
void set_gui(){
	int i,j;
	for(i = 0; i < C; i++) {
		for(j = 0; j < R; j++) {
	//TODO indexing
			gtk_image_set_from_pixbuf(GTK_IMAGE(FIELD[i][j]), IMAGE[LOGICAL_CELLS[i][j]]);
		}
	}
}

void countdown() {
	
	char *message = "Game starting in: ";
	char *number;
	char *full;
	full = malloc((sizeof(strlen(message)) + sizeof(int)*8+1));
	number = malloc((sizeof(int)*8+1));
	int i;
	for(i = 5; i > 0 && !EXIT; i--) {

		strcpy(full, message);
		sprintf(number, "%d", i);
		strcat(full, number);
		
		gtk_label_set_text(GTK_LABEL(message_label), full);
		sleep(1);
	}
	//toggle_state();
	RUNNING = 1;
	gtk_label_set_text(GTK_LABEL(message_label), "Good Luck!");
}



// Toggle the RUNNING state. (Button callback function).
void toggle_state() {

	if(RUNNING) {
		RUNNING = 0;
		gtk_label_set_text(GTK_LABEL(message_label), "Press 'u' to begin!");
	} else {
		RUNNING = 1;
		gtk_label_set_text(GTK_LABEL(message_label), "Good Luck!");
	} 
}

// Handle keyboard events.
void key_event(GtkWidget *widget, GdkEventKey *kevent, gpointer data) {
	

	// "q" key: Stop program.
	if(kevent->type == 8 && kevent->keyval == 'q' && EXIT) {
			QUIT = 1;

	// Handle key down events.
	} else if(kevent->type == 8 && !EXIT) {
		/*
		// "o" key: RUNNING while key is pressed.
		if(kevent->keyval == 'o' && !RUNNING) {
			toggle_state();
		
		// "u" key: Start game.
		} else if(kevent->keyval == 'u' && !RUNNING && !STEP) {
			START = 1;

		// "i" key: Toggle RUNNING state.
		} else if(kevent->keyval == 'i') {
			toggle_state();

		// "p" key: Step once.
		} else if(kevent->keyval == 'p' && !STEP) {
			STEP = 1;
		*/
		// "w" key: Snake dir up.
		if(kevent->keyval == 'w') {
			if(PDIR[0] != UP) {
				PDIR[0] = UP;
			}

		// "s" key: Snake dir down.
		} else if(kevent->keyval == 's') {
			if(PDIR[0] != DOWN) {
                PDIR[0] = DOWN;
            }

		// "a" key:  Snake dir left.
		} else if(kevent->keyval == 'a') {
			if(PDIR[0] != LEFT) {
                PDIR[0] = LEFT;
            }

		// "d" key:  Snake dir right.
		} else if(kevent->keyval == 'd') {
			if(PDIR[0] != RIGHT) {
                PDIR[0] = RIGHT;
            }

		// "up" key: Snake dir up.
        } else if(kevent->keyval == 65362) {
            if(PDIR[1] != UP) {
                PDIR[1] = UP;
            }

        // "down" key: Snake dir down.
        } else if(kevent->keyval == 65364) {
            if(PDIR[1] != DOWN) {
                PDIR[1] = DOWN;
            }

        // "left" key:  Snake dir left.
        } else if(kevent->keyval == 65361) {
            if(PDIR[1] != LEFT) {
                PDIR[1] = LEFT;
            }

        // "right" key:  Snake dir right.
        } else if(kevent->keyval == 65363) {
            if(PDIR[1] != RIGHT) {
                PDIR[1] = RIGHT;
            }

		// "w" key: Snake dir up.
        } else if(kevent->keyval == 'y') {
            if(PDIR[2] != UP) {
                PDIR[2] = UP;
            }

        // "s" key: Snake dir down.
        } else if(kevent->keyval == 'h') {
            if(PDIR[2] != DOWN) {
                PDIR[2] = DOWN;
            }

        // "a" key:  Snake dir left.
        } else if(kevent->keyval == 'g') {
            if(PDIR[2] != LEFT) {
                PDIR[2] = LEFT;
            }

        // "d" key:  Snake dir right.
        } else if(kevent->keyval == 'j') {
            if(PDIR[2] != RIGHT) {
                PDIR[2] = RIGHT;
            }
		}
	// Handle key up events.
	} /* else if (kevent->type == 9) {
		
		//printf("Key Pressed: %d, %c, %d;\n", kevent->keyval, kevent->keyval, kevent->type);
		if(kevent->keyval == 'o' && !START && !EXIT) {
			RUNNING = 0;
		}
	} else {
		//printf("Invalid type.\n");
	} */
}

void close_game(enum QUIT qt) {
	
	//g_print("Recieved exit request from ");
	switch(qt){
	case WINDOW:
		if(EXIT) {
			QUIT = 1;
			return;
		}
		g_print("Recieved exit request from ");
		g_print("window manager...\n");
		break;
/*	case Q_KEY:
		g_print("'q' key event");
		break;
	case P1:
		g_print("Player 1");
		break;
	case P2:
		g_print("Player 2");
		break; */
	case GAME_OVER:
		g_print("Recieved exit request from ");
		g_print("game over...\n");

		int i;
		int isWinner = 0;
		for(i = 0; i < NUM_SNAKES; i++) {
			if(SNAKES[i]->isAlive == ALIVE) {
				isWinner = 1;
				//gtk_widget_modify_bg(INDICATOR[i], GTK_STATE_NORMAL, &green);
			}
		}
		if(isWinner) {
			gtk_label_set_text(GTK_LABEL(message_label), "We have a winner!  Press q to quit.");

		} else {
			gtk_label_set_text(GTK_LABEL(message_label), "The match was a draw!  Press q to quit.");
		}

		EXIT = 1;
		while(!QUIT) {
			usleep(100000);
		}
		break;
	default:
		g_print("ERROR");
	}
	
	gtk_widget_hide(window);
	
	// Display shutdown progress.
	g_print("Exit in progress:\n");
	g_print("  - Waiting for worker to finish..");
	
	EXIT = 1;
	RUNNING = 0;

	if(qt == WINDOW) {
		// Wait for worker to complete.
		while(!WORKER_FINISHED) {
			g_print(".");
			while( gtk_events_pending() ) {
				gtk_main_iteration();
			}
			usleep(100000);
		}
	}
		
	g_print("Done.\n");
	// Exit GTK.
	g_print("  - Main exiting...");
	gtk_main_quit();
	printf("Done.\nHave a nice day!\n");
	
}

// Called from window manager, shuts down GUI.
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	
	close_game( WINDOW );

	return FALSE;
}


// Initialize the GUI and return the window to caller.
// WARNING: CELLS must be allocated and initialized before this call.
GtkWidget* make_gui() {

	GdkColor red = { 1, 0xD600, 0x9B00, 0x9B00 };
	GdkColor gray = { 1, 0xE800, 0xE800, 0xE800 };
	GdkColor dgray = { 1, 0xD200, 0xD200, 0xD200 };
	GdkColor green = { 1, 0x6300, 0xDC00, 0x8300 };
	// Load pictures from local directory/imgs.
	load_images();

	// Local variables for creating the GUI.
	int i, j;
	GtkWidget *window;
	GtkWidget *table_vbox;
	GtkWidget *base_hbox;
	GtkWidget *message_align;
	GtkWidget *message_event_box;
	GtkWidget *message_event_box_white;
	GtkWidget *table_align;
	GtkWidget *table_event_box;
	GtkWidget *board_event_box_align;
	GtkWidget *board_event_box_white_align;
	GtkWidget *board_event_box;
	GtkWidget *board_event_box_white;
	GtkWidget *board_vbox;
	GtkWidget *player_hbox;
	GtkWidget *player_label;
	GtkWidget *player_pixbuf;
	GtkWidget *player_align;
	INDICATOR = malloc(sizeof(GtkWidget*)*NUM_SNAKES);
	POINTS = malloc(sizeof(GtkWidget*)*NUM_SNAKES);
	char *buffer = malloc(8 + sizeof(int)*8 + 1 + 1);

	// Create GUI widgets.
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	base_hbox = gtk_hbox_new(FALSE,0);
	table = gtk_table_new(R, C, TRUE);
	table_vbox = gtk_vbox_new(FALSE,0);
	message_label = gtk_label_new("Press 'u' to begin!");
	message_event_box_white = gtk_event_box_new();
	message_event_box = gtk_event_box_new();
	message_align = gtk_alignment_new(.5, .5, 0, 0);
	table_align = gtk_alignment_new(.5, .5, 0, 0);
	table_event_box = gtk_event_box_new();
	board_event_box_align = gtk_alignment_new(.5, .5, 0, 0);
	board_event_box_white_align = gtk_alignment_new(.5, .5, 0, 0);
	board_event_box = gtk_event_box_new();
	board_event_box_white = gtk_event_box_new();
	board_vbox = gtk_vbox_new(FALSE, 5);
	
	// Initialize memory for FIELD, set each image to white, and attach to table.
	// WARNING: CELLS must be allocated and initialized before this call.
	FIELD = malloc(sizeof(GtkWidget**)*C);
	for(i = 0; i <  C; i++) {
		FIELD[i] = malloc(sizeof(GtkWidget*)*R);
			for(j = 0; j <  R; j++) {
			//TODO fix this up
				FIELD[i][j] = gtk_image_new_from_pixbuf(IMAGE[LOGICAL_CELLS[i][j]]);
				gtk_table_attach(GTK_TABLE(table), FIELD[i][j], i, i+1, j, j+1, GTK_FILL, GTK_FILL, 0, 0);
			gtk_widget_show(FIELD[i][j]);
		}
	}
	
	
	// Set GUI widget attributes.
	// Window.
	gtk_window_set_title(GTK_WINDOW(window), "Conway's Game Of Snake");
	gtk_window_set_icon(GTK_WINDOW(window), gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/icon4.png"))));
	g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), "window manager");
	g_signal_connect(GTK_WINDOW(window), "key_press_event", G_CALLBACK(key_event), NULL);
	g_signal_connect(GTK_WINDOW(window), "key_release_event", G_CALLBACK(key_event), NULL);
	
	// Message label.
	gtk_widget_set_size_request(message_label, 5*C, 30);
	gtk_widget_modify_bg(message_event_box_white, GTK_STATE_NORMAL, &gray);
	gtk_widget_modify_bg(message_event_box, GTK_STATE_NORMAL, &dgray);
	gtk_widget_set_size_request(board_event_box_white, 200, -1);

	// Table.
	gtk_widget_modify_bg(table_event_box, GTK_STATE_NORMAL, &dgray);

	// Scoreboard.
	gtk_widget_modify_bg(board_event_box_white, GTK_STATE_NORMAL, &gray);
	gtk_widget_modify_bg(board_event_box, GTK_STATE_NORMAL, &dgray);


	// Set GUI widget borders.
	gtk_container_set_border_width(GTK_CONTAINER(board_event_box_white), 10);
	gtk_container_set_border_width(GTK_CONTAINER(message_event_box_white), 10);
	gtk_container_set_border_width(GTK_CONTAINER(table), 10);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_container_set_border_width(GTK_CONTAINER(message_event_box), 5);
	gtk_container_set_border_width(GTK_CONTAINER(table_event_box), 5);
	gtk_container_set_border_width(GTK_CONTAINER(board_event_box), 5);
	gtk_container_set_border_width(GTK_CONTAINER(board_vbox), 5);


	// Build player list.
	for(i = 0; i < NUM_SNAKES; i++) {
		
		player_align = gtk_alignment_new(.5, .5, 1, 1);
		player_hbox = gtk_hbox_new(FALSE, 0);
		INDICATOR[i] = gtk_event_box_new();
		gtk_widget_modify_bg(INDICATOR[i], GTK_STATE_NORMAL, &gray);

		player_pixbuf = gtk_image_new_from_pixbuf(IMAGE[(i%(NUM_IMAGES-3))+3]);
		//player_pixbuf = gtk_image_new_from_pixbuf(IMAGE[i]);

		if(i+1 < 10) {
			sprintf(buffer, "Player %d:   ", i+1);
		} else {
			sprintf(buffer, "Player %d: ", i+1);
		}
		player_label = gtk_label_new(buffer);

		POINTS[i] = gtk_label_new("0");
		gtk_widget_set_size_request(POINTS[i], 55, -1);

		gtk_box_pack_start(GTK_BOX(player_hbox), player_pixbuf, TRUE, TRUE, 5);
		gtk_box_pack_start(GTK_BOX(player_hbox), player_label, TRUE, TRUE, 5);
		gtk_box_pack_start(GTK_BOX(player_hbox), POINTS[i], TRUE, TRUE, 5);
		gtk_container_add(GTK_CONTAINER(INDICATOR[i]), player_hbox);
		gtk_container_add(GTK_CONTAINER(player_align), INDICATOR[i]);
		gtk_box_pack_start(GTK_BOX(board_vbox), player_align, TRUE, TRUE, 0);

		gtk_widget_show(player_align);
		gtk_widget_show(player_pixbuf);
		gtk_widget_show(player_label);
		gtk_widget_show(POINTS[i]);
		gtk_widget_show(INDICATOR[i]);
		gtk_widget_show(player_hbox);
	}



	// Construct GUI.
	gtk_container_add(GTK_CONTAINER(message_event_box_white), message_label);
	gtk_container_add(GTK_CONTAINER(message_event_box), message_event_box_white);
	gtk_container_add(GTK_CONTAINER(message_align), message_event_box);
	gtk_container_add(GTK_CONTAINER(table_event_box), table);
	gtk_container_add(GTK_CONTAINER(table_align), table_event_box);
	gtk_box_pack_start(GTK_BOX(table_vbox), table_align, TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(table_vbox), message_align, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(board_event_box_white), board_vbox);
	gtk_container_add(GTK_CONTAINER(board_event_box_white_align), board_event_box_white);
	gtk_container_add(GTK_CONTAINER(board_event_box), board_event_box_white_align);
	gtk_container_add(GTK_CONTAINER(board_event_box_align), board_event_box);
	gtk_box_pack_start(GTK_BOX(base_hbox), board_event_box_align, TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(base_hbox), table_vbox, TRUE, TRUE, 0); 
	gtk_container_add(GTK_CONTAINER(window), base_hbox);

	// Show GUI widgets.
	gtk_widget_show(message_event_box_white);
	gtk_widget_show(message_event_box);
	gtk_widget_show(message_align);
	gtk_widget_show(board_event_box_white_align);
	gtk_widget_show(board_event_box_align);
	gtk_widget_show(board_vbox);
	gtk_widget_show(board_event_box);
	gtk_widget_show(board_event_box_white);
	gtk_widget_show(table_align);
	gtk_widget_show(table_event_box);
	gtk_widget_show(table_vbox);
	gtk_widget_show(base_hbox);
	gtk_widget_show(message_label);
	gtk_widget_show(table);

	return window;
}

void wipe_board(){
	int i,j;
	for(i=0;i<C;i++){
		for(j=0;j<R;j++){
	//TODO fix this up
			LOGICAL_CELLS[i][j] = DEAD;
		}
	}
}

void show_winner(int winner){
	
}

void load_images() {

	IMAGE[0] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/white5.png")));
	IMAGE[1] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/black5.png")));
	IMAGE[2] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/dgreen5.png")));
	IMAGE[3] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/cyan5.png")));
	IMAGE[4] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/red5.png")));
	IMAGE[5] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/yellow5.png")));
	IMAGE[6] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/gold5.png")));
	IMAGE[7] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/green5.png")));
	IMAGE[8] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/greenish5.png")));
	IMAGE[9] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/indigo5.png")));
	IMAGE[10] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/lgreen5.png")));
	IMAGE[11] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/lpurple5.png")));
	IMAGE[12] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/orange25.png")));
	IMAGE[13] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/puce5.png")));
	IMAGE[14] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/purple5.png")));
	IMAGE[15] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/blue5.png")));
	IMAGE[16] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/teal5.png")));
	IMAGE[17] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/vomit5.png")));
	IMAGE[18] = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file("imgs/beige5.png")));

}

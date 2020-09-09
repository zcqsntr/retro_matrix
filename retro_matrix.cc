

#include "led-matrix.h"
#include "retro_matrix.h"

#include "graphics.h"
#include "mylib.h"

#include <getopt.h>
#include <string>

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h> 
#include <iostream>
#include <vector> 


#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/input.h>
#include <cstdlib>
#include "libevdev.h"

using rgb_matrix::Font;
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Color;

using namespace std;


RetroMatrix::RetroMatrix(rgb_matrix::Canvas* cv, int rows, int cols) {
		
		n_rows = rows;
		n_cols = cols;
		canvas = cv;
		brightness = 1;
		
		
		
	  // make buttons 
	  Button P1_button{Point{2,2}, "1 PLAYER", true};
	  Button P2_button{Point{25,2}, "2 PLAYER", false};
	  
	  vector<Button> snake_buttons{P1_button, P2_button};
	  
	  // make buttons 
	  Button snake_button{Point{2,2}, "SNAKE", true};
	  Button tetris_button{Point{13,2}, "TETRIS", false};
	  Button space_button{Point{25,2}, "SPACE INVADERS", false};
	  
	  vector<Button> games_buttons{snake_button, tetris_button, space_button};
	  
	  // make buttons 
	  Button GOL_button{Point{2,2}, "GAME OF LIFE", true};
	  Button R30_button{Point{13,2}, "RULE 30", false};
	  Button ant_button{Point{25,2}, "LANGTON'S ANT", false};
	  Button loop_button{Point{36,2}, "LANGTON'S LOOP", false};
	  
	  vector<Button> CA_buttons{GOL_button, R30_button, ant_button, loop_button};
	  
	  
	  
	  // make buttons 
	  Button games_button{Point{2,2}, "GAMES", true};
	  Button automata_button{Point{13,2}, "AUTOMATA", false};
	  Button shutdown_button{Point{25,2}, "SHUTDOWN", false};
	  
	  
	  
	  vector<Button> main_buttons{games_button, automata_button, shutdown_button};
  
  
		
	menu_structure["MAIN"] = main_buttons;
	menu_structure["AUTOMATA"] = CA_buttons;
	menu_structure["GAMES"] = games_buttons;
	menu_structure["SNAKE"] = snake_buttons;
	
	cout << main_buttons.size() << CA_buttons.size()<< games_buttons.size() << snake_buttons.size() << endl;
	
	cout <<menu_structure["MAIN"].size()<< endl;
		
}
		


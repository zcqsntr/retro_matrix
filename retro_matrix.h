#ifndef RM_H
#define RM_H

#include "led-matrix.h"
#include "libevdev.h"
#include <vector> 
#include <map> 
#include "mylib.h"

using rgb_matrix::Canvas;
class RetroMatrix {
	public:
		int n_rows;
		int n_cols;
		float brightness;
		Canvas *canvas;
		
		
		struct libevdev *dev;
		struct libevdev *dev1;
		
		
		void draw_menu(vector<Button> buttons);
		
	
		int start_menu();
		
		int run_snake(int n_players);
		void one_p_snake();
		void two_p_snake();
		int run_tetris();
		int run_space_invaders();
		
		int run_GOL();
		int run_R30();
		int run_ant();
		int run_loop();
		int draw_weather();
		
		std::map<std::string, std::vector<Button>> menu_structure;
		
		RetroMatrix(rgb_matrix::Canvas *canvas, int rows, int cols);
		
	
		
};



#endif

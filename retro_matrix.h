#ifndef RM_H
#define RM_H

#include "led-matrix.h"
#include "libevdev.h"
#include <vector> 

using rgb_matrix::Canvas;
class RetroMatrix {
	public:
		int n_rows;
		int n_cols;
		float brightness;
		Canvas *canvas;
		
		
		struct libevdev *dev;
		struct libevdev *dev1;
		
		void draw_main_menu();
		
		int ca_menu();
		int games_menu();
		int snake_menu();
		int start_menu();
		
		int run_snake(int n_players);
		void one_p_snake();
		void two_p_snake();
		int run_tetris();
		
		int run_GOL();
		int run_R30();
		int run_ant();
		
		RetroMatrix(rgb_matrix::Canvas *canvas, int rows, int cols);
		
	
		
};



#endif



#include "led-matrix.h"
#include "retro_matrix.h"

#include "graphics.h"
#include "mylib.h"
#include "game_of_life.h"
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
}
		


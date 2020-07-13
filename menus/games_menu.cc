
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


int RetroMatrix::games_menu() {

  int current_selected = 0;
  
         

         
    
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }
  
  // make buttons 
  Button snake_button{Point{2,2}, "SNAKE", true};
  Button tetris_button{Point{25,2}, "TETRIS", false};
  
  vector<Button> buttons{snake_button, tetris_button};
  

  
  
  Color bg_color(0, 0, 0);
  Color bright_color(150*brightness, 0, 255*brightness);
  Color dim_color(70*brightness, 0, 150*brightness);
  Color highlight_color(255*brightness,255*brightness,255*brightness);
  ResetCanvas(canvas, n_rows, n_cols, bg_color);
  
  int selected_button = get_selected_button(buttons);
  list <ControllerInput> inputs;
  draw_buttons(canvas, buttons, font, bright_color, dim_color);
  int fd;
  int rc;
  struct input_event ev;
  while(true){
  
      inputs = get_inputs_from_ps4(dev);
      
    
      
      for(const auto &input: inputs){
        switch(input.type) {  // go from first input as unlikely to have multiple inputs perframes with no sleep

          case 'y':
            change_selected_button(buttons, input.value, current_selected);
            draw_buttons(canvas, buttons, font, bright_color, dim_color);
            break;
            
          case 'E':
            if(input.value == 1){
              return 0;
            }
            break;
          case 'S':
            selected_button = get_selected_button(buttons);
            
            if(input.value == 1){
              switch (selected_button) {
                case 0:
                  snake_menu();
                  ResetCanvas(canvas, n_rows, n_cols, bg_color);
                  draw_buttons(canvas, buttons, font, bright_color, dim_color);
                  get_inputs_from_ps4(dev); //clear input buffer
                  break;
                case 1:
                  run_tetris();
                  ResetCanvas(canvas, n_rows, n_cols, bg_color);
                  draw_buttons(canvas, buttons, font, bright_color, dim_color);
                  get_inputs_from_ps4(dev); //clear input buffer
                  break;
                default:
                  cout<< "Unrocognised button index!";
                  break;
                
                  }
              }
              break;
             case 'D': // controller disconnect
              fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
              rc = libevdev_new_from_fd(fd, &dev);
              break;

            default:
              break;
            
        }
      }
}
      
}




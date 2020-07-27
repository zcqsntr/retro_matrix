
#include "led-matrix.h"
#include "graphics.h"
#include "retro_matrix.h"
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
#include <list>

using rgb_matrix::Font;
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Color;

using namespace std;



int RetroMatrix::start_menu() {
    cout << 1;
  int current_selected = 0;
 
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }
  cout << 2;
  // make buttons 
  Button resume_button{Point{2,2}, "RESUME", true};
  Button quit_button{Point{10,2}, "QUIT", false};
  
  vector<Button> buttons{resume_button, quit_button};
  
  int letter_spacing = 0;
cout << 3;
  
  Color bg_color(0, 0, 0);
  Color bright_color(150*brightness, 0, 255*brightness);
  Color dim_color(70*brightness, 0, 150*brightness);
  Color highlight_color(255*brightness,255*brightness,255*brightness);
  ResetCanvas(canvas, n_rows, n_cols, bg_color);

  
  list <ControllerInput> inputs;
  draw_buttons(canvas, buttons, font, bright_color, dim_color);
  int fd;
  int rc;
  struct input_event ev;
  cout << 4;
  while(true){
    
    
     
        inputs = get_inputs_from_ps4(dev);
      
    
   
      for(const auto &input: inputs){
        switch(input.type) {  // go from first input as unlikely to have multiple inputs perframes with no sleep

          case 'y':
            change_selected_button(buttons, input.value, current_selected);
            draw_buttons(canvas, buttons, font, bright_color, dim_color);
            break;
            
          case 'S':
          ResetCanvas(canvas, n_rows, n_cols, bg_color);
            return 0;
            
          case 'E':
            if(input.value == 1) {
              ResetCanvas(canvas, n_rows, n_cols, bg_color);
              return get_selected_button(buttons);// 0 for resume, 1 for quit
            }
          break;
          case 'D': // controller disconnect
              fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
              rc = libevdev_new_from_fd(fd, &dev);
          break;
        }
      }
  }
 
}




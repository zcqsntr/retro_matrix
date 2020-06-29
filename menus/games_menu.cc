
#include "led-matrix.h"
#include "graphics.h"
#include "snake_menu.h"
#include "tetris.h"

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


static void DrawGamesCanvas(Canvas *canvas) {
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   
  canvas->Fill(0, 0, 255);

  int center_x = canvas->width() / 2;
  int center_y = canvas->height() / 2;
  float radius_max = canvas->width() / 2;
  float angle_step = 1.0 / 360;
  for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
    if (interrupt_received)
      return;
    float dot_x = cos(a * 2 * M_PI) * r;
    float dot_y = sin(a * 2 * M_PI) * r;
    canvas->SetPixel(center_x + dot_x, center_y + dot_y,
                     255, 0, 0);
    usleep(1 * 1000);  // wait a little to slow down things.
  }
  * */
  
   
  
  struct libevdev *dev = NULL;
  int fd;
  int rc = 1;

  int current_selected = 0;
  // ps4 controller "/dev/input/event6",
  fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
  rc = libevdev_new_from_fd(fd, &dev);
  if (rc < 0) {
          fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
          exit(1);
  }
  printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
  printf("Input device ID: bus %#x vendor %#x product %#x\n",
         libevdev_get_id_bustype(dev),
         libevdev_get_id_vendor(dev),
         libevdev_get_id_product(dev));
         
         
  // filter out the constant events that flood the queue
  /*
  libevdev_disable_event_code	(dev, EV_ABS, ABS_X);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_Y);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_RX);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_RY);
  * */
         
    
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }
  
  // make buttons 
  Button snake_button{Point{2,2}, "SNAKE", true};
  Button tetris_button{Point{25,2}, "TETRIS", false};
  
  vector<Button> buttons{snake_button, tetris_button};
  

  
  Color bg_color(0, 0, 0);
  Color bright_color(150, 0, 255);
  Color dim_color(70, 0, 150);
  Color highlight_color(255,255,255);
  ResetCanvas(canvas, 32, 64, bg_color);
  int selected_button = get_selected_button(buttons);
  list <ControllerInput> inputs;
  draw_buttons(canvas, buttons, font, bright_color, dim_color);
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
              return;
            }
            break;
          case 'S':
            selected_button = get_selected_button(buttons);
            
            if(input.value == 1){
              switch (selected_button) {
                case 0:
                  snake_menu(canvas);
                  ResetCanvas(canvas, 32, 64, bg_color);
                  draw_buttons(canvas, buttons, font, bright_color, dim_color);
                  get_inputs_from_ps4(dev); //clear input buffer
                  break;
                case 1:
                  run_tetris(canvas);
                  ResetCanvas(canvas, 32, 64, bg_color);
                  draw_buttons(canvas, buttons, font, bright_color, dim_color);
                  get_inputs_from_ps4(dev); //clear input buffer
                  break;
                default:
                  cout<< "Unrocognised button index!";
                  break;
                
                  }
              }
            default:
              break;
            
        }
      }
}
      
}


int games_menu(Canvas *canvas) {
  
  if (canvas == NULL)
    return 1;
  

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  //signal(SIGTERM, InterruptHandler);
  //signal(SIGINT, InterruptHandler);

  DrawGamesCanvas(canvas);    // Using the canvas.

  // Animation finished. Shut down the RGB matrix.
  

  return 0;
}

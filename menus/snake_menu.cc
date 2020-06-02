
#include "led-matrix.h"
#include "graphics.h"
#include "mylib.h"
#include <getopt.h>
#include <string>
#include "snake.h"
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


static void DrawSnakeMenuCanvas(Canvas *canvas) {
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
  int dir = 0;
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
  Button games_button{Point{2,2}, "1 PLAYER", true};
  Button GOL_button{Point{25,2}, "2 PLAYER", false};
  
  vector<Button> buttons{games_button, GOL_button};
  
  int letter_spacing = 0;

  
  Color bg_color(0, 0, 0);
  Color bright_color(150, 0, 255);
  Color dim_color(70, 0, 150);
  Color highlight_color(255,255,255);
  ResetCanvas(canvas, 32, 64, bg_color);
  
  
  while(true){
    for(int i = 0; i < buttons.size(); i++) {
     
      //draw_text(canvas, font, buttons[i].position.row + font.baseline(), buttons[i].position.col,
                             //color, bg_color, buttons[i].text,
                             //letter_spacing);
                             
     
      if(buttons[i].is_selected){
        // border
        //DrawRect(canvas, buttons[i].position.row -1, buttons[i].position.col -1, 6, 20, highlight_color);
        rgb_matrix::DrawText(canvas, font, buttons[i].position.col, buttons[i].position.row + font.baseline(), bright_color, &bg_color, buttons[i].text, letter_spacing);
      } else {
        //DrawRect(canvas, buttons[i].position.row -1, buttons[i].position.col -1, 6, 20, bg_color);
        rgb_matrix::DrawText(canvas, font, buttons[i].position.col, buttons[i].position.row + font.baseline(), dim_color, &bg_color, buttons[i].text, letter_spacing);
      }
    }
      

        
      struct input_event ev;
      rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
      while(rc == 0) { // get all events since last iteration of game loop 
        if (rc == 0) {
          
          if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_HAT0Y") == 0 && ev.value == 1) {
            dir = 'd';
            
          } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_HAT0Y") == 0 && ev.value == -1) {
            dir = 'u';
          } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("BTN_EAST") == 0 && ev.value == 1) {
            
            return;
            
          } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("BTN_SOUTH") == 0 && ev.value == 1) {
            ResetCanvas(canvas, 32, 64, bg_color);
            if(buttons[0].is_selected) { //probably better way to do this
              
              // run snake with 1 p
              run_snake(canvas, 1);
            } else {
              // run snake with 2 p
              run_snake(canvas, 2);
            }
            ResetCanvas(canvas, 32, 64, bg_color);
          }
        }  
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);  
        
      }
      
        

      
      
      
      SetPixel(canvas, current_selected, 60, 0, 0, 0);
      switch(dir)
          {
            case 'd':
              if(current_selected < buttons.size()-1) {
                buttons[current_selected].is_selected = false;
                current_selected +=1;
                buttons[current_selected].is_selected = true;
              }
              
          
              dir = 0;
              break;
            
            case 'u':
              if(current_selected > 0) {
                buttons[current_selected].is_selected = false;
                current_selected -=1;
                buttons[current_selected].is_selected = true;
              }
              
              dir = 0;

              break;
              
            
            
            }
            SetPixel(canvas, current_selected, 60, 255, 0, 0);
          }
          
          
        
}


int snake_menu(Canvas *canvas) {
  
  if (canvas == NULL)
    return 1;
  

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  //signal(SIGTERM, InterruptHandler);
  //signal(SIGINT, InterruptHandler);

  DrawSnakeMenuCanvas(canvas);    // Using the canvas.

  // Animation finished. Shut down the RGB matrix.
  

  return 0;
}

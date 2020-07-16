// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)


#include "led-matrix.h"
#include "graphics.h"
#include "mylib.h"
#include "retro_matrix.h"
#include <getopt.h>
#include <string>

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h> 
#include <iostream>
#include <vector> 
#include <deque>

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/input.h>
#include <cstdlib>
#include "libevdev.h"
#include <fstream>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
using rgb_matrix::Font;

using rgb_matrix::Color;

using namespace std;

/*
volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
  interrupt_received = true;
}
* */

struct Ant {
  Point pos;
  int dir;
};
//TODO: spawn food only takes account ofplayer one 

int RetroMatrix::run_ant() {
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

  
  
 
  int fd;
  int rc;
  
  float brightness = 1.;
  Color light_blue(0*brightness,200*brightness,255*brightness); // line
  Color dark_blue(0*brightness,0*brightness,255*brightness); // baackwards L
  Color orange(255*brightness,150*brightness,0*brightness); // L
  Color yellow(255*brightness, 225*brightness, 0*brightness); // square
  Color green(0*brightness,255*brightness,50*brightness); // sqiggle
  Color red(255*brightness, 0*brightness, 0*brightness); //other squiggle 
  Color magenta(150*brightness, 0*brightness, 255*brightness); // cross
  
  Color bg_color(0,0,0);
  Color on_color = orange;

  Color ant_color(255,255,255);
  
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }

  int t = 5;
  int score = 1;
  int letter_spacing = 0;
  //wont compile if you take this line out 
  rgb_matrix::DrawText(canvas, font, 0, 8 + font.baseline(), bg_color, &bg_color, to_string(score).c_str(), letter_spacing);
  
  int LED_matrix[64][64] = {};
  
  for (int i = 0; i<64; i++){
    for(int j= 0; j<64; j++){
      LED_matrix[i][j] = -1;
    }
    
  }
  

  
  Point ant_pos{32, 32};
  
  Ant ant;
  ant.pos = ant_pos;
  ant.dir = 2;
  
  SetPixel(canvas, ant.pos.row, ant.pos.col, ant_color);
  
  int tn = 0;
  list <ControllerInput> inputs;
  int rotation = 0;
  ResetCanvas(canvas, n_rows, n_cols, bg_color);
  int quit;
  while (tn < 100000){
      inputs = get_inputs_from_ps4(dev);
      
      for(const auto &input: inputs){
   
        switch(input.type) {  
          case 'p':
            quit = start_menu();
            if(quit) {
              return 0;
            } else {
              ResetCanvas(canvas, n_rows, n_cols, bg_color);
              SetPixel(canvas, ant.pos.row, ant.pos.col, ant_color);
              for (int i = 0; i<64; i++){
                for(int j= 0; j<64; j++){
                  
                  if(LED_matrix[i][j] == 1){
                    
                   SetPixel(canvas, i, j, on_color);
                  }
                }
              }
                
              
            }
            break;
            case 'D': // controller disconnect
              fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
              rc = libevdev_new_from_fd(fd, &dev);
              break;
        }
      }
      
    
  
    rotation = LED_matrix[ant.pos.row][ant.pos.col]; //if -1 move clockwise, if 1 couter clockwise 
    
    
    LED_matrix[ant.pos.row][ant.pos.col] = -1 * (LED_matrix[ant.pos.row][ant.pos.col]); //flip colour of the square
    
    if(LED_matrix[ant.pos.row][ant.pos.col] == 1){
      
      SetPixel(canvas, ant.pos.row, ant.pos.col, on_color);
      
    } else if(LED_matrix[ant.pos.row][ant.pos.col] == -1){
      
      SetPixel(canvas, ant.pos.row, ant.pos.col, bg_color);
    }
    
    
    
    
    ant.dir += rotation; //0 == up, 1 == right, 2 == down, 3 == left
    
    //move forward 

    if(ant.dir == -1){
     ant.dir = 3; 
    }else if(ant.dir == 4){
      ant.dir = 0;
    }
    switch(ant.dir) {
      case 3:
        ant.pos.col = ant.pos.col -1;
        if (ant.pos.col== -1){
              ant.pos.col = 63;
            }
        break;
      case 0:
        ant.pos.row = ant.pos.row -1;
        if (ant.pos.row== -1){
              ant.pos.row = 63;
            }
        break;
      case 1:
        ant.pos.col = ant.pos.col + 1;
        if (ant.pos.col== 64){
              ant.pos.col = 0;
            }
        break;
      case 2:
        ant.pos.row = ant.pos.row +1;
          if (ant.pos.row== 64){
              ant.pos.row = 0;
            }
        break;
      
          }
          
    SetPixel(canvas, ant.pos.row, ant.pos.col, ant_color);
      
  
    usleep(t * 3000);
    tn += 1;
}
return 0;
}



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


//TODO: spawn food only takes account ofplayer one 

int rule_30(int left, int mid, int right) {
  
    
    
    int sum = left+mid+right;
    if(sum == 1) { //get three of the rules like this
      return 1;
    } else if(sum == 0 || sum == 3) {
      return 0;
    } else if(left == 1 && mid == 1 && right == 0){
      return 0;
    } else if(left == 1 && mid == 0 && right == 1){
      return 0;
    } else if(left == 0 && mid == 1 && right == 1){
      return 1;
    }
    
    
}
void R30Update(int LED_matrix[64][64], string boundary_conditions = "looping", int n_rows = 64, int n_cols = 64) {
  
  // first do middle area of array insulating effects 
  
  int new_matrix[64][64] = {0};
  // for every i exceppt 0 the new row jsut equals the previous row 
  
  for(int i = 1; i < n_rows; i++){
    for(int j = 0; j< n_cols; j++) {
      new_matrix[i][j] = LED_matrix[i-1][j];
    }
  }
  
  
  int left; int mid; int right; 
  
  //do middle cells
  for (int j=1; j < n_cols -1 ; j++){ 
    
      left = LED_matrix[0][j-1];
      mid = LED_matrix[0][j];
      right = LED_matrix[0][j+1];
      new_matrix[0][j] = rule_30(left, mid, right);
      
  }
    
    // do the edges 
    
    left = LED_matrix[0][n_cols-1];
    mid = LED_matrix[0][0];
    right = LED_matrix[0][1];
    
    new_matrix[0][0] = rule_30(left, mid, right);
    //new_matrix[0][0] = 0;
    left = LED_matrix[0][n_cols-2];
    mid = LED_matrix[0][n_cols-1];
    right = LED_matrix[0][0];
    
    new_matrix[0][n_cols-1] = rule_30(left, mid, right);
    //new_matrix[0][n_cols-1] = 1;
    
  
  

  
  
 
  for (int i = 0; i < n_rows; i++) { // for each row
    
    for (int j = 0; j < n_cols; j++) {
        
        LED_matrix[i][j] = new_matrix[i][j];
        
        
    }
  }
  
}
 
 
  
void RandomICs1D(int LED_matrix[64][64], float density, int n_rows = 64, int n_cols = 64) {
  
  // initial conditions
   // for first row
    
    for (int j = 0; j < n_cols; j++) {
       
       if ((float)(rand() % 1000)/1000 < density){
         
         LED_matrix[0][j] =1;
       }
    
      
  }
  
  
}


int RetroMatrix::run_R30(){
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
  
  libevdev_disable_event_code	(dev, EV_ABS, ABS_X);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_Y);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_RX);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_RY);
  
  
  float brightness = 0.6;
  Color light_blue(0*brightness,200*brightness,255*brightness); // line
  Color dark_blue(0*brightness,0*brightness,255*brightness); // baackwards L
  Color orange(255*brightness,150*brightness,0*brightness); // L
  Color yellow(255*brightness, 225*brightness, 0*brightness); // square
  Color green(0*brightness,255*brightness,50*brightness); // sqiggle
  Color red(255*brightness, 0*brightness, 0*brightness); //other squiggle 
  Color magenta(150*brightness, 0*brightness, 255*brightness); // cross
  
  Color bg_color(0,0,0);
  
  
  
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }

  int t = 5;
  int score = 1;
  int letter_spacing = 0;
  //wont compile if you take this line out 
  rgb_matrix::DrawText(canvas, font, 0, 8 + font.baseline(), bg_color, &bg_color, to_string(score).c_str(), letter_spacing);
  
  int LED_matrix[64][64] = {0};
  LED_matrix[0][33] = 1;
  
  int tn = 0;
  list <ControllerInput> inputs;
  while (tn < 500){
      inputs = get_inputs_from_ps4(dev);
      
      for(const auto &input: inputs){
   
        switch(input.type) {  // go from first input as unlikely to have multiple inputs perframes with no sleep
          case 'p':
            int quit = start_menu();
            if(quit) {
              return 0;
            }
        }
      }
      
    
  
    SetPixels(canvas, LED_matrix, green, 64, 64);
     
    R30Update(LED_matrix);
    usleep(t * 10000);
    tn += 1;
}
return 0;
}




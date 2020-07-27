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




void loop_update(int LED_matrix[64][64], vector<string> rules, string boundary_conditions = "looping", int n_rows = 64, int n_cols = 64) {
  
  // first do middle area of array insulating effects 
  
  int new_matrix[64][64] = {0};
  
  // current cell and all the neighbours 
  int c;
  int t;
  int b;
  int l;
  int r;
  
  
  
  for (int i=0; i < n_rows; i++){

    for (int j=0; j < n_cols; j++){
      
      // get current state and all perumtations of the neighbours to check against the rules
      c = LED_matrix[i][j];
      
      if(i == 0){
        t = LED_matrix[63][j];
      } else {
        t = LED_matrix[i-1][j];
      }
      
      if(i == 63){
        b = LED_matrix[0][j];
      } else {
        b = LED_matrix[i+1][j];
      }
      
      if(j == 0){
        l = LED_matrix[i][63];
      } else {
        l = LED_matrix[i][j-1];
      }
      
      if(j == 63){
        r = LED_matrix[i][0];
      } else {
        r = LED_matrix[i][j+1];
      }
      
    
      vector<string> rots;
      rots.push_back(to_string(c) + to_string(t) + to_string(r) + to_string(b) + to_string(l));
      rots.push_back(to_string(c) + to_string(l) + to_string(t) + to_string(r) + to_string(b));
      rots.push_back(to_string(c) + to_string(b) + to_string(l) + to_string(t) + to_string(r));
      rots.push_back(to_string(c) + to_string(r) + to_string(b) + to_string(l) + to_string(t));
      
      
      for (int k = 0; k < rules.size(); k++) {
        
        for(int l = 0; l < rots.size(); l++) {
          
          if(rots[l].compare(rules[k].substr(0, 5))==0) {
           
            new_matrix[i][j] = stoi(rules[k].substr(5,1));
            goto end;
          }
          
        }
        
      }
      end: continue;
     
   }
 
  }

  for (int i = 0; i < n_rows; i++) { // for each row
    
    for (int j = 0; j < n_cols; j++) {
        //cout << new_matrix[i][j];
        LED_matrix[i][j] = new_matrix[i][j];
        
        
    }
  }
 
  
  
}

void set_pixels(Canvas *canvas, int LED_matrix[64][64], vector<Color> colours, int n_rows = 64,int n_cols = 64){
  
    for (int i = 0; i < n_rows; i++) { // for each row
      for (int j = 0; j < n_cols; j++) { // for each column
          SetPixel(canvas, i, j, colours[LED_matrix[i][j]]);
    }
  }
  
}

int RetroMatrix::run_loop(){
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

  // parse rules file
  string rule;
  vector<string> rules;
  ifstream myfile ("/home/pi/Desktop/retro_matrix/cellular_automata/langtons_loop/rules.txt");
  if (myfile.is_open())
  {
    while(getline(myfile, rule)){
      rules.push_back(rule);
    }
    myfile.close();
  }
  
  int fd;
  int rc;
  
  
  float brightness = 1;
  Color light_blue(0*brightness,200*brightness,255*brightness); // line
  Color dark_blue(0*brightness,0*brightness,255*brightness); // baackwards L
  Color orange(255*brightness,150*brightness,0*brightness); // L
  Color yellow(255*brightness, 225*brightness, 0*brightness); // square
  Color green(0*brightness,255*brightness,0*brightness); // sqiggle
  Color red(255*brightness, 0*brightness, 0*brightness); //other squiggle 
  Color magenta(150*brightness, 0*brightness, 255*brightness); // cross
  Color white(255*brightness, 255*brightness, 255*brightness);
  
  Color bg_color(0,0,0);
  vector<Color> colours = {bg_color, dark_blue, green, orange, yellow, green,  white, red};
  
  
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
  
  //make the ninitial loop
  int loop[10][14] = {{0,2,2,2,2,2,2,2,2,0,0,0,0,0},
            {2,1,7,0,1,4,0,1,4,2,0,0,0,0},
            {2,0,2,2,2,2,2,2,0,2,0,0,0,0},
            {2,7,2,0,0,0,0,2,1,2,0,0,0,0},
            {2,1,2,0,0,0,0,2,1,2,0,0,0,0},
            {2,0,2,0,0,0,0,2,1,2,0,0,0,0},
            {2,7,2,0,0,0,0,2,1,2,0,0,0,0},
            {2,1,2,2,2,2,2,2,1,2,2,2,2,2},
            {2,0,7,1,0,7,1,0,7,1,1,1,1,1},
            {0,2,2,2,2,2,2,2,2,2,2,2,2,2}};
            
  Point pos{25,25};
  for (int i = 0; i < 10; i++){
    for(int j = 0; j < 14; j ++){
      LED_matrix[i+pos.row][j+pos.col] = loop[i][j];
    }
    
  }
  

  
  int quit;
  int tn = 0;
  list <ControllerInput> inputs;
  while (tn < 1200){
      
      inputs = get_inputs_from_ps4(dev);
      
      for(const auto &input: inputs){
   
        switch(input.type) {  // go from first input as unlikely to have multiple inputs perframes with no sleep
          case 'p':
            quit = start_menu();
            if(quit) {
              return 0;
            }
        
        break;
        case 'D': // controller disconnect
              fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
              rc = libevdev_new_from_fd(fd, &dev);
              break;
        }
      }
      
    
  
    set_pixels(canvas, LED_matrix, colours, 64, 64);
     
    loop_update(LED_matrix, rules);
    usleep(t * 10000);
    tn += 1;
}
return 0;
}




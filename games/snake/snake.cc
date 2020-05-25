// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include <string>

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h> 
#include <iostream>
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

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

using namespace std;

/*
volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
  interrupt_received = true;
}
* */

static void SetPixel(Canvas *canvas, int row, int col, uint8_t red, uint8_t green, uint8_t blue) { // swpas x and y so indexing is matrix 
  
  canvas->SetPixel(col, row, red, green, blue);
}

struct Point {
  int row, col;
};



int random(int min, int max) //range : [min, max)
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }

   return min + rand() % (( max + 1 ) - min);
}

Point spawn_food(deque<Point> snake, int rows, int cols){
  // spawns food that isnt inside the snake 
  
  
  
  
  while(true){
    bool food_in_snake = false;
    int row = random(0, rows -1);
    int col = random(0, cols -1);
    
    for(int i = 0; i < snake.size(); i++){
      if(row == snake[i].row && col == snake[i].col) {
        food_in_snake = true;
        row = random(0, rows -1);
        col = random(0, cols -1);
        break;
      }
      
      
    }
 
    
    if(!food_in_snake){
      return Point{row, col};
    }
  }
  
}

//TODO: bug where food disapears if it spawns inside snake 

static void DrawOnCanvas(Canvas *canvas) {
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
  /*
  libevdev_disable_event_code	(dev, EV_ABS, ABS_X);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_Y);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_RX);
  libevdev_disable_event_code	(dev, EV_ABS, ABS_RY);
  * */
         
  

  deque<Point> snake{Point{3,6}, Point{3,5}, Point{3,4}, Point{3,3}, Point{3,2}, Point{3,1}, Point{3,0}};
  // snake is dequeue where the front of the queue is the head and the back fo the queue if the tail
  
  Point food = spawn_food(snake, 32, 64);
  SetPixel(canvas, food.row, food.col, 0, 255, 0);
  cout << food.row << " " << food.col<< endl;
 
  
  int score = 0;
  int t = 100000;

  int dir = 'r';
  
  Point new_head;
  Point back;
  
  bool in_snake = false;
  
  for(int s = 0; s<snake.size(); s++){
          
      SetPixel(canvas, snake[s].row, snake[s].col, 0, 0, 255);
            
            
  }
  
  
  
  int pos = 0;
  while (true){
     
    
    
    struct input_event ev;
    rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
    while(rc == 0) { // get all events since last iteration of game loop 
      
      
      
                             
                        
      if (rc == 0) {
        if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_HAT0Y") && ev.value == -1 && dir != 'r') {
          dir = 'l';
        } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_HAT0Y") && ev.value == 1&& dir != 'l') {
          dir = 'r';
        } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_HAT0X") && ev.value == 1&& dir != 'u') {
          dir = 'd';
        } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_HAT0X") && ev.value == -1&& dir != 'd') {
          dir = 'u';
        }
      }  
      rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);  
    }
      

    
    
    
    switch(dir)
        {
          case 'l':
            new_head.row = snake.front().row;
            new_head.col = snake.front().col -1;
            if (new_head.col == -1){
              new_head.col = 63;
            }
            
            
            break;
          case 'u':
            new_head.row = snake.front().row -1;
            new_head.col = snake.front().col;
            if (new_head.row == -1){
              new_head.row = 31;
            }
            
            
            
            break;
          case 'r':
            new_head.row = snake.front().row;
            new_head.col = snake.front().col + 1;
            if (new_head.col == 64){
              new_head.col = -1;
            }
            
            
            break;
          case 'd':
            new_head.row = snake.front().row +1;
            new_head.col = snake.front().col;
            if (new_head.row == 32){
              new_head.row = 0;
            }
            
            break;
            
          
          
          }
          snake.push_front(new_head);
          SetPixel(canvas, new_head.row, new_head.col, 0, 0, 255);
    
    
    
    if(new_head.row == food.row && new_head.col == food.col) { // if eats food
      score +=1;
      if(score%10 == 0){
        t*= 0.9;
      }
      SetPixel(canvas, food.row, food.col, 0, 0, 255);
      food = spawn_food(snake, 32, 64);
      SetPixel(canvas, food.row, food.col, 0, 255, 0);
      
    } else {
      back = snake.back();
      snake.pop_back();
      SetPixel(canvas, back.row, back.col, 0, 0, 0);
    }
    
    
    
    
    //check if snake has eaten itself
    

    for(int s = 1; s<snake.size(); s++){
      if(new_head.row==snake[s].row && new_head.col == snake[s].col) {
        usleep(50 * t);
        exit(0);
      }
    }
    
    
    usleep(1 * t);
    
}
}



  
  
  
  


int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "adafruit-hat";  // or e.g. "adafruit-hat"
  defaults.rows = 32;
  defaults.cols = 64;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = false;
  
  // use --led-slowdown-gpio=4

  Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  //signal(SIGTERM, InterruptHandler);
  //signal(SIGINT, InterruptHandler);

  DrawOnCanvas(canvas);    // Using the canvas.

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}

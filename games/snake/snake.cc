// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)


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


Point spawn_food(deque<Point> snake, int min_row, int max_row, int min_col, int max_col){
  // spawns food that isnt inside the snake 
  
  
  
  int row;
  int col;
  while(true){
    bool food_in_snake = false;
    row = random(min_row, max_row);
    col = random(min_col, max_col);
    
    for(int i = 0; i < snake.size(); i++){
      if(row == snake[i].row && col == snake[i].col) {
        food_in_snake = true;
        break;
      }
      
      
    }
 
    
    if(!food_in_snake){
      return Point{row, col};
    }
  }
  
}

//TODO: spawn food only takes account ofplayer one 



int get_direction_from_keyboard(struct libevdev *dev, int current_dir){
      struct input_event ev;
      int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
      int dir = current_dir;
     
      while(rc == 0) { // get all events since last iteration of game loop 
              
        if (rc == 0) {
          if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("KEY_LEFT") == 0 && ev.value == 1 && dir != 'r') {
            dir = 'l';
            cout << "LEFT" << endl;
          } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("KEY_RIGHT") ==0 && ev.value == 1 && dir != 'l') {
            dir = 'r';
            cout << "RIGHT" << endl;
          } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("KEY_DOWN") == 0&& ev.value == 1 && dir != 'u') {
            dir = 'd';
            cout << "DOWN" << endl;
          } else if (string(libevdev_event_code_get_name(ev.type, ev.code)).compare("KEY_UP") == 0&& ev.value == 1&& dir != 'd') {
            dir = 'u';
            cout << "UP" << endl;
          }
        }  
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);  
      }
      return dir;
}


void move_head(deque<Point> &snake,  int dir, bool boundaries,  int min_row, int max_row, int min_col, int max_col){
  Point new_head;
  Point back;
  switch(dir) // THIS SHOULD BE A FUNCTIOn
        {
          case 'l':
            new_head.row = snake.front().row;
            new_head.col = snake.front().col -1;
            if (!boundaries &&new_head.col == min_col-1){
              new_head.col = max_col;
            }
            
            
            break;
          case 'u':
            new_head.row = snake.front().row -1;
            new_head.col = snake.front().col;
            if (!boundaries &&new_head.row == min_row-1){
              new_head.row = max_row;
            }
            
            
            
            break;
          case 'r':
            new_head.row = snake.front().row;
            new_head.col = snake.front().col + 1;
            if (!boundaries &&new_head.col == max_col + 1){
              new_head.col = min_col;
            }
            
            
            break;
          case 'd':
            new_head.row = snake.front().row +1;
            new_head.col = snake.front().col;
            if (!boundaries &&new_head.row == max_row + 1){
              new_head.row = min_row;
            }
            
            break;
          
          }
          snake.push_front(new_head);
         
}

bool check_eats_food(deque<Point> snake, Point food) {
  
  Point new_head = snake.front();
  
  if(new_head.row == food.row && new_head.col == food.col) { // if eats food
      
      
      return true;
      
    } else {
      
      return false;
    }
  
}

bool check_collision(deque<Point> snake, bool boundaries, int min_row, int max_row, int min_col, int max_col) {
    int row = snake.front().row;
    int col = snake.front().col;
    
    if ((boundaries) &&(col == min_col-1 || col == max_col+1 ||row == min_row-1 || row == max_row+1  )){
        return true;
    }
  
  
  //check if snake has eaten itself
    
    Point new_head = snake.front();
    for(int s = 1; s<snake.size(); s++){
      if(new_head.row==snake[s].row && new_head.col == snake[s].col) {
        return true;
      }
    }
    return false;
}


// if snake has collided with snake1
bool check_collision(deque<Point> snake, deque<Point> snake1, bool boundaries, int min_row, int max_row, int min_col, int max_col) {
  
  
    int row = snake.front().row;
    int col = snake.front().col;
    
    if ((boundaries) &&(col == min_col-1 || col == max_col+1 ||row == min_row-1 || row == max_row+1  )){
        return true;
    }
  
  
  //check if snake has eaten itself
    
    Point new_head = snake.front();
    for(int s = 1; s<snake.size(); s++){
      if(new_head.row==snake1[s].row && new_head.col == snake1[s].col) {
        return true;
      }
    }
    return false;
}


bool turning_back(int dir, int new_dir){
  
  if(dir == 'l' and new_dir == 'r'){
    return true;
  }else if(dir == 'r' and new_dir == 'l'){
    return true;
  }  else if(dir == 'u' and new_dir == 'd'){
    return true;
  } else if(dir == 'd' and new_dir == 'u'){
    return true;
  } else {
    return false;
  }
}

    
void RetroMatrix::one_p_snake() {
  

  int fd;
  int rc = 1;
   
   
  // play area taking into account score display and border
  
  int min_row = 4;
  int max_row = 53;
  int min_col = 7;
  int max_col = 56;
  

  //deque<Point> snake{Point{3,18}, Point{3,17}, Point{3,16}, Point{3,15}, Point{3,14}, Point{3,13}, Point{3,12}};
  deque<Point> snake{ Point{min_row+3,min_col+1}};
  // snake is dequeue where the front of the queue is the head and the back fo the queue if the tail
  
  
 
  // load high score 
  string hs;
  ifstream myfile ("/home/pi/Desktop/retro_matrix/games/snake/high_score.txt");
  if (myfile.is_open())
  {
    getline (myfile,hs);
    
    myfile.close();
  }
  
  
  int high_score = stoi(hs);
  int score = 0;

  int t = 100000;

  //bool in_snake = false;
  
  for(int s = 0; s<snake.size(); s++){
          
      SetPixel(canvas, snake[s].row, snake[s].col, 0, 0, 255);
            
            
  }
  
  
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }
  
  Color score_color(0,150,0);
  Color high_score_color(150, 150, 0);
  Color bg_color(0,0,0);
  Color white(200, 200, 200);
  Color border_color(150, 30, 0);
  border_color = Color(255, 0, 0);
  Color food_color(0,200, 0);
  Color snake_color(0,0,255);
  
  Point food = spawn_food(snake, min_row, max_row, min_col, max_col);
  SetPixel(canvas, food.row, food.col, food_color);
  
  
  int letter_spacing = 0;
  
  bool boundaries = true;
  
  //draw_line(canvas, 0, 11, 31, 11, white);
  DrawRect(canvas, min_row-1, min_col-1, max_row-min_row+2, max_col-min_col+2, border_color);
  
  list <ControllerInput> inputs;
  int dir = 'r';
  int new_dir = -1;
  
  // draw current and hiugh score 
  draw_text(canvas, font, 63, 31 - 5*font.baseline(),  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
  draw_text(canvas, font, 63 , 31 + 3*font.baseline(), high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
  int quit;

  struct input_event ev;
  while(true){
    
    
    inputs = get_inputs_from_ps4(dev);
    //
   
    for(const auto &input: inputs){
   
        switch(input.type) {  
          case 'p':
            quit = start_menu();
            if(quit) {
              return;
            } else{
              ResetCanvas(canvas, n_rows, n_cols, bg_color);
              DrawRect(canvas, min_row-1, min_col-1, max_row-min_row+2, max_col-min_col+2, border_color);
              // draw current and hiugh score 
              draw_text(canvas, font, 63, 31-5*font.baseline(),  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
  draw_text(canvas, font, 63 , 31 + 3*font.baseline(), high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
               SetPixel(canvas, food.row, food.col,food_color);
               
               
              for(int s = 0; s<snake.size(); s++){
          
                  SetPixel(canvas, snake[s].row, snake[s].col, snake_color);
                   
                        
              }
            }
            break;
            case 'D': // controller disconnect
              quit = start_menu();
              if(quit) {
                return ;
              } else{
                  ResetCanvas(canvas, n_rows, n_cols, bg_color);
                  DrawRect(canvas, min_row-1, min_col-1, max_row-min_row+2, max_col-min_col+2, border_color);
                  // draw current and hiugh score 
                  draw_text(canvas, font, 63, 31-5*font.baseline(),  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
  draw_text(canvas, font, 63 , 31 + 3*font.baseline(), high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
                   SetPixel(canvas, food.row, food.col,food_color);
                   
                   
                  for(int s = 0; s<snake.size(); s++){
              
                      SetPixel(canvas, snake[s].row, snake[s].col, snake_color);
                       
                            
                  }
              }
              
              break;
        }
      }
      
      
    new_dir = get_direction_from_inputs(inputs);
    

    if(new_dir != -1 && !turning_back(dir, new_dir)){
      dir = new_dir;
    }

    bool has_eaten = check_eats_food(snake, food);
    
    
    move_head(snake,  dir, boundaries, min_row, max_row, min_col, max_col);
    
    //update_matrix(canvas, snake, food, has_eaten, min_row,  max_row,  min_col,  max_col);
    Point new_head = snake.front();
    SetPixel(canvas, new_head.row, new_head.col, snake_color);
    
    if(has_eaten) {
      
      SetPixel(canvas, food.row, food.col, snake_color);
      food = spawn_food(snake, min_row, max_row, min_col, max_col);
      SetPixel(canvas, food.row, food.col,food_color);
      score+=1;
      // draw current and hiugh score 
      draw_text(canvas, font, 63, 31-5*font.baseline(),  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
  draw_text(canvas, font, 63 , 31 + 3*font.baseline(), high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
      if(score > high_score) {
        high_score = score;
      }
      if(score%10 == 0){
        t*= 0.9;
      }
      
      
    } else {
      Point back = snake.back();
      snake.pop_back();
      SetPixel(canvas, back.row, back.col, 0, 0, 0);
    }
    
   
    
    
    if(check_collision(snake, boundaries, min_row, max_row, min_col, max_col)) {
      ofstream myfile ("/home/pi/Desktop/retro_matrix/games/snake/high_score.txt");
        if (myfile.is_open())
        {
          myfile << high_score;
          myfile.close();
        }
        usleep(50 * t);
        
        return;
    }
    usleep(1 * t);
    
}
}


void RetroMatrix::two_p_snake() {
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
  int rc = 1;
  
  printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
  printf("Input device ID: bus %#x vendor %#x product %#x\n",
         libevdev_get_id_bustype(dev),
         libevdev_get_id_vendor(dev),
         libevdev_get_id_product(dev));
  
  
 
  printf("Input device name: \"%s\"\n", libevdev_get_name(dev1));
  printf("Input device ID: bus %#x vendor %#x product %#x\n",
         libevdev_get_id_bustype(dev1),
         libevdev_get_id_vendor(dev1),
         libevdev_get_id_product(dev1));
         
         
  // filter out the constant events that flood the queue
  
  
  
         
  int min_row = 0;
  int max_row = 63;
  int min_col = 0;
  int max_col = 63;
  Color win_text_color(255,255,255);
  Color bg_color(0, 0, 0);
  Color snake_color(0,0,255);
  Color snake1_color(255, 0, 0);
  Color food_color(0,200,0);
  

  deque<Point> snake{Point{3,6}, Point{3,5}, Point{3,4}, Point{3,3}, Point{3,2}, Point{3,1}, Point{3,0}};
  // snake is dequeue where the front of the queue is the head and the back fo the queue if the tail
  deque<Point> snake1{Point{28,57}, Point{28,58}, Point{28,59}, Point{28,60}, Point{28,61}, Point{28,62}, Point{28,63}};
  
  Point food = spawn_food(snake, min_row, max_row, min_col, max_col);
  SetPixel(canvas, food.row, food.col, food_color);

 
  
  int score = 0;
  int score1 = 0;
  int t = 100000;

  int dir = 'r';
  int new_dir = -1;
  int dir1 = 'l';
  
  Point new_head;
  Point back;
  Point new_head1;
  Point back1;
  
  //bool in_snake = false;
  
  for(int s = 0; s<snake.size(); s++){
          
      SetPixel(canvas, snake[s].row, snake[s].col, snake_color);
      SetPixel(canvas, snake1[s].row, snake1[s].col, snake1_color);
            
  }
  
  
  int letter_spacing = 0;
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }
  
  bool boundaries = false;
  list<ControllerInput> inputs;
  list<ControllerInput> inputs1;
  
  
  while(true){
    inputs = get_inputs_from_ps4(dev);
    

    //
   
    for(const auto &input: inputs){
   
        switch(input.type) {  
          case 'p':
            int quit = start_menu();
            if(quit) {
              return;
            } else{
              ResetCanvas(canvas, 32, 64, bg_color);
              Point food = spawn_food(snake, min_row, max_row, min_col, max_col);
              SetPixel(canvas, food.row, food.col, food_color);
              for(int s = 0; s<snake.size(); s++){
          
                  SetPixel(canvas, snake[s].row, snake[s].col, snake_color);
                  SetPixel(canvas, snake1[s].row, snake1[s].col, snake1_color);
                        
              }
                          
            }
        }
      }
      
      
    new_dir = get_direction_from_inputs(inputs);
 
    if(new_dir != -1 && !turning_back(dir, new_dir)){
      dir = new_dir;
    }
    
    
    
    inputs1 = get_inputs_from_ps4(dev1);
    

    //
   
    for(const auto &input: inputs1){
   
        switch(input.type) {  
          case 'p':
            int quit = start_menu();
            if(quit) {
              return;
            } else{
              ResetCanvas(canvas, 32, 64, bg_color);
              Point food = spawn_food(snake, min_row, max_row, min_col, max_col);
              SetPixel(canvas, food.row, food.col, food_color);
              for(int s = 0; s<snake.size(); s++){
          
                  SetPixel(canvas, snake[s].row, snake[s].col, snake_color);
                  SetPixel(canvas, snake1[s].row, snake1[s].col, snake1_color);
                        
              }
                          
            }
        }
      }
      
      
    new_dir = get_direction_from_inputs(inputs1);
 
    if(new_dir != -1 && !turning_back(dir1, new_dir)){
      dir1 = new_dir;
    }

    move_head(snake,  dir, boundaries, min_row, max_row, min_col, max_col);
    
          
    move_head(snake1,  dir1, boundaries, min_row, max_row, min_col, max_col);
    
    Point new_head = snake.front();
    Point new_head1 = snake1.front();
    SetPixel(canvas, new_head.row, new_head.col,snake_color);
    SetPixel(canvas, new_head1.row, new_head1.col, snake1_color);
    
    //player 1
    if(new_head.row == food.row && new_head.col == food.col) { // if eats food
      score +=1;
      if(score%10 == 0){
        t*= 0.9;
      }
      SetPixel(canvas, food.row, food.col, snake_color);
      food = spawn_food(snake, min_row, max_row, min_col, max_col);
      SetPixel(canvas, food.row, food.col, food_color);
      
    } else {
      back = snake.back();
      snake.pop_back();
      SetPixel(canvas, back.row, back.col, 0, 0, 0);
    }
    
    //player 2
    if(new_head1.row == food.row && new_head1.col == food.col) { // if eats food
      score1 +=1;
      if(score1%10 == 0){
        t*= 0.9;
      }
      SetPixel(canvas, food.row, food.col, snake1_color);
      food = spawn_food(snake, min_row, max_row, min_col, max_col);
      SetPixel(canvas, food.row, food.col, food_color);
      
    } else {
      back1 = snake1.back();
      snake1.pop_back();
      SetPixel(canvas, back1.row, back1.col, 0, 0, 0);
    }
    
    

    //check if snake has eaten itself or been trapped by other snake
    
    //player 1 collides with p1
    
      if(check_collision(snake, boundaries, min_row, max_row, min_col, max_col)) {
        
        rgb_matrix::DrawText(canvas, font, 15, 20,
                           win_text_color, &bg_color, "RED WINS!",
                           letter_spacing);
        usleep(50 * t);
        
        return;
      }
      
      //player 1 collides with p2
      if(check_collision(snake, snake1, boundaries, min_row, max_row, min_col, max_col)) {
      
        rgb_matrix::DrawText(canvas, font, 15, 20,
                           win_text_color, &bg_color, "RED WINS!",
                           letter_spacing);
        usleep(50 * t);
        
        return;
      
      }
      
      

    //player 2 collides with p1
    
      if(check_collision(snake1, boundaries, min_row, max_row, min_col, max_col)) {
        
        rgb_matrix::DrawText(canvas, font, 15, 20,
                           win_text_color, &bg_color, "BLUE WINS!",
                           letter_spacing);
        usleep(50 * t);
        
        
        return;
        
      
    }
    //player 2 collides with p2
      if(check_collision(snake1, snake, boundaries, min_row, max_row, min_col, max_col)) {
        cout<<"four"<<endl;
        
        rgb_matrix::DrawText(canvas, font, 15, 20,
                           win_text_color, &bg_color, "BLUE WINS!",
                           letter_spacing);
        usleep(50 * t);
        
        
        return;
      
      }
    
    
    usleep(1 * t);
    
}
}

int RetroMatrix::run_snake(int n_players) {

  if (canvas == NULL)
    return 1;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  //signal(SIGTERM, InterruptHandler);
  //signal(SIGINT, InterruptHandler);
  
  Color bg_color(0, 0, 0);
  ResetCanvas(canvas, n_rows, n_rows, bg_color);
  if(n_players == 1){
    one_p_snake();
  } else {
    two_p_snake();
  }

  
  return 0;
}

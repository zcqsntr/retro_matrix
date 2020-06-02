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

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

using namespace std;

/*
volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
  interrupt_received = true;
}
*/

static void SetPixel(Canvas *canvas, int row, int col, uint8_t red, uint8_t green, uint8_t blue) { // swpas x and y so indexing is matrix 
  
  canvas->SetPixel(col, row, red, green, blue);
}

static void RandomICs(int LED_matrix[32][64], float density, int n_rows = 32, int n_cols = 64) {
  
  // initial conditions
  for (int i = 0; i < n_rows; i++) { // for each row
    
      for (int j = 0; j < n_cols; j++) {
         
         if ((float)(rand() % 1000)/1000 < density){
           
           LED_matrix[i][j] =1;
         }
      
        
    }
  }
  
}

static void GOLUpdate(int LED_matrix[32][64], string boundary_conditions = "looping", int n_rows = 32, int n_cols = 64) {
  
  // first do middle area of array insulating effects 
  
  int new_matrix[32][64] = {0};
  
  
  
  for (int i = 0; i < n_rows ; i++){
    for (int j=0; j < n_cols ; j++){
      
    
      int current_state = LED_matrix[i][j];
      
      
      int n_living_neighbours = 0;
      
      for (int row = i-1; row <= i+ 1;  row++){
        for(int col = j-1; col <= j+1; col++){
          int r = -1;
          int c = -1;
          
          if(boundary_conditions.compare("looping") ==0 ) {
              if(row == - 1) { //top
                r = 31;
                c = col;
              } else if(col == - 1) { //left
                c = 63;
                r = row;
              } else if(row == 32) { //bottom
                r = 0;
                c = col;
              } else if(col == 64) { //right
                c = 0;
                r = row;
              } else if(row == -1 and col == -1){ //top left
                r = 31;
                c = 63; 
             } else if(row == 32 && col == -1) { //bottom left
               r = 0;
               c = 63;
            
             } else if(row == -1 && col == 64) { //top right
               r = 31;
               c = 0;
            
             } else if(row == 32 && col == 64) { //bottom right
               r = 0;
               c = 0;
            
             } else {
               r = row;
               c = col;
             }
        
          } else if (boundary_conditions.compare("insulating") ==0 ) {
            
            
            if(row == -1 || col == -1 || row == 32 || col == 64){
              continue;
            } else {
              r = row;
              c = col;
            }
          }
          
          n_living_neighbours += LED_matrix[r][c];
        }
      }
      
      n_living_neighbours -= current_state; //remove this if it was counted
      //printf("%d", n_living_neighbours);
      if(current_state == 1 &&  (n_living_neighbours == 2 || n_living_neighbours == 3)){
        
        new_matrix[i][j] = 1;
      } else if(current_state == 0 && n_living_neighbours == 3){
        new_matrix[i][j] = 1;
      } else if (current_state == 1){
        new_matrix[i][j] = 0;
      }
      
      
    }
  }

  for (int i = 0; i < n_rows; i++) { // for each row
    
    for (int j = 0; j < n_cols; j++) {
        
        LED_matrix[i][j] = new_matrix[i][j];
        
        
    }
  }
 
  
  
}

static void SetPixels(Canvas *canvas, int LED_matrix[32][64], int n_rows = 32, int n_cols = 64) {
  
    for (int i = 0; i < n_rows; i++) { // for each row
      for (int j = 0; j < n_cols; j++) { // for each column
        
        if (LED_matrix[i][j] == 1){
          
          SetPixel(canvas, i, j, 255, 255, 0);
          
        } else {
          SetPixel(canvas, i, j, 0, 0, 0);
        }
        
    }
  }
  
}

static void MakeGliderGun(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0, int direction = 0) {
  
  // loc is top left point of glider 
  
  
  int n_points = 36;
  int rows[n_points] = {4, 4, 5, 5, 2,  2,  3,  4,  5,  6,  7,  8,  8,  5,  3,  7,  6,  5,  4,  5,  4,  3,  2,  4,  3,  2,  1,  5,  5,  6,  1,  0,  2,  3,  2,  3};
  int cols[n_points] = {0, 1, 0, 1, 12, 13, 11, 10, 10, 10, 11, 12, 13, 14, 15, 15, 16, 16, 16, 17, 20, 20, 20, 21, 21, 21, 22, 22, 24, 24, 24, 24, 34, 34, 35, 35};
  int new_rows[n_points] = {0};
  int new_cols[n_points] = {0};
  cout << direction;
    
  switch(direction) {
    case 0:
      cout << "case0";
      for (int i =0; i<n_points; i++){
        new_rows[i] = rows[i];
        new_cols[i] = cols[i];
      }
      break;
    case 1:
      cout << " case1";
      for (int i =0; i<n_points; i++){
        new_rows[i] = cols[i];
        new_cols[i] = -rows[i] + 35;
      }
      break;
    case 2:
      
      for (int i =0; i<n_points; i++){
        new_rows[i] = -rows[i] + 8;
        new_cols[i] = -cols[i] + 35;
      }
      break;
    case 3:
      
      for (int i =0; i<n_points; i++){
        new_rows[i] = -cols[i] + 8;
        new_cols[i] = rows[i];
      }
      break;
    
  }
  cout << "\n";
  for (int i = 0; i <n_points; i++){
    cout<< rows[i] << " " << new_rows[i] << " | ";
    cout<< cols[i] << " " << new_cols[i] << "\n";
    LED_matrix[row_loc+ new_rows[i]][col_loc + new_cols[i]] = 1;
  }
  
  
  
  
  
}

static void MakeGlider(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0, int direction = 0) {
  
  // loc is top left point of glider 
  
  int rows[5] = {0, 1, 1, 2, 2};
  int cols[5] = {0, 1, 2, 0, 1};
  int new_rows[5] = {0};
  int new_cols[5] = {0};
  cout << direction;
    
  switch(direction) {
    case 0:
      cout << "case0";
      for (int i =0; i<5; i++){
        new_rows[i] = rows[i];
        new_cols[i] = cols[i];
      }
      break;
    case 1:
      cout << " case1";
      for (int i =0; i<5; i++){
        new_rows[i] = cols[i];
        new_cols[i] = -rows[i] + 2;
      }
      break;
    case 2:
      
      for (int i =0; i<5; i++){
        new_rows[i] = -rows[i] + 2;
        new_cols[i] = -cols[i] + 2;
      }
      break;
    case 3:
      
      for (int i =0; i<5; i++){
        new_rows[i] = -cols[i] + 2;
        new_cols[i] = rows[i];
      }
      break;
    
  }
  cout << "\n";
  for (int i = 0; i < 5; i++){
    cout<< rows[i] << " " << new_rows[i] << " | ";
    cout<< cols[i] << " " << new_cols[i] << "\n";
    LED_matrix[row_loc+ new_rows[i]][col_loc + new_cols[i]] = 1;
  }
  
}

static void MakeBlock(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0) {
  
  // loc is top left point of glider 
  
  
  
  int rows[5] = {0, 0, 1, 1};
  int cols[5] = {0, 1, 0, 1};
  
  for (int i = 0; i < 5; i++){
    
    LED_matrix[row_loc+ rows[i]][col_loc + cols[i]] = 1;
  }
  
  
  
  
  
}

static void MakeBlinker(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0) {
  
  // loc is top left point of glider 
  
  
  
  int rows[5] = {0, 0, 0};
  int cols[5] = {0, 1, 2};
  
  for (int i = 0; i < 5; i++){
    
    LED_matrix[row_loc+ rows[i]][col_loc + cols[i]] = 1;
  }
  
  
  
  
  
}

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
  
  
  
  int LED_matrix[32][64] = {0};
  
  RandomICs(LED_matrix, 0.2);
  
  // four gliders in the corners of a square
  

  //MakeGlider(LED_matrix, 1, 1, 0);
  //MakeGlider(LED_matrix, 1, 28, 1);
  //MakeGlider(LED_matrix, 28, 28, 2);
  //MakeGlider(LED_matrix, 28, 1, 3);
  
  //MakeBlinker(LED_matrix, 10, 10);
  //MakeBlock(LED_matrix, 0, 0);
  //cout << "hello" << endl;
  //MakeGliderGun(LED_matrix, 0, 0,0); 
  
  //MakeBlinker(LED_matrix, 5, 5);
  
 
  int t = 0;
  
  while (t < 1000){
    
  
    SetPixels(canvas, LED_matrix);
     
    GOLUpdate(LED_matrix);
    usleep(1 * 50000);
    t += 1;
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

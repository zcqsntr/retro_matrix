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
#include <list>
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
//#include "start_menu.h"



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

struct Piece {
  vector<Point> points;
  Point pos;
  int rot;
  int colour;
  int length;
  int width;
};




vector<Point> get_points(vector<Point> points, Point pos, int rot, int length, int width) {
  // get the points in a shape at position pos with rotation rot 
  
  
  vector<Point> new_points;
  int row; 
  int col;
  switch(rot){
    
    case 0:
      for(int i = 0; i < points.size(); i++){
        row = points[i].row + pos.row;
        col = points[i].col +pos.col;
        new_points.push_back(Point{row, col});
        
      }
      break;
    
    case 1:
      for(int i = 0; i < points.size(); i++){
        row = points[i].col + pos.row;
        col = -points[i].row+length-1 +pos.col;
        new_points.push_back(Point{row, col});
        
      }
      break;
    
    case 2:
      for(int i = 0; i < points.size(); i++){
        col = -points[i].col +width-1 + pos.col;
        row = -points[i].row +length-1 + pos.row;
        new_points.push_back(Point{row, col});
      }
      break;
      
     
    case 3:
      for(int i = 0; i < points.size(); i++){
        row = -points[i].col +width-1 + pos.row;
        col = points[i].row + pos.col;
        new_points.push_back(Point{row, col});
      }
      break;
      
  }
  return new_points;
  
  
}

vector<Point> get_points(Piece piece, Point pos, int rot) {
  // get the points in a shape at position pos with rotation rot 
  return get_points(piece.points, piece.pos, rot, piece.length, piece.width);
  
}


void add_shape(list<vector<int>> &board, vector<Point> points, Point pos, int rot, int length, int width, int color) {
  
  
  int row;
  int col;
  
  
  vector<Point> actual_points = get_points(points, pos, rot, length, width);
  
  
  list<vector<int>>::iterator row_it;
  vector<int>::iterator col_it;
  
  for(int i = 0; i < actual_points.size(); i++){
      if(actual_points[i].row >=0){
        row_it = next(board.begin(), actual_points[i].row);
        col_it = next(row_it->begin(), actual_points[i].col);
        *col_it = color;
      }
  }
  
}

void add_shape(list<vector<int>> &board, Piece active_piece) {
  add_shape(board, active_piece.points, active_piece.pos, active_piece.rot, active_piece.length, active_piece.width, active_piece.colour);
}

void remove_shape(list<vector<int>> &board, Piece active_piece) {
  add_shape(board, active_piece.points, active_piece.pos, active_piece.rot, active_piece.length, active_piece.width, 0);
}


bool check_collision(list<vector<int>> &board, vector<Point> points, Point pos, int rot, int length, int width) {
    
    int row;
    int col;
   
    vector<Point> actual_points = get_points(points, pos, rot, length, width);
    
  
    list<vector<int>>::iterator row_it;
    vector<int>::iterator col_it;
    
    for(int i = 0; i < points.size(); i++){
        row_it = next(board.begin(), actual_points[i].row);
        col_it = next(row_it->begin(), actual_points[i].col);
        row = actual_points[i].row;
        col = actual_points[i].col;
        if(actual_points[i].row >19 ||*col_it !=0 || col < 0 || col > 9){
          return true;
        }
    }
 
    return false;
  }
  
bool check_side_collision(list<vector<int>> &board, Piece active_piece, int dir, int rot) {
  
  if (dir == 'l') {
  
    return check_collision(board, active_piece.points, Point{active_piece.pos.row , active_piece.pos.col -1}, rot, active_piece.length, active_piece.width);
  }else if (dir == 'r') {
    return check_collision(board, active_piece.points, Point{active_piece.pos.row, active_piece.pos.col + 1}, rot, active_piece.length, active_piece.width);
  } else{
    return check_collision(board, active_piece.points, Point{active_piece.pos.row, active_piece.pos.col}, rot, active_piece.length, active_piece.width);
  }

}


bool has_landed(list<vector<int>> &board, Piece active_piece) {
  
  return check_collision(board, active_piece.points, Point{active_piece.pos.row+1, active_piece.pos.col}, active_piece.rot, active_piece.length, active_piece.width);
}


Point p_to_l(Point p) { // converts portrait coords to landscape 
  return Point{31 - p.col, p.row};
}

void draw_board(Canvas *canvas, list<vector<int>> &board, Point board_pos, int scaling, int portrait, float brightness) {
  
  
    list<vector<int>>::iterator row_it;
    vector<int>::iterator col_it;
    
    int color;
    int row = 0;
    int col = 0;
    Point point;
    Color colour;
    
    for(row_it = board.begin(); row_it != board.end(); ++row_it) {
            
      for(col_it = row_it->begin(); col_it != row_it->end(); ++col_it){
         
        switch(*col_it) { // get color of pixel
          case 0:
            colour = Color(0*brightness,0*brightness,0*brightness);
            break;
          case 1:
            colour = Color(0*brightness,200*brightness,255*brightness);
            break;
          case 2:
            colour = Color(0*brightness,0*brightness,255*brightness);
            break;
          case 3:
            colour = Color(255*brightness,150*brightness,0*brightness);
            break;
          case 4:
            colour = Color(255*brightness,255*brightness,0*brightness);
            break;
          case 5:
            colour = Color(0*brightness,255*brightness,50*brightness);
            break;
          case 6:
            colour = Color(255*brightness,0*brightness,0*brightness);
            break;
          case 7:
            colour = Color(150*brightness,0*brightness,255*brightness);
            break;
          
        }
        
        

        for(int r = 0; r < scaling; r++){
          for(int c = 0; c <scaling; c++) {
            
            if(portrait) {
              point = p_to_l(Point{row + board_pos.row + r+1,  col + board_pos.col + c+1});
            } else {
              point = Point{row + board_pos.row+ r +1,  col + board_pos.col+c+1};
            }
        
            SetPixel(canvas, point,colour);
          }
        }
        col += scaling;
      }
      
      row += scaling;
      col = 0;
      
    }
    if(portrait) {
      point = p_to_l(board_pos);
      
      DrawRect(canvas,  point.row-10 * scaling - 1, point.col, 10*scaling + 1, 20*scaling + 1, Color(255*brightness,255*brightness,255*brightness));
    } else{
      DrawRect(canvas,  board_pos.row, board_pos.col, 20*scaling+1, 10*scaling+1, Color(255*brightness,255*brightness,255*brightness));
    }
  
}

vector<int> get_completed_lines(list<vector<int>> &board, int min_row, int max_row) {
  
    vector<int> completed_lines;
    
    list<vector<int>>::iterator row_it;
    vector<int>::iterator col_it;
    int current_row = min_row;
     for(row_it = next(board.begin(), min_row); row_it != next(board.begin(), max_row+1); ++row_it) {
      
        int row_prod = 1;
        for(col_it = row_it->begin(); col_it != row_it->end(); ++col_it){
          
          row_prod *= *col_it;
        
        }
        
        if (row_prod != 0) {
          completed_lines.push_back(current_row);
        }
        current_row += 1;
    }
    return completed_lines;
  
}

void remove_lines(list<vector<int>> &board, vector<int> lines){
  
  
  for(int i = 0; i < lines.size(); i++){
    cout << "removing line: ";
    cout << lines.at(i)<<endl;
    
    board.erase(next(board.begin(), lines.at(i)));
    cout<<board.size()<<endl;
    board.push_front({0,0,0,0,0,0,0,0,0,0});
    cout<<board.size()<<endl;
  }
  cout << "\n";
}
// 0: empty, 1: lightblue, 2: darkblue  , 3:orange, 4:yellow, 5:green, 6:red, 7:magenta 


    
int RetroMatrix::run_tetris() {
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
  
  
 

  

  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }
  
  float brightness = 0.5;
  Color light_blue(0*brightness,200*brightness,255*brightness); // line
  Color dark_blue(0*brightness,0*brightness,255*brightness); // baackwards L
  Color orange(255*brightness,150*brightness,0*brightness); // L
  Color yellow(255*brightness, 225*brightness, 0*brightness); // square
  Color green(0*brightness,255*brightness,50*brightness); // sqiggle
  Color red(255*brightness, 0*brightness, 0*brightness); //other squiggle 
  Color magenta(150*brightness, 0*brightness, 255*brightness); // cross
  
  Color bg_color(0,0,0);
  
  int letter_spacing = 0;
  
 
  SetPixel(canvas, 10, 52, magenta);
  SetPixel(canvas, 10, 53, magenta);
  SetPixel(canvas, 11, 52, magenta);
  SetPixel(canvas, 11, 53, magenta);
  
  // 0: empty, 1: lightblue, 2: darkblue  , 3:orange, 4:yellow, 5:green, 6:red, 7:magenta 
  
  list<vector<int>> board; //the tetris board 
  
  // make a 20x10 board
  vector<int> line;
  for(int i = 0; i<20; i++){
    line = {0,0,0,0,0,0,0,0,0,0};
    board.push_back(line);
  }
  
  
  
  list<vector<int>>::iterator row_it;
  vector<int>::iterator col_it;
  
  for(row_it = board.begin(); row_it != board.end(); ++row_it) {
    for(col_it = row_it->begin(); col_it != row_it->end(); ++col_it){
      //*col_it = 2;
    }
    
  }
  
  ResetCanvas(canvas, 32, 64, bg_color);
  
  
  vector<Point> I{Point{0,0}, Point{1,0}, Point{2,0}, Point{3,0}};
  vector<Point> J{Point{0,1}, Point{1,1}, Point{2,1}, Point{2,0}};
  vector<Point> L{Point{0,0}, Point{1,0}, Point{2,0}, Point{2,1}};
  vector<Point> O{Point{0,1}, Point{1,1}, Point{1,0}, Point{0,0}};
  vector<Point> S{Point{1,0}, Point{1,1}, Point{0,1}, Point{0,2}};
  vector<Point> Z{Point{0,0}, Point{0,1}, Point{1,1}, Point{1,2}};
  vector<Point> T{Point{0,0}, Point{0,1}, Point{1,1}, Point{0,2}};
  
  //add_shape(board, I, Point{0,0}, 0, 4, 1, 1);
  //add_shape(board, I, Point{0,5}, 3, 4, 1, 1);
  //add_shape(board, I, Point{5,0}, 2, 4, 1, 1);
  int rotation = 3;
  int length = 3;
  int width = 2;
  int colour = 3;
  //add_shape(board, L, Point{5,5}, rotation, length, width, colour);
  Point portait_pos{20, 5};
  Point landscape_pos{5, 15};
  Point three_scale_pos{2,0};
  
  
  Point big_pos{2, 21};
  
  Point board_pos = big_pos;
  
  int scale = 3;
  int portrait = 0;
   
  brightness = 0.5;
  
  //draw_board(canvas, board, board_pos, scale, portrait, brightness);
  
  //wont compile if you take this line out 
  //rgb_matrix::DrawText(canvas, font, 0, 8 + font.baseline(), bg_color, &bg_color, to_string(score).c_str(), letter_spacing);
  int t = 100;
  int i = 0;
  list <ControllerInput> inputs;
  int dir;
  

  // 0: empty, 1: lightblue, 2: darkblue  , 3:orange, 4:yellow, 5:green, 6:red, 7:magenta 
  
  rotation = 0;
  colour = 1;
  length = 4; 
  width = 1;
  
  Piece I_piece{I, Point{0,0}, rotation, colour, length, width};
  
  
  colour = 2;
  length = 3; 
  width = 2;
  
  Piece J_piece{J, Point{0,5}, rotation, colour, length, width};
 
  

  colour = 3;
  length = 3; 
  width = 2;
  
  Piece L_piece{L, Point{0,5}, rotation, colour, length, width};
  
  
  colour = 4;
  length = 2; 
  width = 2;
  
  Piece O_piece{O, Point{0,5}, rotation, colour, length, width};
  
  colour = 5;
  length = 2; 
   width = 3;
  
  Piece S_piece{S, Point{0,5}, rotation, colour, length, width};
  
  colour = 6;
  length = 2; 
  width = 3;
  
  Piece Z_piece{Z, Point{0,5}, rotation, colour, length, width};
  
  colour = 7;
  length = 2; 
  width = 3;
  
  Piece T_piece{T, Point{0,5}, rotation, colour, length, width};
  
  
  vector<Piece> all_pieces{I_piece, J_piece, L_piece, O_piece, S_piece, Z_piece, T_piece};
  
  
  int rot = 0;
  int frame = 0;
  
  int rand = random(0, 6);
 // rand = 6;
  Piece active_piece = all_pieces[rand];
  
  // load high score 
  string hs;
  ifstream myfile ("/home/pi/Desktop/retro_matrix/games/tetris/high_score.txt");
  if (myfile.is_open())
  {
    getline (myfile,hs);
    
    myfile.close();
  }
  
  
  Color score_color(0,150,0);
  Color high_score_color(150, 150, 0);
  
  int high_score = stoi(hs);
  int score = 0;
  
  // draw current and hiugh score 
  draw_text(canvas, font, 8 + font.baseline(), 0,  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
  draw_text(canvas, font, 14+2* font.baseline(), 0, high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
  //active_piece.pos.row -= active_piece.length/2;
  //active_piece.pos.col -= active_piece.width/2;
  int frame_skip = 20;
  while (true){
    
    
    //
   
   
    for(const auto &input: inputs){
      
        switch(input.type) {  
          case 'p':
            int quit = start_menu();
            if(quit) {
              return 0;
            } else{
              ResetCanvas(canvas, 32, 64, bg_color);
              draw_text(canvas, font, 8 + font.baseline(), 0,  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
              draw_text(canvas, font, 14+2* font.baseline(), 0, high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
            }
            
        }
      }
    
      
    inputs = get_inputs_from_ps4(dev);
    
   
    // read inputs 
    for(const auto &input: inputs){
      if(input.type == 'N' && input.value == 1) {
        rot = (rotation + 1)%4;
        
        
        
      } else if(input.type == 'E' && input.value == 1) {
        rot = rotation -1; 
        if (rot== -1) {
          rot = 3;
        }
      }
      
     
      
      switch(input.type) {  // go from first input as unlikely to have multiple inputs perframes with no sleep

            case 'y':
              if(input.value == -1) {
                dir = 'u';
              }else if(input.value == 1){
                dir = 'd';
              } else if(input.value ==0) {
                dir = -1;
              }
              break;
              
            case 'x':
              if(input.value == -1) {
                dir = 'l';
              }else if(input.value == 1){
                dir = 'r';
              }else if(input.value == 0) {
                dir = -1;
              }
              break;
            default:
              break;
              
          }
    }
    
    //cout << "-----------" << endl;
    //cout << check_side_collision(board, active_piece, dir, active_piece.rot) << endl;
    //cout << has_landed(board, active_piece)  << endl;
    //cout << "-----------" << endl;
  
    
   
    
    
   
   // draw current and hiugh score 
    
    //rgb_matrix::DrawText(canvas, font, 0, 14+2* font.baseline(), dark_blue, &light_blue, to_string(high_score).c_str(), letter_spacing);
    
    //
    

  
    
    
    
    add_shape(board, active_piece);
    draw_board(canvas, board, board_pos, scale, portrait, brightness);
    draw_text(canvas, font, 8 + font.baseline(), 0,  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
    draw_text(canvas, font, 14+2* font.baseline(), 0, high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
    
   
    
    usleep(1 * 50000);
    remove_shape(board, active_piece);

    
    //rotation
    if(!check_collision(board, active_piece.points, Point{active_piece.pos.row, active_piece.pos.col}, rot, active_piece.length, active_piece.width)){ //rotation
      if(rotation != rot) { //if rotation has changed
        
        rotation = rot;
        active_piece.rot = rotation;
        /*
        if(active_piece.rot == 0 || active_piece.rot == 2){
          active_piece.pos.row -= active_piece.length/2;
          active_piece.pos.col -= active_piece.width/2;
        }else{
          active_piece.pos.col -= active_piece.length/2;
          active_piece.pos.row -= active_piece.width/2;
        }
        */
      }
   
            
    } else {
        rot = rotation;
    }
    
    
     
    //sideways movement
    if(dir == 'l' && !check_collision(board, active_piece.points, Point{active_piece.pos.row, active_piece.pos.col-1}, active_piece.rot, active_piece.length, active_piece.width)){
      active_piece.pos.col -=1;
    } else if(dir == 'r'&& !check_collision(board, active_piece.points, Point{active_piece.pos.row, active_piece.pos.col+1}, active_piece.rot, active_piece.length, active_piece.width)){
      active_piece.pos.col +=1;
    } 

  

    
    // downwards movemnet
    if(!check_collision(board, active_piece.points, Point{active_piece.pos.row+1, active_piece.pos.col}, active_piece.rot, active_piece.length, active_piece.width) && frame % frame_skip ==0 && dir != 'd'){
      active_piece.pos.row += 1;
    } else if (!check_collision(board, active_piece.points, Point{active_piece.pos.row+1, active_piece.pos.col}, active_piece.rot, active_piece.length, active_piece.width) &&  dir == 'd'){
      active_piece.pos.row +=1;
       
    
    }else if(check_collision(board, active_piece.points, Point{active_piece.pos.row+1, active_piece.pos.col}, active_piece.rot, active_piece.length, active_piece.width ) && (frame %frame_skip ==0 || dir == 'd')){
      
      add_shape(board, active_piece);
      
        
       // check to see if any rows removed in the lines affected by the piece
       int min_row = active_piece.pos.row;
       int max_row = min_row;
       
       if(active_piece.rot == 0 || active_piece.rot == 2){
         max_row += active_piece.length-1;
       } else {
         max_row += active_piece.width-1;
      }
      
      vector<int> completed_lines = get_completed_lines(board, min_row, max_row);
      
      if (completed_lines.size() > 0 ){
        // draw current and hiugh score 
        score += completed_lines.size();
        if(score > high_score) {
          high_score = score;
        }
        draw_text(canvas, font, 8 + font.baseline(), 0,  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
        draw_text(canvas, font, 14+2* font.baseline(), 0, high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
        
         //remove the complete lines
        remove_lines(board, completed_lines);
      }
       for(int i = 0; i < completed_lines.size(); i++){
         if((score - i)%10 == 0){
           frame_skip = (int)frame_skip*0.9;
         
         }
       }
    
       rand = random(0, 6);
       // rand = 6;
       active_piece = all_pieces[rand];
       active_piece.pos = Point{0,5};
       active_piece.rot = 0;
       rot = 0;
       rotation = 0;
       if(has_landed(board, active_piece)) {
         ofstream myfile ("/home/pi/Desktop/retro_matrix/games/tetris/high_score.txt");
          if (myfile.is_open())
          {
            myfile << high_score;
            myfile.close();
          }
          add_shape(board, active_piece);
          draw_board(canvas, board, board_pos, scale, portrait, brightness);
          draw_text(canvas, font, 8 + font.baseline(), 0,  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
          draw_text(canvas, font, 14+2* font.baseline(), 0, high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
          usleep(50 * 100000);
          
          return 0;
    
       }
    }
    
    
    
    frame += 1;
    
    
}
}


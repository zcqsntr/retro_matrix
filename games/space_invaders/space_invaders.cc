
#include "led-matrix.h"
#include "retro_matrix.h"
#include "graphics.h"
#include "mylib.h"
#include <sstream>
#include <getopt.h>
#include <string>

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h> 
#include <iostream>
#include <vector> 
#include <iostream>
#include <map>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/input.h>
#include <cstdlib>
#include "libevdev.h"
#include <filesystem>
#include <regex>


using rgb_matrix::Font;
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Color;

using namespace std;

struct Alien{
  vector<Point> points;
  Point pos;
  Color colour;

};

struct Barrier {
  Point pos;
  vector<int> barrier;
  int height;
  int width;
};



void draw_alien(Canvas *canvas, Alien alien) {
  
  for(int i = 0; i < alien.points.size(); i++){
    SetPixel(canvas, Point{alien.pos.row + alien.points[i].row, alien.pos.col + alien.points[i].col}, alien.colour);
  }
}

void remove_alien(Canvas *canvas, Alien alien) {
  
  for(int i = 0; i < alien.points.size(); i++){
    SetPixel(canvas, Point{alien.pos.row + alien.points[i].row, alien.pos.col + alien.points[i].col}, Color(0,0,0));
  }
}

void remove_aliens(Canvas *canvas, vector<Alien> aliens) {
  
  for(int i = 0; i < aliens.size(); i++){
    remove_alien(canvas, aliens[i]);
  }
  
}

void draw_aliens(Canvas *canvas, vector<Alien> aliens) {
  
  for(int i = 0; i < aliens.size(); i++){
    draw_alien(canvas, aliens[i]);
  }
  
}

void move_alien(Alien &alien, Point dir){
  
  
  alien.pos.row += dir.row;
  alien.pos.col += dir.col;
}


void move_aliens(vector<Alien> &aliens, Point dir) {
  for(int i = 0; i < aliens.size(); i++){
    move_alien(aliens[i], dir);
  }
  
}

  
void draw_barrier(Canvas *canvas, Barrier barrier) {
  
  for(int i = 0; i < barrier.barrier.size(); i++){
    Color c(0,0,0);
    switch(barrier.barrier[i]) {
	case 0:
         
	 break;
	case 1:
	  //c.r = 70;
	  c.g = 50;
	  break;
	case 2:
	  //c.r = 50;
	  c.g = 110;
	  break;
	case 3:
	  c.g = 255;
	  break;
	default:
	  break;
    }
  SetPixel(canvas, Point{barrier.pos.row + i/barrier.width, barrier.pos.col + i%barrier.width}, c);	
  }
}

void draw_barriers(Canvas *canvas, vector<Barrier> barriers){
  
  for(int i = 0; i < barriers.size(); i++){
    draw_barrier(canvas, barriers[i]);
  }
}


int get_barrier_hit(Barrier barrier, Point &bullet){
  
  for(int i = 0; i < barrier.barrier.size(); i++){
    if(barrier.pos.row + i/barrier.width == bullet.row &&  barrier.pos.col + i%barrier.width == bullet.col && barrier.barrier[i] > 0) {
      return i;
    }
  }
  return -1;
}

vector<int> get_barrier_hits(Barrier barrier, vector<Point> &bullets) {
  
  vector<int> hits;
  int b;
  
  for(int i = 0; i < bullets.size(); i++){
    
    b = get_barrier_hit(barrier, bullets[i]);
    
    if(b != -1){
      bullets.erase(bullets.begin() + i);
      hits.push_back(b);
    }
    
  }
  
  return hits;
}

void damage_barrier(Barrier &barrier, vector<int> hits) {
  
  
  int hit;
  for(int i = 0; i < hits.size(); i++){
    hit = hits[i];
    
    barrier.barrier[hit] -= 2;
    
    if(hit%barrier.width != barrier.width -1) {
      barrier.barrier[hit + 1] -=1;
    }
    
    if(hit% barrier.width != 0) {
      barrier.barrier[hit - 1] -=1;
    }
   // barrier.barrier[hit + barrier.width] -=1;
    
    if(barrier.barrier[hit] < 0) {
      barrier.barrier[hit] = 0;
    }
     if(barrier.barrier[hit+1] < 0) {
      barrier.barrier[hit+1] = 0;
    }
     if(barrier.barrier[hit-1] < 0) {
      barrier.barrier[hit-1] = 0;
    }
     if(barrier.barrier[hit+ barrier.width] < 0) {
      barrier.barrier[hit+ barrier.width] = 0;
    }
  }

}


void damage_barriers(vector<Barrier> &barriers, vector<Point> &player_bullets, vector<Point> &alien_bullets) {
  
  vector<int> hits;
  
  for(int i = 0; i < barriers.size(); i++){
    
    hits = get_barrier_hits(barriers[i], player_bullets);
    damage_barrier(barriers[i], hits);
    hits = get_barrier_hits(barriers[i], alien_bullets);
    damage_barrier(barriers[i], hits);
  }
}

void draw_bullets(Canvas *canvas, vector<Point> bullets, Color colour) {
  
  for(int i = 0; i < bullets.size(); i ++){
    SetPixel(canvas, bullets[i], colour);
  }
}

void move_bullets(vector<Point> &bullets, int dir) {
  
  for(int i = 0; i < bullets.size(); i ++){
    if(dir == 'u') {
      bullets[i].row -=1;
    }else if(dir == 'd'){
      bullets[i].row += 1;
    }
    
    if(bullets[i].row > 63 || bullets[i].row < 0) {
      bullets.erase(bullets.begin() + i);
    }
  }
}

bool check_hit(Alien alien, Point bullet) {
  
  for(int i = 0; i < alien.points.size(); i ++){
    if(alien.points[i].row + alien.pos.row == bullet.row && alien.points[i].col + alien.pos.col == bullet.col){
      return true;
    }
  }
  
  return false;
}

bool check_hit(Alien alien, vector<Point> &bullets) {
  
  for(int i = 0; i < bullets.size(); i ++){
    if(check_hit(alien, bullets[i])){
      bullets.erase(bullets.begin() + i);
      return true;
    }
  }
  
  return false;
}

void kill_aliens(vector<Alien> &aliens, vector<Point> &bullets){
  
  for(int i = 0; i < aliens.size(); i ++){
    if(check_hit(aliens[i], bullets)){
      
      aliens.erase(aliens.begin() + i);
    }
    
  }
}

void aliens_shoot(vector<Alien> aliens, vector<Point> &alien_bullets){
  for(int i = 0; i < aliens.size(); i ++){
    if(random(0, 100) < 3) {
      alien_bullets.push_back(Point{aliens[i].pos.row, aliens[i].pos.col});
    }
  }
  
}
int RetroMatrix::run_space_invaders(){

	Color colour(255,0,0);
	
	vector<Point> alien0{Point{0,0}, Point{0,1}, Point{0,2}, Point{1,0}, Point{1,1}, Point{1,2}, Point{2,1}};
	vector<Point> alien1{ Point{0,1}, Point{1,0}, Point{1,1}, Point{1,2}, Point{2,0},Point{2,2}};
	vector<Point> alien2{ Point{0,0}, Point{0,2},Point{1,0}, Point{1,1}, Point{1,2}, Point{2,0},Point{2,2}};
	
	vector<Point> alien3{Point{0,0},  Point{0,2}, Point{1,0}, Point{1,1}, Point{1,2}, Point{2,1}};
	vector<Point> alien4{ Point{0,0}, Point{0,2},Point{1,0}, Point{0,1}, Point{1,1},Point{1,2}, Point{2,0},Point{2,2}};

	
	vector<Point> player_points{Point{1,1}, Point{1,2}, Point{1,3}, Point{0,2}, Point{2,0}, Point{2,2}, Point{2,4}};
	
	vector<vector<Point>> alien_shapes{alien0, alien1, alien2, alien3, alien4};
	
	
	vector<Alien> aliens;
	vector<Point> shape;
	
	Point point;
      
	int rand;


	// initialise aliens
	for(int row = 0; row < 5; row ++){
	  for(int col = 0; col < 10; col ++) {

	      rand = random(0, alien_shapes.size()-1);

	      shape = alien_shapes[rand];
	
	      colour = random_colour();

	      Alien alien{shape, Point{4*row, 4*col}, colour};

	      aliens.push_back(alien);
		
	    }
	  }
	  
	  

	// initialise player
	Color player_colour(0,0,255);
	Alien player{player_points, Point{55, 30}, player_colour};
	
	  
	Point barrier_dim{5, 15};
	vector<int> barrier;
	
	for(int i = 0; i < barrier_dim.row*barrier_dim.col; i ++){
	  barrier.push_back(3); // three health levels
	}
	
	// initialise barriers
	
	vector<Barrier> barriers;
	
	for(int x = 0; x < 3; x++){
	  barriers.push_back(Barrier{45, 20*x+5, barrier, barrier_dim.row, barrier_dim.col});
	}
    
      
      // vectors to hold bullets 
      vector<Point> player_bullets;
      vector<Point> alien_bullets;
      
      
      draw_alien(canvas, player);
      draw_aliens(canvas, aliens);
      draw_barriers(canvas, barriers);
      
      
      int i = 0; //frame count
      int j = 0; //alien frame count
      int alien_dir = 1; // left or right dir 
      int player_dir = 0;
      int alien_skip = 10;
      int quit;
      Color bg_color(0,0,0);
      
      list <ControllerInput> inputs;
      while(true){
	  inputs = get_inputs_from_ps4(dev);
	  for(const auto &input: inputs){
	  // read inputs 
	    switch(input.type) {  // go from first input as unlikely to have multiple inputs perframes with no sleep
		  
		  case 'p':
		    quit = start_menu();
		
		    if(quit) {
		      return 0;
		    } else{
		      ResetCanvas(canvas, 64, 64, bg_color);
		      //draw_text(canvas, font, 8 + font.baseline(), 0,  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
		      //draw_text(canvas, font, 14+2* font.baseline(), 0, high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
		    }
		    break;
		  case 'D': // controller disconnect
		    quit = start_menu();
	
		    if(quit) {
		      return 0;
		    } else{
		      ResetCanvas(canvas, 64, 64, bg_color);
		      //draw_text(canvas, font, 8 + font.baseline(), 0,  score_color, bg_color, (char *)to_string(score).c_str(), letter_spacing);
		      //draw_text(canvas, font, 14+2* font.baseline(), 0, high_score_color, bg_color, (char *)to_string(high_score).c_str(), letter_spacing);
		    }
		 
		    break;
		  case 'x':
		    player_dir = input.value;
		   break;
		  case 'E':
		    if (input.value == 1) {
		      player_bullets.push_back(Point{player.pos.row, player.pos.col + 2});
		    }
		    break;
		    
		  default:
		    break;
		
		}
	  }
	
	
	
	
	// update board
	remove_aliens(canvas, aliens);
	remove_alien(canvas, player);
	
	draw_bullets(canvas, player_bullets, Color(0,0,0)); // remove bulets
	draw_bullets(canvas, alien_bullets, Color(0,0,0)); // remove bulets
	
	kill_aliens(aliens, player_bullets);
	
	
	damage_barriers(barriers, player_bullets, alien_bullets);
	
	
	if(i%alien_skip == 0){
	  aliens_shoot(aliens, alien_bullets);
	  if(j % 26 == 0 and j > 0){
	    
	    move_aliens(aliens, Point{1, 0});
	    alien_dir *= -1;
	  } else {
	    move_aliens(aliens, Point{0, alien_dir});
	  }
	  j += 1;
	}
	
	
	if(player.pos.col + player_dir >= 0 && player.pos.col + player_dir <= 59){ 
	  move_alien(player, Point{0, player_dir});
	}
	
	
	
	move_bullets(player_bullets, 'u');
	move_bullets(alien_bullets, 'd');
	
	
	
	
	
	draw_alien(canvas, player);
	
	draw_aliens(canvas, aliens);
	
	draw_barriers(canvas, barriers);
	draw_bullets(canvas,player_bullets, Color(255,255,0));
	draw_bullets(canvas,alien_bullets, Color(255,0,0));
	
	if(check_hit(player, alien_bullets)){
	  usleep(2500000);
	  return 0;
	}
	
	usleep(25000);
	i += 1;
	
      }
		
	
	
      return 0;
}



/*
int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "adafruit-hat";  // or e.g. "adafruit-hat"
  defaults.rows = 64;
  defaults.cols = 64;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = false;
  
  // use --led-slowdown-gpio=4
  
  std::string path = "/dev/input/";
  
  Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;
  
	RetroMatrix matrix(canvas, defaults.rows, defaults.cols);
	
	for (const auto & entry : std::filesystem::directory_iterator(path)){
         
          if(regex_match((char *)entry.path().c_str(), regex("/dev/input/event[0-9]+"))){ //check if device has already been looked at
              
            
            
              struct libevdev *dv = NULL;
              int fd = open((char *)entry.path().c_str() , O_RDONLY|O_NONBLOCK);
              int rc = libevdev_new_from_fd(fd, &dv);
              
              
              if(rc == 0) { // on success
                cout << entry << fd<<rc << endl;
              printf("Input device name: \"%s\"\n", libevdev_get_name(dv));
                      printf("Input device ID: bus %#x vendor %#x product %#x\n",
                         libevdev_get_id_bustype(dv),
                         libevdev_get_id_vendor(dv),
                         libevdev_get_id_product(dv));
      
              }
              
                        
              if(rc== 0 &&(libevdev_has_event_type(dv, EV_ABS) && libevdev_has_event_code(dv, EV_KEY, BTN_NORTH))){ //is a controller
                      cout << " is a controller"<< endl;

                      matrix.dev = dv;
                     
              }
              
                             
          }
	  }
	
  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  //signal(SIGTERM, InterruptHandler);
  //signal(SIGINT, InterruptHandler);
  
  // ps4 controller "/dev/input/event6",

	matrix.run_space_invaders();
  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
 }
*/

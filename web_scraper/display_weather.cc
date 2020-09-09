
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
using rgb_matrix::Font;
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Color;

using namespace std;

/*
volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
  interrupt_received = true;
}
* */


int RetroMatrix::draw_weather(){
 cout << "C ++" << endl;
  int current_selected = 0;
    
  rgb_matrix::Font font;
  if (!font.LoadFont("/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
    fprintf(stderr, "Couldn't load font \n");
  
  }
  
  // make buttons 
  
  int letter_spacing = 0;

  
  Color bg_color(0, 0, 0);
  Color text_color(255,255,0);
  
  list <ControllerInput> inputs;
  ResetCanvas(canvas, 64, 64, bg_color);
  
  //get_inputs_from_ps4(dev); //clear input buffer
  
  int fd;
  int rc;
  struct input_event ev;

  string button_target;
  vector<Button> target_buttons;
  
  
  string weather_message = "";

  cin >> weather_message; 
  //weather_message = "1,2,3,4,5"; 

  
  cout << weather_message<<endl;

    string token;
      istringstream ss(weather_message);

      vector<string> weather_data;
	

      while(getline(ss, token, ',')) { 
	      
	      weather_data.push_back(token);
      }
      cout << weather_data.size() << endl;
      cout << weather_data[0]<< endl;
      cout << weather_data[1]<< endl;
      cout << weather_data[2]<< endl;
      cout << weather_data[3]<< endl;
      cout << weather_data[4]<< endl;
      cout << weather_data[5]<< endl;
      
      
	draw_text(canvas, font, 0, 0,  text_color, bg_color, (char *)weather_data[0].c_str(), letter_spacing);
	draw_text(canvas, font, 1.2*font.baseline(), 1,  text_color, bg_color, (char *)weather_data[1].c_str(), letter_spacing);
	draw_text(canvas, font, 2.4*font.baseline(), 1,  text_color, bg_color, (char *)weather_data[2].c_str(), letter_spacing);
	draw_text(canvas, font, 3.6*font.baseline(), 1,  text_color, bg_color, (char *)weather_data[3].c_str(), letter_spacing);
	draw_text(canvas, font, 4.8*font.baseline(), 1,  text_color, bg_color, (char *)weather_data[4].c_str(), letter_spacing);
	 usleep(50*100000);
	  
	  /*
      inputs = get_inputs_from_ps4(dev);
      for(const auto &input: inputs){
        switch(input.type) {  // go from first input as unlikely to have multiple inputs perframes with no sleep
            
          case 'S':
            if(input.value == 1){
              ResetCanvas(canvas, n_rows, n_cols, bg_color);
              return 0;
            }
            break;
          
             case 'D': // controller disconnect
              fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
              rc = libevdev_new_from_fd(fd, &dev);
              break;
            default:
              break;
            
        }
      }
      */
  
  
	
}




int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "adafruit-hat";  // or e.g. "adafruit-hat"
  defaults.rows = 64;
  defaults.cols = 64;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = false;
  
  // use --led-slowdown-gpio=4
  
  Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;
  
	RetroMatrix matrix(canvas, defaults.rows, defaults.cols);
	
  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  //signal(SIGTERM, InterruptHandler);
  //signal(SIGINT, InterruptHandler);
  
  // ps4 controller "/dev/input/event6",

	matrix.draw_weather();
  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
 }
	
	

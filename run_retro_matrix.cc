

#include "led-matrix.h"
#include "retro_matrix.h"

#include <algorithm>
#include "graphics.h"
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
#include <chrono>


using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;


int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults; 
  //defaults.hardware_mapping = "adafruit-hat";  // or e.g. "adafruit-hat"
  defaults.rows = 64;
  defaults.cols = 64;
  defaults.chain_length = 1;
  //defaults.parallel = 1;
  //defaults.show_refresh_rate = false;
  
  // use --led-slowdown-gpio=4 --led-gpio-mapping=adafruit-hat
  
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
  
  
  
  
  list <ControllerInput> inputs;
  
  std::string dev_root = "/dev/input/event";

  int connected_contr = 0;
 
  
  std::string path = "/dev/input/";
  
  vector<char> devices;
   Color blue(100*matrix.brightness,150*matrix.brightness,255*matrix.brightness);
    Color white(255*matrix.brightness,255*matrix.brightness,255*matrix.brightness);
    
    Point pos = Point{4,2};
    int scale = 2;
    draw_logo(canvas, pos, scale, blue, white);
     rgb_matrix::Font brand_font;
    if (!brand_font.LoadFont("./rpi-rgb-led-matrix/fonts/5x7.bdf")) {
      fprintf(stderr, "Couldn't load font \n");
    
    }
    int letter_spacing = 0;
           
  while(true){
    // ps4 controller "/dev/input/event6",
    connected_contr = 0;
    

   
    Color c1(150*matrix.brightness, 0, 255*matrix.brightness);
    Color c2(70*matrix.brightness, 0, 150*matrix.brightness);
    draw_text(canvas, brand_font, 38 + brand_font.baseline(),  5, blue, Color{0,0,0}, "NEYTH MAKES", letter_spacing);
    
    Point start_pos = Point{42, 10};
     rgb_matrix::Font font;
    if (!font.LoadFont("./rpi-rgb-led-matrix/fonts/4x6.bdf")) {
      fprintf(stderr, "Couldn't load font \n");
    
    }
    int frame = 0;
    bool bright = true;
    
    int flash_t = 1000;
    draw_text(canvas, font, start_pos.row + 3*font.baseline(), start_pos.col, c1 , Color{0,0,0}, "PRESS START!", letter_spacing);
    auto t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    while (connected_contr==0) { // while controller disconnected 
      
      auto new_t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        
        if (new_t - t > flash_t) {
          t = new_t;
          bright = !bright;
          if(bright) {
           
            draw_text(canvas, font, start_pos.row + 3*font.baseline(), start_pos.col, c1 , Color{0,0,0}, "PRESS START!", letter_spacing);
          } else {
            
            draw_text(canvas, font, start_pos.row + 3*font.baseline(), start_pos.col, c2 , Color{0,0,0}, "PRESS START!", letter_spacing);
          }
        }
        
       
        
        for (const auto & entry : std::filesystem::directory_iterator(path)){
   
          if(regex_match((char *)entry.path().c_str(), regex("/dev/input/event[0-9]+")) && !count(devices.begin(), devices.end(), ((string)entry.path()).back())){ //check if device has already been looked at
              
              
            
              struct libevdev *dv = NULL;
              int fd = open((char *)entry.path().c_str() , O_RDONLY|O_NONBLOCK);
              int rc = libevdev_new_from_fd(fd, &dv);
           
              if (rc < 0) {
                      fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
                      cout << entry << fd<<rc << endl;
                      devices.push_back(((string)entry.path()).back());
              }else if(rc == 0) { // on success
                cout << entry << fd<<rc << endl;
              printf("Input device name: \"%s\"\n", libevdev_get_name(dv));
                      printf("Input device ID: bus %#x vendor %#x product %#x\n",
                         libevdev_get_id_bustype(dv),
                         libevdev_get_id_vendor(dv),
                         libevdev_get_id_product(dv));
                devices.push_back(((string)entry.path()).back());
              }
              
                        
              if(rc== 0 &&(libevdev_has_event_type(dv, EV_ABS) && libevdev_has_event_code(dv, EV_KEY, BTN_NORTH))){ //is a controller
                      cout << " is a controller"<< endl;
                         
                        
                        if (connected_contr == 0) {
                            matrix.dev = dv;
                            connected_contr = 1;
                        } else {
                          matrix.dev1 = dv;
                          connected_contr = 2;
                        }
              } else if(rc == 0 && libevdev_has_event_code(dv, EV_KEY, KEY_ESC)) { // is the beyboard 
                cout << " is a keyboard"<< endl;
                         
                        
                        if (connected_contr == 0) {
                            matrix.dev = dv;
                            connected_contr = 1;
                        } else {
                          matrix.dev1 = dv;
                          connected_contr = 2;
                        }
            }
                             
          }
                            
      }
     // cout << endl;
        /*
        for(int i = 0; i < 8; i++){ // better way to do this #yolo
           
            fd = open((char *) (dev_root + std::to_string(i)).c_str(), O_RDONLY|O_NONBLOCK);
            
            cout << i << endl;
            cout << fd << endl;
            rc = libevdev_new_from_fd(fd, &dv);
            
            if((libevdev_has_event_type(dv, EV_ABS) && libevdev_has_event_code(dv, EV_KEY, BTN_NORTH))){ //is a controller
                      
                      printf("Input device name: \"%s\"\n", libevdev_get_name(dv));
                      printf("Input device ID: bus %#x vendor %#x product %#x\n",
                         libevdev_get_id_bustype(dv),
                         libevdev_get_id_vendor(dv),
                         libevdev_get_id_product(dv));
              
              
            } 
        }
        * */
        //cout << connected_contr << endl;
    }
    
    cout << "controllers connected: " <<connected_contr<<endl;
    
    
   
    
    
    while (connected_contr>0)  {

      
      auto new_t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        if (new_t - t > flash_t) {
          t = new_t;
        if(bright) {
          draw_text(canvas, font, start_pos.row  + 3*font.baseline(), start_pos.col , c1 , Color{0,0,0}, "PRESS START!", letter_spacing);
        } else {
          draw_text(canvas, font, start_pos.row  + 3*font.baseline(), start_pos.col, c2 , Color{0,0,0}, "PRESS START!", letter_spacing);
        }
        bright = !bright;
      }
      inputs = get_inputs_from_ps4(matrix.dev);
     
      for(const auto &input: inputs){
        
        cout << input.type<< endl;
        cout << inputs.size() << endl;
        if(input.type == 'p'){
            cout <<"size: " <<matrix.menu_structure["MAIN"].size()<< endl;
      
            matrix.draw_menu(matrix.menu_structure["MAIN"]);
            
        } else if(input.type == 'D'){ // disconnect
          
          connected_contr -= 1;
        }
      }
      frame += 1;
    }
    
  
          // Using the canvas.
    
    
}
  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}

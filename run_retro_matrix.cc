

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
           
  while(true){
    // ps4 controller "/dev/input/event6",
    connected_contr = 0;
    while (connected_contr==0) { // while controller disconnected 
       
        for (const auto & entry : std::filesystem::directory_iterator(path)){
         
          if(regex_match((char *)entry.path().c_str(), regex("/dev/input/event[0-9]+")) && !count(devices.begin(), devices.end(), ((string)entry.path()).back())){ //check if device has already been looked at
              
            
            
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
    
    cout << "conts: " <<connected_contr<<endl;
    
    
    Color white(255,255,255);
    Color blue(100,150,255);
    
    Point pos = Point{5,18};
    
    vector<vector<Point>> blues;
    vector<Point> whites;
       
    
    
    
    blues.push_back({Point{pos.row +15,pos.col +26}, Point{pos.row +15,pos.col +27}, Point{pos.row +15, pos.col +28}});
    blues.push_back({Point{pos.row +14, pos.col +25}, Point{pos.row +14,pos.col +26}, Point{pos.row +14,pos.col +27}, Point{pos.row +14, pos.col +28}, Point{pos.row +14, pos.col +29}});
    
    for(int i = 13; i >1; i--){
      whites.push_back(Point{pos.row + i, pos.col +27});
      //blues.push_back({Point{pos.row + i, pos.col +25}, Point{pos.row + i, pos.col +26}, Point{pos.row + i, pos.col +28}, Point{pos.row + i, pos.col +29}});
    }
    
    for(int i = 0; i < 7; i++){
      whites.push_back(Point{pos.row + 3 + i, pos.col + 26 - i});
    }
    
    for(int i = 0; i < 6; i++){
      whites.push_back(Point{pos.row + 8 - i, pos.col + 19 - i});
    }
    
    for (int i = 0; i < 12; i++ ){
      whites.push_back(Point{pos.row + 2 + i, pos.col + 13});
    }
    
    for (int i = 0; i < 11; i++ ){
      whites.push_back(Point{pos.row + 13 - i, pos.col + 13 - i});
    }
    
    for (int i = 0; i < 14; i++ ){
      whites.push_back(Point{pos.row + 2 + i, pos.col + 2});
    }
    
    for(int i = 0; i < 2; i++){
      for(auto point: blues[i]) {
        SetPixel(canvas, point, blue);
      }
      usleep(25 * 1000);
    }
    
    for(int i = 0; i < whites.size() - 2; i++) {
      Point point = whites[i];
      
                      
      if(i == whites.size()-3) {
        blues.push_back({Point{point.row -1, point.col }, Point{point.row-2, point.col }, Point{point.row -1, point.col-1 }, Point{point.row-2, point.col-1 }, Point{point.row -1, point.col+1 }, Point{point.row-2, point.col+1 }, 
                        Point{point.row +1, point.col }, Point{point.row+2, point.col }, Point{point.row +1, point.col-1 }, Point{point.row+2, point.col-1 }, Point{point.row +1, point.col+1 }, Point{point.row+2, point.col+1 }, 
                        Point{point.row, point.col-1 }, Point{point.row , point.col-2 }, Point{point.row +1, point.col-2 }, Point{point.row-1 , point.col-2 },
                        Point{point.row, point.col+1 }, Point{point.row , point.col+2 }, Point{point.row +1, point.col+2 }, Point{point.row-1 , point.col+2 }, Point{point.row+2 , point.col+2 }, Point{point.row+2 , point.col-2 }
                      });
      } else {
        blues.push_back({Point{point.row -1, point.col }, Point{point.row-2, point.col }, Point{point.row -1, point.col-1 }, Point{point.row-2, point.col-1 }, Point{point.row -1, point.col+1 }, Point{point.row-2, point.col+1 }, 
                        Point{point.row +1, point.col }, Point{point.row+2, point.col }, Point{point.row +1, point.col-1 }, Point{point.row+2, point.col-1 }, Point{point.row +1, point.col+1 }, Point{point.row+2, point.col+1 }, 
                        Point{point.row, point.col-1 }, Point{point.row , point.col-2 }, Point{point.row +1, point.col-2 }, Point{point.row-1 , point.col-2 },
                        Point{point.row, point.col+1 }, Point{point.row , point.col+2 }, Point{point.row +1, point.col+2 }, Point{point.row-1 , point.col+2 }
                      });
      }
      
    }
    

    for(int i = 0; i < blues.size(); i++){
      for(const Point point: blues[i]) {
        
        // only set the blue pixels that havent already been set to white
        bool set = true;
        for (int j = 0; j<i; j++){
          if(whites[j].row == point.row && whites[j].col == point.col) {
            set = false;
          }

        }
        
        if (set) {
          SetPixel(canvas, point, blue);
        }
      }
      
      
      SetPixel(canvas, whites[i],white);
      usleep(10 * 1000);
    }
    
    rgb_matrix::Font brand_font;
    if (!brand_font.LoadFont("./rpi-rgb-led-matrix/fonts/5x7.bdf")) {
      fprintf(stderr, "Couldn't load font \n");
    
    }
    int letter_spacing = 0;
    Color c1(150, 0, 255);
    Color c2(70, 0, 150);
    draw_text(canvas, brand_font, 23 + brand_font.baseline(),  5, blue, Color{0,0,0}, "NEYTH MAKES", letter_spacing);
    
    rgb_matrix::Font font;
    if (!font.LoadFont("./rpi-rgb-led-matrix/fonts/4x6.bdf")) {
      fprintf(stderr, "Couldn't load font \n");
    
    }
    
    int frame = 0;
    bool bright = true;
    while (connected_contr>0)  {
 
      if (frame%1000000 == 0) {
        
        if(bright) {
          draw_text(canvas, font, 32 + 3*font.baseline(), 10, c1 , Color{0,0,0}, "PRESS START!", letter_spacing);
        } else {
          draw_text(canvas, font, 32 + 3*font.baseline(), 10, c2 , Color{0,0,0}, "PRESS START!", letter_spacing);
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

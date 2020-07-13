

#include "led-matrix.h"
#include "retro_matrix.h"


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
  
  struct libevdev *dv = NULL;
  int fd;
  int rc = -19;
  int dir = 0;
  int current_selected = 0;
  list <ControllerInput> inputs;
  
  
  while(true){
    // ps4 controller "/dev/input/event6",
    
    
    

    
    while (rc == -19 || rc == -9) {
     
            fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
            rc = libevdev_new_from_fd(fd, &dv);
    }
    
    
    
    matrix.dev = dv;
    
    while (!(rc == -19 || rc == -9))  {
     
      inputs = get_inputs_from_ps4(matrix.dev);
     
      for(const auto &input: inputs){
        //cout << inputs.size() << endl;
        if(input.type == 'p'){
            
            matrix.draw_main_menu();
            
        } else if(input.type == 'D'){
          
          fd = open("/dev/input/event6", O_RDONLY|O_NONBLOCK);
          rc = libevdev_new_from_fd(fd, &matrix.dev);
        }
      }
    }
  
          // Using the canvas.
    
    
}
  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}

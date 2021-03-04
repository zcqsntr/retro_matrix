

#include <stdio.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/input.h>
#include <cstdlib>
#include "graphics.h"
#include "libevdev.h"
#include <list>
#include <iostream>
#include "string.h"
using namespace std;
struct ControllerInput {
  int type;
  int value;
  
};



ControllerInput parse_ps4_input(struct input_event ev){
  
  string input_string = string(libevdev_event_code_get_name(ev.type, ev.code));
 
  
  int type;
  int value; 
  
  if (input_string.compare("ABS_HAT0X") == 0) { // left right dpad
    
    type = 'x';
    value = ev.value;
    
  
  } else if (input_string.compare("ABS_HAT0Y") == 0) { // up down dpad
    
    type = 'y';
    value = ev.value;
    
    //face buttons
  } else if (input_string.compare("BTN_SOUTH") == 0){ 
    type = 'S';
    value = ev.value;
    
    
  } else if (input_string.compare("BTN_EAST") == 0){
    type = 'E';
    value = ev.value;
    
  } else if (input_string.compare("BTN_WEST") == 0){
    type = 'W';
    value = ev.value;
    
  } else if (input_string.compare("BTN_NORTH") == 0){
    type = 'N';
    value = ev.value;
    
    // bumpers
  } else if (input_string.compare("BTN_TL") == 0){
    type = 'l';
    value = ev.value;
  } else if (input_string.compare("BTN_TR") == 0){
    type = 'r';
    value = ev.value;
  
  // triggers
  } else if (input_string.compare("BTN_TL2") == 0){
    type = 'L';
    value = ev.value;
  } else if (input_string.compare("BTN_TR2") == 0){
    type = 'R';
    value = ev.value;
    
  // start button
  } else if (input_string.compare("BTN_START") == 0){
    type = 'p';
    value = ev.value;
    
    // input we arent interested in 
  } else {
      type = -1;
      value = 0;
  }
  
  
  
  ControllerInput input{type, value};
  return input;
  
}


list<ControllerInput> get_inputs_from_ps4(struct libevdev *dev){
      struct input_event ev;
      int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
      list<ControllerInput> inputs;
      
      while(rc == 0) { // get all events since last iteration of game loop 
      
        if (rc == 0) {
          ControllerInput input = parse_ps4_input(ev);
          
          if(input.type != -1){
            inputs.push_back(input);
           }
        }  
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);  
      }
      
      return inputs;
}



using namespace std;
int main(int argc, char **argv)
{
    struct libevdev *dev = NULL;
    int fd;
    int rc = 1;
    // ps4 controller "/dev/input/event6"
    // keyboard "/dev/input/event0"
    
    fd = open("/dev/input/event0", O_RDONLY|O_NONBLOCK);
    rc = libevdev_new_from_fd(fd, &dev);
    
    
    
    
    
    cout << rc << endl;
    if (rc < 0) {
            fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
            exit(1);
    }
    printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
    printf("Input device ID: bus %#x vendor %#x product %#x\n",
           libevdev_get_id_bustype(dev),
           libevdev_get_id_vendor(dev),
           libevdev_get_id_product(dev));
    if (libevdev_has_event_type(dev, EV_ABS) &&
        libevdev_has_event_code(dev, EV_KEY, BTN_NORTH)) {
            printf("This device looks like a controller\n");
            //exit(1);
    }
    /*
    while(true){
        list<ControllerInput> inputs = get_inputs_from_ps4(dev);
        
        for(const auto &i: inputs){
            
            cout << (char)i.type << " " << (int)i.value <<endl <<"\n";
            
        }
    }
    */
      
    
    
    do {
            struct input_event ev;
            
            rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
            
            
   
            if (rc == 0 && !(string(libevdev_event_code_get_name(ev.type, ev.code)).compare("SYN_REPORT") == 0)){// && !(string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_X") == 0) && !(string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_Y") == 0) && !(string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_RY") == 0)&& !(string(libevdev_event_code_get_name(ev.type, ev.code)).compare("ABS_RX") == 0))
                    printf("Event: %s %s %d\n",
                           libevdev_event_type_get_name(ev.type),
                           libevdev_event_code_get_name(ev.type, ev.code),
                           ev.value);
                           string input_string = string(libevdev_event_code_get_name(ev.type, ev.code));
                           cout << (int)input_string.back()<<endl;;
                         }
    } while (rc == 1 || rc == 0 || rc == -EAGAIN || rc == -19);
    
}   

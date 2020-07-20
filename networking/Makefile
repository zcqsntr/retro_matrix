CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter --std=c++17
CXX = g++
CXXFLAGS=$(CFLAGS)
OBJECTS=snake.o
BINARIES=snake

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

EVDEV_LIB_DISTRIBUTION=/usr/local
EVDEV_INCDIR=/usr/local/include/libevdev-1.0/libevdev
EVDEV_LIBDIR=/usr/local/lib
EVDEV_LIBRARY_NAME=evdev
EVDEV_LIBRARY=$(EVDEV_LIBDIR)/lib$(EVDEV_LIBRARY_NAME).a
LDFLAGS+= -L$(EVDEV_LIBDIR) -l$(EVDEV_LIBRARY_NAME) 

MY_INCDIR = /home/pi/Desktop/retro_matrix/lib
MY_LIB = retromatrix

LDFLAGS+= -L$(MY_INCDIR) -l$(MY_LIB) 
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread
LDFLAGS+= -L$(EVDEV_LIBDIR) -l$(EVDEV_LIBRARY_NAME) 




all : paint



communication : communication.o
	$(CXX) $< -o $@ 
	
communication.o: communication.cc 
	$(CXX)  $(CXXFLAGS)   -c communication.cc 

paint : paint.o  $(RGB_LIBRARY) $(EVDEV_LIBRARY)
	$(CXX) paint.o  -o $@ $(LDFLAGS) 
	
paint.o: paint.cc 
	$(CXX) -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS)  -c paint.cc



clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE

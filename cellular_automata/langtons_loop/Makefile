CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
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
MENU_DIR = /home/pi/Desktop/retro_matrix/menus
MY_INCDIR = /home/pi/Desktop/retro_matrix/lib
MY_LIB = retromatrix

LDFLAGS+= -L$(MY_INCDIR) -l$(MY_LIB) 




all : run_loop

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

run_loop : run_loop.o  retro_matrix.o langtons_loop.o start_menu.o $(RGB_LIBRARY) $(EVDEV_LIBRARY)
	$(CXX) run_loop.o retro_matrix.o langtons_loop.o  start_menu.o -o $@ $(LDFLAGS) 

run_loop.o: run_loop.cc 
	$(CXX) -o run_loop.o -I/home/pi/Desktop/retro_matrix -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) -I/home/pi/Desktop/retro_matrix  $(CXXFLAGS) $(LDLIBS) -c run_loop.cc
	
retro_matrix.o: /home/pi/Desktop/retro_matrix/retro_matrix.cc 
	$(CXX) -o retro_matrix.o -I$(RGB_INCDIR) -I$(MY_INCDIR) -I$(EVDEV_INCDIR) -I.  $(CXXFLAGS) $(LDLIBS) -I$(MENU_DIR) -c /home/pi/Desktop/retro_matrix/retro_matrix.cc
	
	
start_menu.o: $(MENU_DIR)/start_menu.cc 
	$(CXX) -o start_menu.o -I$(MY_INCDIR) -I/home/pi/Desktop/retro_matrix -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) -I$(MENU_DIR) $(CXXFLAGS) $(LDLIBS) -c $(MENU_DIR)/start_menu.cc
	
langtons_loop.o: langtons_loop.cc 
	$(CXX) -o langtons_loop.o -I/home/pi/Desktop/retro_matrix -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS)  -c langtons_loop.cc

	

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE

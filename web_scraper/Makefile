CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter --std=c++17
CXX = g++
CXXFLAGS=$(CFLAGS)
OBJECTS=main_menu.o include.o
BINARIES=main_menu include

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix


EVDEV_LIB_DISTRIBUTION=/usr/local
EVDEV_INCDIR=/usr/local/include/libevdev-1.0/libevdev
EVDEV_LIBDIR=/usr/local/lib
EVDEV_LIBRARY_NAME=evdev
EVDEV_LIBRARY=$(EVDEV_LIBDIR)/lib$(EVDEV_LIBRARY_NAME).a



MY_INCDIR = /home/pi/Desktop/retro_matrix/lib
MY_LIB = retromatrix

LDFLAGS+=-lstdc++fs
LDFLAGS+= -L$(MY_INCDIR) -l$(MY_LIB) 
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread
LDFLAGS+= -L$(EVDEV_LIBDIR) -l$(EVDEV_LIBRARY_NAME) 


SNAKE_DIR = /home/pi/Desktop/retro_matrix/games/snake
TETRIS_DIR = /home/pi/Desktop/retro_matrix/games/tetris

GOL_DIR = /home/pi/Desktop/retro_matrix/cellular_automata/game_of_life
R30_DIR = /home/pi/Desktop/retro_matrix/cellular_automata/rule30
ANT_DIR = /home/pi/Desktop/retro_matrix/cellular_automata/langtons_ant
LOOP_DIR = /home/pi/Desktop/retro_matrix/cellular_automata/langtons_loop

MENU_DIR = /home/pi/Desktop/retro_matrix/menus

all : display_weather

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

display_weather: display_weather.o retro_matrix.o $(RGB_LIBRARY) $(EVDEV_LIBRARY)
	$(CXX) display_weather.o retro_matrix.o -o $@ $(LDFLAGS) 

display_weather.o: display_weather.cc 
	$(CXX) -o display_weather.o -I$(RGB_INCDIR) -I/home/pi/Desktop/retro_matrix -I$(MY_INCDIR)  -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS) -I$(MENU_DIR) -c display_weather.cc

retro_matrix.o: /home/pi/Desktop/retro_matrix/retro_matrix.cc 
	$(CXX) -o retro_matrix.o -I$(RGB_INCDIR) -I/home/pi/Desktop/retro_matrix -I$(GOL_DIR) -I$(MY_INCDIR)  -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS) -I$(MENU_DIR) -c /home/pi/Desktop/retro_matrix/retro_matrix.cc

clean:
	rm *.o

FORCE:
.PHONY: FORCE

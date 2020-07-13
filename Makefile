CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
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

LDFLAGS+= -L$(MY_INCDIR) -l$(MY_LIB) 
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread
LDFLAGS+= -L$(EVDEV_LIBDIR) -l$(EVDEV_LIBRARY_NAME) 


SNAKE_DIR = /home/pi/Desktop/retro_matrix/games/snake
TETRIS_DIR = /home/pi/Desktop/retro_matrix/games/tetris

GOL_DIR = /home/pi/Desktop/retro_matrix/cellular_automata/game_of_life
R30_DIR = /home/pi/Desktop/retro_matrix/cellular_automata/rule30
ANT_DIR = /home/pi/Desktop/retro_matrix/cellular_automata/langtons_ant

MENU_DIR = /home/pi/Desktop/retro_matrix/menus

all : run_retro_matrix

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

run_retro_matrix: run_retro_matrix.o retro_matrix.o main_menu.o start_menu.o cellular_automata_menu.o games_menu.o snake_menu.o snake.o tetris.o game_of_life.o rule_30.o ant.o $(RGB_LIBRARY) $(EVDEV_LIBRARY)
	$(CXX) run_retro_matrix.o main_menu.o retro_matrix.o start_menu.o cellular_automata_menu.o games_menu.o snake_menu.o game_of_life.o snake.o tetris.o rule_30.o ant.o -o $@ $(LDFLAGS) 

run_retro_matrix.o: run_retro_matrix.cc 
	$(CXX) -o run_retro_matrix.o -I$(RGB_INCDIR) -I. -I$(GOL_DIR) -I$(MY_INCDIR)  -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS) -I$(MENU_DIR) -c run_retro_matrix.cc

retro_matrix.o: retro_matrix.cc 
	$(CXX) -o retro_matrix.o -I$(RGB_INCDIR) -I. -I$(GOL_DIR) -I$(MY_INCDIR)  -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS) -I$(MENU_DIR) -c retro_matrix.cc

main_menu.o: $(MENU_DIR)/main_menu.cc 
	$(CXX) -o main_menu.o -I$(RGB_INCDIR) -I. -I$(GOL_DIR) -I$(MY_INCDIR)  -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS) -I$(MENU_DIR) -c $(MENU_DIR)/main_menu.cc
	
games_menu.o: $(MENU_DIR)/games_menu.cc 
	$(CXX) -o games_menu.o -I$(MY_INCDIR) -I. -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) -I$(MENU_DIR) -I$(TETRIS_DIR) $(CXXFLAGS) $(LDLIBS) -c $(MENU_DIR)/games_menu.cc
	
start_menu.o: $(MENU_DIR)/start_menu.cc 
	$(CXX) -o start_menu.o -I$(MY_INCDIR) -I. -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) -I$(MENU_DIR) $(CXXFLAGS) $(LDLIBS) -c $(MENU_DIR)/start_menu.cc

snake_menu.o: $(MENU_DIR)/snake_menu.cc 
	$(CXX) -o snake_menu.o -I$(SNAKE_DIR) -I. -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(MENU_DIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS) -c $(MENU_DIR)/snake_menu.cc
	
cellular_automata_menu.o: $(MENU_DIR)/cellular_automata_menu.cc
	$(CXX) -o cellular_automata_menu.o -I. -I$(ANT_DIR) -I$(R30_DIR) -I$(GOL_DIR) -I$(MENU_DIR) -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS) -c $(MENU_DIR)/cellular_automata_menu.cc

snake.o: $(SNAKE_DIR)/snake.cc 
	$(CXX) -I. -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(MENU_DIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS)  -c $(SNAKE_DIR)/snake.cc
	
tetris.o: $(TETRIS_DIR)/tetris.cc 
	$(CXX) -I. -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(MENU_DIR)  -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS)  -c $(TETRIS_DIR)/tetris.cc
	
game_of_life.o: $(GOL_DIR)/game_of_life.cc 
	$(CXX) -I. -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(MENU_DIR)  -I$(EVDEV_INCDIR) $(CXXFLAGS) -c $(GOL_DIR)/game_of_life.cc

ant.o: $(ANT_DIR)/ant.cc 
	$(CXX) -I. -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(MENU_DIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) -c $(ANT_DIR)/ant.cc
	
	
rule_30.o: $(R30_DIR)/rule_30.cc 
	$(CXX) -I. -I$(MY_INCDIR) -I$(RGB_INCDIR) -I$(MENU_DIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) -c $(R30_DIR)/rule_30.cc

clean:
	rm *.o

FORCE:
.PHONY: FORCE

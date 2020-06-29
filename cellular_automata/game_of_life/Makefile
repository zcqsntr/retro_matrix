CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXX = g++
CXXFLAGS=$(CFLAGS)
OBJECTS=game_of_life.o
BINARIES=game_of_life

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread


MY_INCDIR = /home/pi/Desktop/retro_matrix/lib
MY_LIB = retromatrix

LDFLAGS+= -L$(MY_INCDIR) -l$(MY_LIB) 

all : game_of_life.o

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

game_of_life : game_of_life.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)
	
game_of_life.o: game_of_life.cc 
	$(CXX) -I$(MY_INCDIR) -I$(RGB_INCDIR) $(CXXFLAGS) -c game_of_life.cc




clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE

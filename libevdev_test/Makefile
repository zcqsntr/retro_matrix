CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXX = g++
CXXFLAGS=$(CFLAGS)
OBJECTS=test.o
BINARIES=test

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=/home/pi/Desktop/LED_matrix/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a



EVDEV_LIB_DISTRIBUTION=/usr/local
EVDEV_INCDIR=/usr/local/include/libevdev-1.0/libevdev
EVDEV_LIBDIR=/usr/local/lib
EVDEV_LIBRARY_NAME=evdev
EVDEV_LIBRARY=$(EVDEV_LIBDIR)/lib$(EVDEV_LIBRARY_NAME).a

LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -L$(EVDEV_LIBDIR) -l$(EVDEV_LIBRARY_NAME) -lrt -lm -lpthread 



all : test

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

test : test.o $(RGB_LIBRARY) $(EVDEV_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS) 
	
test.o: test.cc 
	$(CXX) -I$(RGB_INCDIR) -I$(EVDEV_INCDIR) $(CXXFLAGS) $(LDLIBS)  -c test.cc




clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE

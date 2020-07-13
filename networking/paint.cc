#include <iostream>
#include <sstream>
#include <string>
#include "led-matrix.h"
#include "graphics.h"
#include "mylib.h"
#include <vector>

using rgb_matrix::Font;
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Color;

using namespace std;

int main(int argc, char* argv[]){
	
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
		
	

    string python_message = "";
    bool quit = false;
    Point coords;
    Color colour;

    while (!quit){
        cin >> python_message;
        //cout << "mess: " << python_message << endl;
	
	if(python_message.compare("clear") == 0) {
	  ResetCanvas(canvas, 64, 64, Color(0,0,0));
	}else {
	  
	    string token;
	    istringstream ss(python_message);
	    
	    vector<int> inputs;
	    
	    
	    while(getline(ss, token, ',')) { 
		    inputs.push_back(stoi(token));
	    }
	    
	    
	    for(int i = 0; i < inputs.size()/5; i++) {
		coords = Point{inputs[5*i], inputs[5*i + 1]};
		colour = Color{inputs[5*i + 2], inputs[5*i + 3], inputs[5*i + 4]};
		SetPixel(canvas, coords, colour);
	    }
	    
	    /*
	    for( int i = 0; i < inputs.size(); i++){
		    cout << i << ": "<< inputs[i] << " ";
	    }
	    cout << endl;
	    
	    cout << coords.row << endl;
	    cout << coords.col << endl;
	    
	    cout << colour.r << " " <<colour.g << " " << colour.b << endl; 
	    */
	    
	  }
	  

    }
   return 0; 
}

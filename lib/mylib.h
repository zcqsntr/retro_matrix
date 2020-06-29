
#ifndef INC_H
#define INC_H


#include "led-matrix.h"
#include "graphics.h"
#include <vector>
#include <list>
#include "libevdev.h"

using rgb_matrix::Font;
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Color;

using namespace std;

struct Point {
  int row, col;
};

struct Button {
  Point position; // left hand corner
  char* text;
  bool is_selected; 
  
};


struct ControllerInput {
  int type;
  int value;
  
};

void draw_buttons(Canvas *canvas, vector <Button> buttons, Font &font, Color selected_color, Color unselected_color);

int get_selected_button(vector<Button> buttons);

int random(int min, int max);

void draw_line(Canvas *c, int row0, int col0, int row1, int col1, const Color &color);

void SetPixel(Canvas *canvas, int row, int col, uint8_t red, uint8_t green, uint8_t blue);

void SetPixel(Canvas *canvas, int row, int col, Color color);

void SetPixel(Canvas *canvas, Point point, Color color);

void draw_text(Canvas *canvas, Font &font, int row, int col, Color color, Color bg_color, char *text, int letter_spacing);

void DrawRect(Canvas *canvas, int row, int col, int height, int width, Color color);

void ResetCanvas(Canvas *canvas, int n_rows, int n_cols, Color bg_color);

void SetPixels(Canvas *canvas, int LED_matrix[32][64], Color color, int n_rows, int n_cols );


ControllerInput parse_ps4_input(struct input_event ev);

list <ControllerInput> get_inputs_from_ps4(struct libevdev *dev);

list<ControllerInput> get_inputs_from_ps4(struct libevdev *dev);

void change_selected_button(vector<Button> &buttons, int dir, int &current_selected);
int get_direction_from_inputs(list <ControllerInput> inputs);
#endif

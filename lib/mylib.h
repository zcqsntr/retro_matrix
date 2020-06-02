
#ifndef INC_H
#define INC_H

#include "led-matrix.h"
#include "graphics.h"
using rgb_matrix::Font;
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Color;

int random(int min, int max);


void SetPixel(Canvas *canvas, int row, int col, uint8_t red, uint8_t green, uint8_t blue);

void SetPixel(Canvas *canvas, int row, int col, Color color);

void draw_text(Canvas *canvas, Font font, int row, int col, Color color, Color bg_color, char *text, int letter_spacing);

void DrawRect(Canvas *canvas, int row, int col, int height, int width, Color color);

void ResetCanvas(Canvas *canvas, int n_rows, int n_cols, Color bg_color);

struct Point {
  int row, col;
};

struct Button {
  Point position; // left hand corner
  char* text;
  bool is_selected; 
  
};


#endif

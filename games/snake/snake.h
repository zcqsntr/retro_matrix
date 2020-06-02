
#ifndef SNK_H
#define SNK_H
#include <deque>
#include "led-matrix.h"
#include "graphics.h"
#include "mylib.h"

using namespace std;
using rgb_matrix::Canvas;
Point spawn_food(deque<Point> snake, int rows, int cols);
int run_snake(Canvas *canvas, int n_players);
static void DrawOnePlayer(Canvas *canvas);
static void DrawTwoPlayer(Canvas *canvas);

#endif

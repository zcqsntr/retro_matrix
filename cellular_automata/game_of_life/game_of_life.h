
#ifndef GOL_H
#define GOL_H

#include <string>
using namespace std;

void RandomICs2D(int LED_matrix[32][64], float density, int n_rows = 32, int n_cols = 64);
void GOLUpdate(int LED_matrix[32][64], string boundary_conditions = "looping", int n_rows = 32, int n_cols = 64);

void MakeGliderGun(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0, int direction = 0);
void MakeGlider(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0, int direction = 0);
static void MakeBlock(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0); 
void MakeBlinker(int LED_matrix[32][64],  int row_loc= 0, int col_loc = 0);
static void DrawGOLOnCanvas(Canvas *canvas);
int run_GOL(Canvas *canvas);

#endif

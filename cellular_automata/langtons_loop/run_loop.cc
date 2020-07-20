
#include "retro_matrix.h"


using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;


int main(int argc, char *argv[]) {
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
  
	RetroMatrix matrix(canvas, defaults.rows, defaults.cols);
	
  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  //signal(SIGTERM, InterruptHandler);
  //signal(SIGINT, InterruptHandler);
  
  // ps4 controller "/dev/input/event6",
  
  
  matrix.run_loop();    // Using the canvas.

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}

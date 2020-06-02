# retro_matrix

## Dependencies:
rpi-rgb-led-matrix: https://github.com/hzeller/rpi-rgb-led-matrix
libevdev

## PS4 controller button mapping:

ev.type = ABS_HAT0X, ABS_HAT0Y are the X and Y directions on the D pad, with ev.value = -1, +1 
	for left, right and down, up (note up and down inverse) and 0 for any button released

ev.type = BTN_SOUTH, BTN_EAST, BTN_NORTH,BTN_WEST for the X,O,TRI,SQU button, ev.value = 
	1 when pressed, 0 released 
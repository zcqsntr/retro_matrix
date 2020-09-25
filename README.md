# retro_matrix
This is a C++ apllication that runs various cellular automata and retro games on a raspberry Pi and an LED matrix. It can be controlled using a controller, I have used a PS4, and Nintendo Switch Pro controller successfully. Consider this a pre alpha version, install and play around with it if you are interested, but it doesn't function as a finished product yet. This requires a raspberry Pi, I have tested on the Pi 4 and a headless Pi zero. The Pi 4 works flawlessly, the Pi zero works alright for some of the cellular automata, but the games are too laggy to play. This was written for a 64x64 matrix, but have also tried most of it on a 32x64 matrix. This isnt officially supported right now and requires editing the code but try it if you like. 

## You will need
* Raspberry Pi, preferably better than a Pi zero
* LED matrix, preferably 64x64
* A Nintendo Switch Pro controller, PS4 controller or it also works with one of these cheaper [controllers](https://www.amazon.co.uk/8BitDo-Wireless-Bluetooth-Controller-Powerful/dp/B083FJLVLH/ref=sr_1_17_sspa?dchild=1&keywords=8bitdo+controller&qid=1601051019&sr=8-17-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzME5UMFlLRTVTMzVSJmVuY3J5cHRlZElkPUEwOTQwMTY2MkJWQURBNjhORUE0QSZlbmNyeXB0ZWRBZElkPUEwMjQ5OTMzMUZHSk1GRUFSNjg4NCZ3aWRnZXROYW1lPXNwX210ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=) 
* (recommended) matrix hat or bonnet, so you don't have to wire it to the GPIO pins yourself
* (optional) 3D printer to print the case
* (optional) 5v fan to prevent overheating in the case

## Install instructions

Note that this is a very early version and the install process is not at all streamlined, wont be too difficult if you have some experience with compiling C++ libraries though. The binary is currently in the repository so you might not need to compile

1) Install the dependancies below and change the Makefile to point to the libraries. 
2) ./setup.sh to run the compilation and a command that reduces flickering 
3) ./run_retro_matrix
4) fix any bugs you have during this process
5) (optional)  3D print the case found here https://www.thingiverse.com/thing:4605737

## Dependencies:
rpi-rgb-led-matrix: https://github.com/hzeller/rpi-rgb-led-matrix

libevdev

## PS4 controller button mapping:

ev.type = ABS_HAT0X, ABS_HAT0Y are the X and Y directions on the D pad, with ev.value = -1, +1 
	for left, right and down, up (note up and down inverse) and 0 for any button released

ev.type = BTN_SOUTH, BTN_EAST, BTN_NORTH,BTN_WEST for the X,O,TRI,SQU button, ev.value = 
	1 when pressed, 0 released 

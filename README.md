# retro_matrix

Follow me on Instagram for more projects! [@neyth_makes](http://instagram.com/neyth_makes/)

This is a C++ apllication that runs various cellular automata and retro games on a raspberry Pi and an LED matrix. It can be controlled using a controller, I have used a PS4, and Nintendo Switch Pro controller successfully. Consider this a pre alpha version, install and play around with it if you are interested, but it doesn't function as a finished product yet. This requires a raspberry Pi, I have tested on the Pi 4 and a headless Pi zero. The Pi 4 works flawlessly, the Pi zero works alright for some of the cellular automata, but the games are too laggy to play. This was written for a 64x64 matrix, but have also tried most of it on a 32x64 matrix. This isnt officially supported right now and requires editing the code but try it if you like. It can also interface with an iPad companion [app](https://github.com/zcqsntr/retro_matrix_companion) I wrote, the code for this is in the networking folder. The iPad app allows you to paint individual pixels and communicates these over a local network to the matrix.

## Games 
* Snake, one and two player
* Tetris 
* Space Invaders

## Cellular Automata 
* Conway's game of life 
* Rule 30
* Langton's ant
* Langton's loop

## You will need
* Raspberry Pi, preferably better than a Pi zero. Tested oon Pi3A, Pi4 and Pi0. All work although the 0 is quite laggy
* [LED matrix](https://shop.pimoroni.com/products/rgb-led-matrix-panel?variant=3029531983882), 64x64 is currently the only supported size
* A Nintendo Switch Pro controller, PS4 controller or it also works with one of these cheaper [controllers](https://www.amazon.co.uk/8BitDo-Wireless-Bluetooth-Controller-Powerful/dp/B083FJLVLH/ref=sr_1_17_sspa?dchild=1&keywords=8bitdo+controller&qid=1601051019&sr=8-17-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzME5UMFlLRTVTMzVSJmVuY3J5cHRlZElkPUEwOTQwMTY2MkJWQURBNjhORUE0QSZlbmNyeXB0ZWRBZElkPUEwMjQ5OTMzMUZHSk1GRUFSNjg4NCZ3aWRnZXROYW1lPXNwX210ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=) 
* (recommended) matrix hat or [bonnet](https://shop.pimoroni.com/products/adafruit-rgb-matrix-bonnet-for-raspberry-pi), so you don't have to wire it to the GPIO pins yourself 
* (optional) 3D printer to print the [case](https://www.thingiverse.com/thing:4605737)
* (optional) 5v fan to prevent overheating in the case

## Install instructions

If you don't need to recompile follow these steps, if you do follow these and the ones below first install git and libevdev:

```console
sudo apt-get update
sudo apt-get install git
sudo apt-get install -y libevdev-tools
```

then create a directory and inside that directory run

```console
git clone https://github.com/zcqsntr/retro_matrix --recursive
```

If the application doesn't run after you've done this try the steps below. 

If you need to recompile for any reason then you need to setup libevdev first: 

```console
sudo apt-get install autoconf
sudo apt-get install libtool
cd libevdev

./autogen.sh 
./configure
make
make install
```

Then run my setup script which compiles everything and runs a command on the excecutable to reduce flickering. 

```console

cd ..
./setup
```

## Run instructions
Then to run the application you need to run (from the root retro_matrix directory)

```console
./run_retro_matrix <args>
```

where args are the arguments required to get the rgb-rpi demos working, e.g. I use on a Pi3 and Pi4

```console
./run_retro_matrix --led-slowdown-gpio=4 --led-gpio-mapping=adafruit-hat
```

You should get some console output.

## Use instructions
You need either a controller or a keyboard connected to run, not all controllers are supported but all keyboards should work. 

To start the application press the start button (esc on keyboard), if start doesn't work try pressing random buttons as the input mapping might be slightly different on different controllers. 

The controller button mappings are standard. For the keyboard, directions are w,a,s,d. Space is select (the A button on controller) and j,k,l are the remaining face buttons, with l being back (the B button on controller). 

If anything doesn't work let me know and I'll try and fix it

## Dependencies:
* [libevdev](https://www.freedesktop.org/wiki/Software/libevdev/)
* pip install pytz (for weather)


![IMG_20200924_182925](https://user-images.githubusercontent.com/33317183/94297181-025dd780-ff5c-11ea-822f-bd70cae16fc4.jpg)
![IMG_20200924_183004](https://user-images.githubusercontent.com/33317183/94297233-199cc500-ff5c-11ea-9bb1-b6e7d46e44b3.jpg)
![IMG_20200924_183055](https://user-images.githubusercontent.com/33317183/94297273-29b4a480-ff5c-11ea-858a-7a0df3335b66.jpg)
![IMG_20200924_183216](https://user-images.githubusercontent.com/33317183/94297298-346f3980-ff5c-11ea-8b19-fa941abe2dbc.jpg)
![IMG_20200925_173649](https://user-images.githubusercontent.com/33317183/94297326-3e913800-ff5c-11ea-8043-c144f5955fe7.jpg)
![IMG_20200925_173748](https://user-images.githubusercontent.com/33317183/94297378-536dcb80-ff5c-11ea-9276-d1f97b62e455.jpg)
![IMG_20200925_173807](https://user-images.githubusercontent.com/33317183/94297469-75ffe480-ff5c-11ea-872a-1de910d49d0f.jpg)
![IMG_20200925_173839](https://user-images.githubusercontent.com/33317183/94297561-9e87de80-ff5c-11ea-859d-28a4ce01e2dc.jpg)
![MVIMG_20200925_173727](https://user-images.githubusercontent.com/33317183/94297697-d55df480-ff5c-11ea-9194-afffea160d8e.jpg)
![MVIMG_20200925_173819](https://user-images.githubusercontent.com/33317183/94297799-ffafb200-ff5c-11ea-9fcf-4699693db247.jpg)
![MVIMG_20200925_173902](https://user-images.githubusercontent.com/33317183/94297942-3be31280-ff5d-11ea-9d8a-9d22c5d83f6c.jpg)




make -C ./rpi-rgb-led-matrix
make -C ./lib

make

sudo setcap 'cap_sys_nice=eip' run_retro_matrix

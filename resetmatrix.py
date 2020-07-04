#!/usr/bin/python3

from __future__ import print_function
from gpiozero import LED
from time import sleep
#     0123456789012345

b12a="0111111111111111"
b12b="0111100000111111"
b12c="0111111111111111"
b12d="0111100000111111"

b12a="0111111111111111"
b12b="0111111111111111"
b12c="0111111111111111"
b12d="0111111111111111"

b13a="0000000001000000"
b13b="0000000001000000"
b13c="0000000001000000"
b13d="0000000001000000"

# b12  - 1  adds red tinge
# b12  - 9/8/7/6/5  =  4 bit brightness
# b13  - 9   =1 screen on
# b13  - 6   =1 screen off
xr1=LED(5)
xr2=LED(12)
xg1=LED(13)
xg2=LED(16)
xb1=LED(6)
xb2=LED(23)

xA=LED(22)
xB=LED(26)
xC=LED(27)
xD=LED(20)

xLAT=LED(21)
xCLK=LED(17)
xOE=LED(4)

xCLK.off()

xOE.off()

xA.on()
xB.off()
xC.off()
xD.off()

xr1.off()
xr2.off()
xg1.off()
xg2.off()
xb1.off()
xb2.off()

b12=b12a
b13=b13a

for x in range(128):
	y=x%16
	if(y==0):
		print(' ',end='')
	if (b12[y:y+1] is "0"):
		print('0',end='')
		xr1.off()
		xr2.off()
		xg1.off()
		xg2.off()
		xb1.off()
		xb2.off()
	else:
		print('1',end='')
		xr1.on()
		xr2.on()
		xg1.on()
		xg2.on()
		xb1.on()
		xb2.on()
	xCLK.on()
	sleep(0.001)
	xCLK.off()
	sleep(0.001)
	if(x>31):
		b12=b12b
	if(x>63):
		b12=b12c
	if(x>95):
		b12=b12d
	if(x==(128-12)):
		print('*',end='')
		xLAT.on()

xLAT.off();
print('')

for x in range(128):
        y=x%16
        if(y==0):
                print(' ',end='')
        if (b13[y:y+1] is "0"):
                print('0',end='')
                xr1.off()
                xr2.off()
                xg1.off()
                xg2.off()
                xb1.off()
                xb2.off()
        else:
                print('1',end='')
                xr1.on()
                xr2.on()
                xg1.on()
                xg2.on()
                xb1.on()
                xb2.on()
        xCLK.on()
        sleep(0.001)
        xCLK.off()
        sleep(0.001)
        if(x>31):
                b13=b13b
        if(x>63):
                b13=b13c
        if(x>95):
                b13=b13d
        if(x==(128-13)):
                print('*',end='')
                xLAT.on()

xLAT.off();
print('')

xOE.on();

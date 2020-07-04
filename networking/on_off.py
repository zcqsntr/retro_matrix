import subprocess
import paho.mqtt.client as mqtt
import os
#proc = subprocess.Popen("./communication",


state = "run"

    
def connectionStatus(client, userdata, flags, rc):
    mqttClient.subscribe("rpi/gpio")

def messageDecoder(client, userdata, msg):
    message = msg.payload.decode(encoding='UTF-8')
    
    input = message
    
    if message == "on":
        print("ON!")
        proc = subprocess.Popen(["/home/pi/Desktop/retro_matrix/menus/main_menu", "--led-slowdown-gpio=4"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE)
    elif message == "off":
        
        print("OFF!")
    
        
clientName = "RPI"
serverAddress = "192.168.43.141"

mqttClient = mqtt.Client(clientName)

mqttClient.on_connect = connectionStatus
mqttClient.on_message = messageDecoder

mqttClient.connect(serverAddress)
mqttClient.loop_forever()


'''
while state == "run":
    input = raw_input("Message to CPP>> ")
    
    

    if input == "quit":
        state = "terminate" # any string other than "run" will do

    proc.stdin.write(input + "\n")
    cppMessage = proc.stdout.readline().rstrip("\n") 
    print "cppreturn message ->" + cppMessage + " written by python \n"
'''

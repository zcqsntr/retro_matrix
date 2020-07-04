import subprocess
import paho.mqtt.client as mqtt

proc = subprocess.Popen("./communication",
stdin=subprocess.PIPE,
stdout=subprocess.PIPE)

state = "run"

    
def connectionStatus(client, userdata, flags, rc):
    mqttClient.subscribe("rpi/gpio")

def messageDecoder(client, userdata, msg):
    message = msg.payload.decode(encoding='UTF-8')

    print(message)
    
    if message == "quit":
        state = "terminate" # any string other than "run" will do

    proc.stdin.write(message + "\n")
    cppMessage = proc.stdout.readline().rstrip("\n") 
    print "cppreturn message ->" + cppMessage + " written by python \n"
    
    if message == "on":
        gpio.output(21, gpio.HIGH)
        print("ON!")
    elif message == "off":
        gpio.output(21, gpio.LOW)
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
